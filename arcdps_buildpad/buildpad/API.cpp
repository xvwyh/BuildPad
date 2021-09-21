#include "API.h"
#include "json/json.hpp"
#include "SkillStorage.h"

namespace buildpad
{
void API::LoadSkillData() const
{
    SkillStorage::Instance().SetLoadingState(SkillStorage::LoadingState::Loading);

    auto const loader = [this](std::string_view const data)
    {
        for (auto const& profession : nlohmann::json::parse(data.begin(), data.end(), nullptr, false))
        {
            GW2::Profession id;
            if (auto const itr = util::find_if(GW2::GetProfessionInfos(), util::member_equals(&GW2::ProfessionInfo::Name, profession["id"])))
                id = itr->Profession;
            else
                throw std::exception();

            std::vector<GW2::Specialization> specializations;
            for (auto const& specialization : profession["specializations"])
                specializations.emplace_back((GW2::Specialization)specialization);

            for (auto const& skill : profession["skills"])
            {
                SkillStorage::Instance().AddProfessionSkill(id, skill["id"], [type = skill["type"]]
                {
                    if (type == "Profession")
                        return SkillStorage::SkillType::Profession;
                    if (type == "Heal")
                        return SkillStorage::SkillType::Heal;
                    if (type == "Utility")
                        return SkillStorage::SkillType::Utility;
                    if (type == "Elite")
                        return SkillStorage::SkillType::Elite;
                    return SkillStorage::SkillType::None;
                }());
            }

            auto specialization = specializations.begin();
            for (auto const& training : profession["training"])
            {
                if (training["category"] == "Specializations" || training["category"] == "EliteSpecializations")
                {
                    for (auto const& track : training["track"])
                    {
                        if (track["type"] == "Skill")
                        {
                            auto& skill = *SkillStorage::Instance().GetSkill(track["skill_id"]);
                            skill.Profession = id;
                            skill.Specialization = *specialization;
                        }
                    }
                    ++specialization;
                }
            }

            for (auto const& pair : profession["skills_by_palette"])
                SkillStorage::Instance().AddPalette(pair[0], pair[1]);
        }

        auto const loader = [](std::string_view const data)
        {
            for (auto const& legend : nlohmann::json::parse(data.begin(), data.end(), nullptr, false))
            {
                GW2::RevenantLegend id;
                if (auto const itr = util::find_if(GW2::GetRevenantLegendInfos(), util::member_equals(&GW2::RevenantLegendInfo::Legend, (GW2::RevenantLegend)legend["code"])))
                    id = itr->Legend;
                else
                    throw std::exception();

                SkillStorage::RevenantLegendSkills skills { };
                skills.SwapSkill = legend["swap"];

                uint8_t index = 0;
                skills.Skills[index++] = legend["heal"];
                for (auto const& utility : legend["utilities"])
                    skills.Skills[index++] = utility;
                skills.Skills[index] = legend["elite"];

                SkillStorage::Instance().AddRevenantLegendSkills(id, skills);
            }

            // Needed until the API contains palette assignments for all revenant legend skills (currently only Kalla is present)
            Web::Instance().Request(
                "https://buildpad.gw2archive.eu/skillpalette.json",
                [](std::string_view const data)
                {
                    for (auto const& skill : nlohmann::json::parse(data.begin(), data.end(), nullptr, false))
                        for (auto& palette : skill["palette"])
                            SkillStorage::Instance().AddPalette(palette, skill["id"], skill.contains("override_api") ? (bool)skill["override_api"] : false);

                    SkillStorage::Instance().SetLoadingState(SkillStorage::LoadingState::Done);
                },
                [](auto&&) { SkillStorage::Instance().SetLoadingState(SkillStorage::LoadingState::Failed); }, false);
        };

        Web::Instance().Request(
            fmt::format("https://api.guildwars2.com/v2/legends?lang={}&v={}&ids={}", GetLanguageInfos().front().Tag, m_schema, "all"),
            loader,
            [loader](auto&&)
        {
            Web::Instance().Request(
                "https://buildpad.gw2archive.eu/apifallback_legends.json",
                loader,
                [](auto&&) { SkillStorage::Instance().SetLoadingState(SkillStorage::LoadingState::Failed); }, false);
        }, false);
    };

    Web::Instance().Request(
        fmt::format(InfoHandlers<Profession>::URL, GetLanguageInfos().front().Tag, m_schema, "all"),
        loader,
        [loader](auto&&)
        {
            Web::Instance().Request(
                "https://buildpad.gw2archive.eu/apifallback_professions.json",
                loader,
                [](auto&&) { SkillStorage::Instance().SetLoadingState(SkillStorage::LoadingState::Failed); }, false);
        }, false);
}

void API::PreloadAllPets()
{
    bool result = true;
    std::set<uint32_t> ids;
    for (uint32_t const id : Handler::Instance().GetPets())
        if (id)
            Reserve<Pet>(id, ids, result);

    Request<Pet>(ids);
}

void API::PreloadAllProfessions()
{
    if (!m_professionsRequested)
    {
        m_professionsRequested = true;
        Request<Profession>(std::set<uint32_t> { 1 });
    }
}

void API::PreloadAllProfessionSkills(GW2::Profession profession)
{
    PreloadAllProfessions();

    bool result = true;
    if (auto const& info = Profession::Get((uint32_t)profession))
    {
        std::set<uint32_t> ids;
        for (auto const& id : info.Skills)
            Reserve<Skill>(id, ids, result);

        Request<Skill>(ids);
    }
}

void API::PreloadAllProfessionSpecializations(GW2::Profession profession)
{
    PreloadAllProfessions();

    bool result = true;
    if (auto const& info = Profession::Get((uint32_t)profession))
    {
        std::set<uint32_t> ids;
        for (auto const& specialization : info.Specializations)
        {
            Reserve<Specialization>((uint32_t)specialization, ids, result);

            if (auto& info = Specialization::Get((uint32_t)specialization))
            {
                if (!info.IconLoaded)
                {
                    info.IconLoaded = true;
                    info.Icon.Download(info.IconURL);
                }
            }
        }

        Request<Specialization>(ids);
    }
}

bool API::PreloadAllBuildInfos(Build const& build)
{
    bool result = true;
    auto const& parsed = build.GetParsedInfo();

    {
        std::set<uint32_t> ids;
        for (auto const& info : GW2::GetSpecializationInfos())
            if (auto const id = (uint32_t)info.Specialization; id)
                Reserve<Specialization>(id, ids, result);

        Request<Specialization>(ids);
    }

    if (parsed.SkillsLand || parsed.SkillsWater || parsed.Profession == GW2::Profession::Revenant)
    {
        std::set<uint32_t> ids;
        if (parsed.SkillsLand)
            for (uint32_t const palette : *parsed.SkillsLand)
                if (uint32_t const id = SkillStorage::Instance().FromPalette(palette, parsed.RevenantLegendsLand[0]))
                    Reserve<Skill>(id, ids, result);

        if (parsed.SkillsWater)
            for (uint32_t const palette : *parsed.SkillsWater)
                if (uint32_t const id = SkillStorage::Instance().FromPalette(palette, parsed.RevenantLegendsWater[0]))
                    Reserve<Skill>(id, ids, result);

        if (parsed.Profession == GW2::Profession::Revenant)
        {
            for (auto const& info : GW2::GetRevenantLegendInfos())
                if (auto const* skills = SkillStorage::Instance().GetRevenantLegendSkills(info.Legend))
                    if (uint32_t const id = skills->SwapSkill)
                        Reserve<Skill>(id, ids, result);

            for (auto const& legend : parsed.RevenantLegendsLand)
                if (legend != GW2::RevenantLegend::None)
                    if (auto const* skills = SkillStorage::Instance().GetRevenantLegendSkills(legend))
                        for (uint32_t const id : skills->Skills)
                            Reserve<Skill>(id, ids, result);

            for (auto const& legend : parsed.RevenantLegendsWater)
                if (legend != GW2::RevenantLegend::None)
                    if (auto const* skills = SkillStorage::Instance().GetRevenantLegendSkills(legend))
                        for (uint32_t const id : skills->Skills)
                            Reserve<Skill>(id, ids, result);
        }

        Request<Skill>(ids);
    }

    if (parsed.Profession == GW2::Profession::Ranger)
    {
        std::set<uint32_t> ids;
        for (uint32_t const id : parsed.RangerPetsLand)
            Reserve<Pet>(id, ids, result);

        for (uint32_t const id : parsed.RangerPetsWater)
            Reserve<Pet>(id, ids, result);

        Request<Pet>(ids);
    }

    if (parsed.TraitLines)
    {
        for (auto const& line : *parsed.TraitLines)
        {
            if (auto& info = Specialization::Get((uint32_t)line.Specialization))
            {
                if (!info.IconLoaded)
                {
                    info.IconLoaded = true;
                    info.Icon.Download(info.IconURL);
                }

                std::set<uint32_t> ids;
                for (uint32_t const id : info.MinorTraits)
                    if (Reserve<Trait>(id, ids, result))
                        Get<Trait>(id).Icon.SetAlt(true);

                for (uint32_t const id : info.MajorTraits)
                    if (Reserve<Trait>(id, ids, result))
                        Get<Trait>(id).Icon.SetAlt(false);

                if (uint32_t const id = info.WeaponTrait)
                    if (Reserve<Trait>(id, ids, result))
                        Get<Trait>(id).Icon.SetAlt(true);

                Request<Trait>(ids);
            }
        }
    }

    return result;
}

bool API::PreloadAllGearInfos(ChatLink::ArcDPSLegendaryTemplate<uint32_t> const& gear)
{
    bool result = true;
    std::set<uint32_t> items;
    std::set<uint32_t> stats;
    for (auto const& item : gear.Items)
    {
        if (uint32_t const id = item.Item)
            Reserve<Item>(id, items, result);
        if (uint32_t const id = item.Stats)
            Reserve<ItemStats>(id, stats, result);
        for (uint32_t const id : item.Upgrades)
            if (id)
                Reserve<Item>(id, items, result);
        for (uint32_t const id : item.Infusions)
            if (id)
                Reserve<Item>(id, items, result);
    }
    Request<Item>(items);
    Request<ItemStats>(stats);

    return result;
}

void API::InfoHandlers<API::Profession>::Success(std::string_view const data)
{
    for (auto const& profession : nlohmann::json::parse(data.begin(), data.end(), nullptr, false))
    {
        GW2::Profession id;
        if (auto const itr = util::find_if(GW2::GetProfessionInfos(), util::member_equals(&GW2::ProfessionInfo::Name, profession["id"])))
            id = itr->Profession;
        else
            return;

        auto& info = Profession::Get((uint32_t)id);
        info.Name = profession["name"];
        for (auto const& specialization : profession["specializations"])
            info.Specializations.emplace_back((GW2::Specialization)specialization);
        for (auto const& skill : profession["skills"])
            info.Skills.emplace_back(skill["id"]);
        info.Icon.Set(Handler::Instance().GetIcon(id), false);
        info.Loaded = true;
    }
}
void API::InfoHandlers<API::Profession>::Error(std::set<uint32_t> const& ids) { }

void API::InfoHandlers<API::Specialization>::Success(std::string_view const data)
{
    for (auto const& specialization : nlohmann::json::parse(data.begin(), data.end(), nullptr, false))
    {
        uint32_t const id = specialization["id"];
        auto& info = Specialization::Get(id);
        info.Name = specialization["name"];
        for (size_t i = 0; i < 3; ++i)
            info.MinorTraits[i] = specialization["minor_traits"][i];
        for (size_t i = 0; i < 9; ++i)
            info.MajorTraits[i] = specialization["major_traits"][i];
        if (specialization.contains("weapon_trait"))
            info.WeaponTrait = specialization["weapon_trait"];
        info.IconURL = specialization["icon"];
        info.Loaded = true;
    }
}
void API::InfoHandlers<API::Specialization>::Error(std::set<uint32_t> const& ids) { }

void API::InfoHandlers<API::Trait>::Success(std::string_view data)
{
    for (auto const& trait : nlohmann::json::parse(data.begin(), data.end(), nullptr, false))
    {
        uint32_t const id = trait["id"];
        auto& info = Trait::Get(id);
        info.Name = trait["name"];
        info.Major = trait["slot"] == "Major";
        info.Icon.SetAlt(!info.Major);
        info.Icon.Download(trait["icon"]);
        info.Loaded = true;
    }
}
void API::InfoHandlers<API::Trait>::Error(std::set<uint32_t> const& ids)
{
    for (uint32_t const id : ids)
        Trait::Get(id).Icon.SetError();
}

void API::InfoHandlers<API::Skill>::Success(std::string_view const data)
{
    for (auto const& skill : nlohmann::json::parse(data.begin(), data.end(), nullptr, false))
    {
        uint32_t const id = skill["id"];
        auto& info = Skill::Get(id);
        info.Name = skill["name"];
        for (auto const& flag : skill["flags"])
        {
            if (flag == "NoUnderwater")
                info.NoUnderwater = true;
        }
        info.Icon.Download(skill["icon"], [](TextureData texture) { return texture.Trim(texture.Width / 10); });
        info.Loaded = true;
    }
}
void API::InfoHandlers<API::Skill>::Error(std::set<uint32_t> const& ids)
{
    for (uint32_t const id : ids)
        Skill::Get(id).Icon.SetError();
}

void API::InfoHandlers<API::Item>::Success(std::string_view const data)
{
    for (auto const& item : nlohmann::json::parse(data.begin(), data.end(), nullptr, false))
    {
        uint32_t const id = item["id"];
        auto& info = Item::Get(id);
        info.Name = item["name"];
        if (auto const itr = util::find_if(GW2::GetRarityInfos(), util::member_equals(&GW2::RarityInfo::Name, item["rarity"])))
            info.Rarity = itr->Rarity;
        info.Icon.Download(item["icon"]);
        info.Loaded = true;
    }
}
void API::InfoHandlers<API::Item>::Error(std::set<uint32_t> const& ids)
{
    for (uint32_t const id : ids)
        Item::Get(id).Icon.SetError();
}

void API::InfoHandlers<API::ItemStats>::Success(std::string_view const data)
{
    for (auto const& stat : nlohmann::json::parse(data.begin(), data.end(), nullptr, false))
    {
        uint32_t const id = stat["id"];
        auto& info = ItemStats::Get(id);
        info.Name = stat["name"];
        info.Loaded = true;
    }
}
void API::InfoHandlers<API::ItemStats>::Error(std::set<uint32_t> const& ids) { }

void API::InfoHandlers<API::Pet>::Success(std::string_view const data)
{
    for (auto const& pet : nlohmann::json::parse(data.begin(), data.end(), nullptr, false))
    {
        uint32_t const id = pet["id"];
        auto& info = Pet::Get(id);
        info.Name = pet["name"];
        info.Icon.Download(pet["icon"], [](TextureData texture) { return texture.Trim(52); });
        info.Loaded = true;
    }
}
void API::InfoHandlers<API::Pet>::Error(std::set<uint32_t> const& ids)
{
    for (uint32_t const id : ids)
        Pet::Get(id).Icon.SetError();
}
}
