#include "API.h"
#include "json/json.hpp"

namespace buildpad
{
#define H Handler::Instance()  // NOLINT(cppcoreguidelines-macro-usage)
using Icons = Handler::Icons;

void API::PreloadAllPets()
{
    bool result = true;
    std::set<uint32_t> ids;
    for (uint32_t const id : H.GetPets())
        if (id)
            Reserve<Pet>(id, ids, result);

    Request<Pet>(ids);
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
                if (uint32_t const id = H.SkillPaletteToSkill(palette, parsed.RevenantLegendsLand[0]))
                    Reserve<Skill>(id, ids, result);

        if (parsed.SkillsWater)
            for (uint32_t const palette : *parsed.SkillsWater)
                if (uint32_t const id = H.SkillPaletteToSkill(palette, parsed.RevenantLegendsWater[0]))
                    Reserve<Skill>(id, ids, result);

        if (parsed.Profession == GW2::Profession::Revenant)
        {
            for (auto const& info : GW2::GetRevenantLegendInfos())
                if (uint32_t const id = info.SwapSkill)
                    Reserve<Skill>(id, ids, result);

            for (auto const& legend : parsed.RevenantLegendsLand)
            {
                if (legend != GW2::RevenantLegend::None)
                {
                    auto const& info = GW2::GetRevenantLegendInfo(legend);
                    for (uint32_t const id : info.Skills)
                        Reserve<Skill>(id, ids, result);
                }
            }
            for (auto const& legend : parsed.RevenantLegendsWater)
            {
                if (legend != GW2::RevenantLegend::None)
                {
                    auto const& info = GW2::GetRevenantLegendInfo(legend);
                    for (uint32_t const id : info.Skills)
                        Reserve<Skill>(id, ids, result);
                }
            }
        }

        Request<Skill>(ids);
    }

    if (parsed.Profession == GW2::Profession::Ranger)
    {
        std::set<uint32_t> ids;
        for (uint32_t const id : parsed.RangerPetsLand)
            if (Reserve<Pet>(id, ids, result))
                Get<Pet>(id).Icon = H.GetIcon(Icons::LoadingPet);

        for (uint32_t const id : parsed.RangerPetsWater)
            if (Reserve<Pet>(id, ids, result))
                Get<Pet>(id).Icon = H.GetIcon(Icons::LoadingPet);

        Request<Pet>(ids);
    }

    if (parsed.TraitLines)
    {
        for (auto const& line : *parsed.TraitLines)
        {
            if (auto& info = Specialization::Get((uint32_t)line.Specialization))
            {
                if (!info.Icon)
                {
                    info.Icon = H.GetIcon(Icons::LoadingSpecialization);
                    Web::Instance().Request(info.IconURL,
                        [&info](std::string_view const data) { info.Icon = H.LoadTexture(std::pair { data.data(), data.size() }); },
                        [&info](auto const&) { info.Icon = H.GetIcon(Icons::ErrorSpecialization); });
                }

                std::set<uint32_t> ids;
                for (uint32_t const id : info.MinorTraits)
                    if (Reserve<Trait>(id, ids, result))
                        Get<Trait>(id).Icon = H.GetIcon(Icons::LoadingTraitMinor);

                for (uint32_t const id : info.MajorTraits)
                    if (Reserve<Trait>(id, ids, result))
                        Get<Trait>(id).Icon = H.GetIcon(Icons::LoadingTrait);

                if (uint32_t const id = info.WeaponTrait)
                    if (Reserve<Trait>(id, ids, result))
                        Get<Trait>(id).Icon = H.GetIcon(Icons::LoadingTraitMinor);

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
        info.Loaded = true;

        Web::Instance().Request(trait["icon"],
            [&info](std::string_view const data) { info.Icon = H.LoadTexture(std::pair { data.data(), data.size() }); },
            [&info](auto const&) { info.Icon = H.GetIcon(info.Major ? Icons::ErrorTrait : Icons::ErrorTraitMinor); });
    }
}
void API::InfoHandlers<API::Trait>::Error(std::set<uint32_t> const& ids)
{
    for (uint32_t const id : ids)
        if (auto& info = Trait::Get(id); info.Icon.Texture == H.GetIcon(Icons::LoadingTrait).Texture || info.Icon.Texture == H.GetIcon(Icons::LoadingTraitMinor).Texture)
            info.Icon = H.GetIcon(info.Icon.Texture == H.GetIcon(Icons::LoadingTrait).Texture ? Icons::ErrorTrait : Icons::ErrorTraitMinor);
}

void API::InfoHandlers<API::Skill>::Success(std::string_view const data)
{
    for (auto const& skill : nlohmann::json::parse(data.begin(), data.end(), nullptr, false))
    {
        uint32_t const id = skill["id"];
        auto& info = Skill::Get(id);
        info.Name = skill["name"];
        info.Loaded = true;

        Web::Instance().Request(skill["icon"], [&info](std::string_view const data)
        {
            if (auto texture = H.LoadTexture(std::pair { data.data(), data.size() }))
                info.Icon = texture->Trim(texture->Width / 10);
        }, [&info](auto const&) { info.Icon = H.GetIcon(Icons::ErrorSkill); });
    }
}
void API::InfoHandlers<API::Skill>::Error(std::set<uint32_t> const& ids)
{
    for (uint32_t const id : ids)
        if (auto& info = Skill::Get(id); info.Icon.Texture == H.GetIcon(Icons::LoadingSkill).Texture)
            info.Icon = H.GetIcon(Icons::ErrorSkill);
}

void API::InfoHandlers<API::Item>::Success(std::string_view const data)
{
    for (auto const& item : nlohmann::json::parse(data.begin(), data.end(), nullptr, false))
    {
        uint32_t const id = item["id"];
        auto& info = Item::Get(id);
        info.Name = item["name"];
        if (auto itr = std::find_if(GW2::GetRarityInfos().begin(), GW2::GetRarityInfos().end(),
            [rarity = (std::string_view)item["rarity"]](GW2::RarityInfo const& info) { return info.Name == rarity; }); itr != GW2::GetRarityInfos().end())
            info.Rarity = itr->Rarity;
        info.Loaded = true;

        Web::Instance().Request(item["icon"].get<std::string>(),
            [&info](std::string_view const data) { info.Icon = H.LoadTexture(std::pair { data.data(), data.size() }); },
            [&info](auto const&) { info.Icon = H.GetIcon(Icons::ErrorItem); });
    }
}
void API::InfoHandlers<API::Item>::Error(std::set<uint32_t> const& ids)
{
    for (uint32_t const id : ids)
        if (auto& info = Item::Get(id); info.Icon.Texture == H.GetIcon(Icons::LoadingItem).Texture)
            info.Icon = H.GetIcon(Icons::ErrorItem);
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
        info.Loaded = true;

        Web::Instance().Request(pet["icon"], [&info](std::string_view const data)
        {
            if (auto texture = H.LoadTexture(std::pair { data.data(), data.size() }))
                info.Icon = texture->Trim(52);
        }, [&info](auto const&) { info.Icon = H.GetIcon(Icons::ErrorPet); });
    }
}
void API::InfoHandlers<API::Pet>::Error(std::set<uint32_t> const& ids)
{
    for (uint32_t const id : ids)
        if (auto& info = Pet::Get(id); info.Icon.Texture == H.GetIcon(Icons::LoadingPet).Texture)
            info.Icon = H.GetIcon(Icons::ErrorPet);
}
}
