#include "Build.h"
#include "ChatLink.h"
#include "Handler.h"
#include "SkillStorage.h"

namespace buildpad
{
void Build::PostLoad(uint32_t const buildVersion)
{
    switch (GetParsedProfession())
    {
        case GW2::Profession::Revenant:
            if (auto link = ChatLink::Decode(GetLink()); link && std::holds_alternative<ChatLink::BuildTemplate>(*link))
            {
                auto& data = std::get<ChatLink::BuildTemplate>(*link);
                for (uint8_t water = 0; water < 2; ++water)
                    for (uint8_t i = 0; i < 3; ++i)
                        if (!data.ProfessionSpecific.Revenant.InactiveSkills.Select((bool)water)[i])
                            data.ProfessionSpecific.Revenant.InactiveSkills.Select((bool)water)[i] = data.Skills[1 + i].Select((bool)water);

                // Fix reordered legend IDs after 2020-08-25 GW2 patch
                switch (buildVersion)
                {
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    {
                        bool const patch20200609 = buildVersion == 4
                            || util::find_if(data.ProfessionSpecific.Revenant.Legends.Land, util::equals((GW2::RevenantLegend)19))
                            || util::find_if(data.ProfessionSpecific.Revenant.Legends.Water, util::equals((GW2::RevenantLegend)19));
                        for (auto& legend : data.ProfessionSpecific.Revenant.Legends.Land)
                            if (legend != GW2::RevenantLegend::None)
                                legend = (GW2::RevenantLegend)((std::underlying_type_t<GW2::RevenantLegend>)legend - (patch20200609 ? 13 : 12));
                        for (auto& legend : data.ProfessionSpecific.Revenant.Legends.Water)
                            if (legend != GW2::RevenantLegend::None)
                                legend = (GW2::RevenantLegend)((std::underlying_type_t<GW2::RevenantLegend>)legend - (patch20200609 ? 13 : 12));
                        break;
                    }
                    default:
                        break;
                }

                SetLink(ChatLink::Encode(data));
            }
            break;
        default:
            break;
    }
    SetSaved();
}

Build::ParsedInfo Build::ParseInfo(std::string_view code, std::optional<uint32_t> const buildVersion)
{
    ParsedInfo parsed { };
    if (auto link = ChatLink::Decode(code))
    {
        std::visit(overloaded
        {
            [&parsed, &buildVersion](ChatLink::BuildTemplate& link)
            {
                parsed.Profession = link.Profession;
                auto& land = parsed.SkillsLand.emplace();
                auto& water = parsed.SkillsWater.emplace();
                for (uint8_t i = 0; i < 5; ++i)
                {
                    land[i] = link.Skills[i].Land;
                    water[i] = link.Skills[i].Water;
                }
                auto& traits = parsed.TraitLines.emplace();
                for (uint8_t i = 0; i < 3; ++i)
                {
                    traits[i].Specialization = link.Specializations[i].Specialization;
                    traits[i].Traits[0] = link.Specializations[i].Trait0;
                    traits[i].Traits[1] = link.Specializations[i].Trait1;
                    traits[i].Traits[2] = link.Specializations[i].Trait2;
                }
                parsed.Specialization = traits[2].Specialization != GW2::Specialization::None
                                      ? traits[2].Specialization
                                      : traits[1].Specialization != GW2::Specialization::None
                                      ? traits[1].Specialization
                                      : traits[0].Specialization;
                if (auto const error = ValidateParsedInfo(parsed))
                {
                    parsed.Error = *error;
                    return;
                }
                if (auto itr = util::find_if(land, [&parsed](uint32_t palette) { return GW2::GetSpecializationInfo(SkillStorage::Instance().GetPaletteSpecialization(palette, parsed.RevenantLegendsLand[0])).Elite; }))
                    parsed.Specialization = SkillStorage::Instance().GetPaletteSpecialization(*itr, parsed.RevenantLegendsLand[0]);
                else if (itr = util::find_if(water, [&parsed](uint32_t palette) { return GW2::GetSpecializationInfo(SkillStorage::Instance().GetPaletteSpecialization(palette, parsed.RevenantLegendsWater[0])).Elite; }))
                    parsed.Specialization = SkillStorage::Instance().GetPaletteSpecialization(*itr, parsed.RevenantLegendsWater[0]);
                switch (parsed.Profession)
                {
                    case GW2::Profession::Ranger:
                        std::copy(link.ProfessionSpecific.RangerPets.Land.begin(), link.ProfessionSpecific.RangerPets.Land.end(), parsed.RangerPetsLand.begin());
                        std::copy(link.ProfessionSpecific.RangerPets.Water.begin(), link.ProfessionSpecific.RangerPets.Water.end(), parsed.RangerPetsWater.begin());
                        break;
                    case GW2::Profession::Revenant:
                        // Fix reordered legend IDs after 2020-08-25 GW2 patch
                        switch (buildVersion.value_or(0))
                        {
                            case 1:
                            case 2:
                            case 3:
                            case 4:
                            {
                                bool const patch20200609 = buildVersion == 4
                                    || util::find_if(link.ProfessionSpecific.Revenant.Legends.Land, util::equals((GW2::RevenantLegend)19))
                                    || util::find_if(link.ProfessionSpecific.Revenant.Legends.Water, util::equals((GW2::RevenantLegend)19));
                                for (auto& legend : link.ProfessionSpecific.Revenant.Legends.Land)
                                    if (legend != GW2::RevenantLegend::None)
                                        legend = (GW2::RevenantLegend)((std::underlying_type_t<GW2::RevenantLegend>)legend - (patch20200609 ? 13 : 12));
                                for (auto& legend : link.ProfessionSpecific.Revenant.Legends.Water)
                                    if (legend != GW2::RevenantLegend::None)
                                        legend = (GW2::RevenantLegend)((std::underlying_type_t<GW2::RevenantLegend>)legend - (patch20200609 ? 13 : 12));
                                break;
                            }
                            default:
                                break;
                        }
                        // Just reset incorrect legends at this point to prevent a crash
                        for (auto& legend : link.ProfessionSpecific.Revenant.Legends.Land)
                            if (legend >= (GW2::RevenantLegend)12)
                                legend = GW2::RevenantLegend::None;
                        for (auto& legend : link.ProfessionSpecific.Revenant.Legends.Water)
                            if (legend >= (GW2::RevenantLegend)12)
                                legend = GW2::RevenantLegend::None;

                        std::copy(link.ProfessionSpecific.Revenant.Legends.Land.begin(), link.ProfessionSpecific.Revenant.Legends.Land.end(), parsed.RevenantLegendsLand.begin());
                        std::copy(link.ProfessionSpecific.Revenant.Legends.Water.begin(), link.ProfessionSpecific.Revenant.Legends.Water.end(), parsed.RevenantLegendsWater.begin());
                        for (uint8_t i = 0; i < 3; ++i)
                        {
                            parsed.RevenantInactiveSkillsLand[i] = link.ProfessionSpecific.Revenant.InactiveSkills.Land[i];
                            parsed.RevenantInactiveSkillsWater[i] = link.ProfessionSpecific.Revenant.InactiveSkills.Water[i];
                        }
                        if (auto itr = util::find_if(parsed.RevenantLegendsLand, [](GW2::RevenantLegend legend) { return GW2::GetRevenantLegendInfo(legend).RequiredSpecialization != GW2::Specialization::None; }))
                            parsed.Specialization = GW2::GetRevenantLegendInfo(*itr).RequiredSpecialization;
                        else if (itr = util::find_if(parsed.RevenantLegendsWater, [](GW2::RevenantLegend legend) { return GW2::GetRevenantLegendInfo(legend).RequiredSpecialization != GW2::Specialization::None; }))
                            parsed.Specialization = GW2::GetRevenantLegendInfo(*itr).RequiredSpecialization;
                        break;
                    default:
                        break;
                }
            },
            [&parsed](ChatLink::ArcDPSCode<ChatLink::ArcDPSSkillTemplate>& link)
            {
                parsed.Profession = (GW2::Profession)link.Data.Profession;
                auto& land = parsed.SkillsLand.emplace();
                std::copy(link.Data.Land.begin(), link.Data.Land.end(), land.begin());
                auto& water = parsed.SkillsWater.emplace();
                std::copy(link.Data.Water.begin(), link.Data.Water.end(), water.begin());
                if (auto const error = ValidateParsedInfo(parsed))
                {
                    parsed.Error = *error;
                    return;
                }
                if (auto itr = util::find_if(land, [&parsed](uint32_t palette) { return GW2::GetSpecializationInfo(SkillStorage::Instance().GetPaletteSpecialization(palette, parsed.RevenantLegendsLand[0])).Elite; }))
                    parsed.Specialization = SkillStorage::Instance().GetPaletteSpecialization(*itr, parsed.RevenantLegendsLand[0]);
                else if (itr = util::find_if(water, [&parsed](uint32_t palette) { return GW2::GetSpecializationInfo(SkillStorage::Instance().GetPaletteSpecialization(palette, parsed.RevenantLegendsWater[0])).Elite; }))
                    parsed.Specialization = SkillStorage::Instance().GetPaletteSpecialization(*itr, parsed.RevenantLegendsWater[0]);
                else
                    parsed.Specialization = GW2::GetProfessionInfo(parsed.Profession).Specializations.front();
                parsed.NeedsSecondaryLink = true;
                parsed.SecondaryLinkIsTraits = true;
            },
            [&parsed](ChatLink::ArcDPSCode<ChatLink::ArcDPSTraitTemplate>& link)
            {
                parsed.Profession = (GW2::Profession)link.Data.Profession;
                auto& traits = parsed.TraitLines.emplace();
                for (uint8_t i = 0; i < 3; ++i)
                {
                    auto const& info = GW2::GetSpecializationInfo((GW2::Specialization)link.Data.Specializations[i]);
                    traits[i].Specialization = info.Specialization;
                    traits[i].Traits[0] = info.GetTraitIndex(0, (uint16_t)link.Data.Traits[i * 3 + 0]);
                    traits[i].Traits[1] = info.GetTraitIndex(1, (uint16_t)link.Data.Traits[i * 3 + 1]);
                    traits[i].Traits[2] = info.GetTraitIndex(2, (uint16_t)link.Data.Traits[i * 3 + 2]);
                }
                parsed.Specialization = traits[2].Specialization != GW2::Specialization::None
                                      ? traits[2].Specialization
                                      : traits[1].Specialization != GW2::Specialization::None
                                      ? traits[1].Specialization
                                      : traits[0].Specialization;
                if (auto const error = ValidateParsedInfo(parsed))
                {
                    parsed.Error = *error;
                    return;
                }
                parsed.NeedsSecondaryLink = true;
                parsed.SecondaryLinkIsTraits = false;
            },
        }, *link);
    }
    else if (!code.empty())
        parsed.Error = "Unrecognized code";

    return parsed;
}

std::optional<std::string> Build::ValidateParsedInfo(ParsedInfo& parsed)
{
    std::vector<std::string> errors;

    if (!util::find_if(GW2::GetProfessionInfos(), util::member_equals(&GW2::ProfessionInfo::Profession, parsed.Profession)))
    {
        parsed.Profession = GW2::Profession::None;
        errors.emplace_back("Unknown profession");
    }

    if (auto const itr = util::find_if(GW2::GetSpecializationInfos(), util::member_equals(&GW2::SpecializationInfo::Specialization, parsed.Specialization)))
    {
        if (itr->Specialization != GW2::Specialization::None && itr->Profession != parsed.Profession)
        {
            parsed.Specialization = GW2::Specialization::None;
            errors.emplace_back("Specialization doesn't match profession");
        }
    }
    else
    {
        parsed.Specialization = GW2::Specialization::None;
        errors.emplace_back("Unknown specialization");
    }

    if (parsed.TraitLines)
    {
        for (auto& line : *parsed.TraitLines)
        {
            if (auto const itr = util::find_if(GW2::GetSpecializationInfos(), util::member_equals(&GW2::SpecializationInfo::Specialization, line.Specialization)))
            {
                if (itr->Specialization != GW2::Specialization::None && itr->Profession != parsed.Profession)
                {
                    line.Specialization = GW2::Specialization::None;
                    errors.emplace_back("Specialization doesn't match profession");
                }
            }
            else
            {
                line.Specialization = GW2::Specialization::None;
                errors.emplace_back("Unknown specialization");
            }

            for (auto& trait : line.Traits)
            {
                if (trait > 3)
                {
                    trait = 0;
                    errors.emplace_back("Malformed trait selection");
                }
            }
        }
    }

    for (auto* legends : { &parsed.RevenantLegendsLand, &parsed.RevenantLegendsWater })
    {
        for (auto& legend : *legends)
        {
            if (!util::find_if(GW2::GetRevenantLegendInfos(), util::member_equals(&GW2::RevenantLegendInfo::Legend, legend)))
            {
                legend = GW2::RevenantLegend::None;
                errors.emplace_back("Unknown revenant legend");
            }
        }
    }

    /* Disabled, as this requires manual maintenance and might break saved builds until its updated
    if (Handler::Instance().ArePetsLoaded())
    {
        for (auto* pets : { &parsed.RangerPetsLand, &parsed.RangerPetsWater })
        {
            for (auto& pet : *pets)
            {
                if (pet && !util::find_if(Handler::Instance().GetPets(), util::equals(pet)))
                {
                    pet = 0;
                    errors.emplace_back("Unknown pet");
                }
            }
        }
    }
    */

    /* Disabled, as this requires manual maintenance and might break saved builds until its updated
    if (SkillStorage::Instance().AreSkillsLoaded())
    {
        bool water = false;
        for (auto* skills : { &parsed.SkillsLand, &parsed.SkillsWater })
        {
            if (*skills)
            {
                for (auto& palette : **skills)
                {
                    if (!palette)
                        continue;

                    GW2::RevenantLegend const legend = water ? parsed.RevenantLegendsLand[0] : parsed.RevenantLegendsWater[0];
                    if (!SkillStorage::Instance().FromPalette(palette, legend))
                    {
                        palette = 0;
                        errors.emplace_back("Unknown skill");
                    }

                    GW2::Profession const profession = SkillStorage::Instance().GetPaletteProfession(palette, legend);
                    if (profession != GW2::Profession::None && profession != parsed.Profession)
                    {
                        palette = 0;
                        errors.emplace_back("Skill from a wrong profession");
                    }

                    GW2::Specialization const specialization = SkillStorage::Instance().GetPaletteSpecialization(palette, legend);
                    if (specialization != GW2::Specialization::None && specialization != parsed.Specialization)
                    {
                        palette = 0;
                        errors.emplace_back("Skill from a wrong specialization");
                    }
                }
            }
            water = true;
        }
    }
    */

    if (errors.empty())
        return { };

    return fmt::format("{}", fmt::join(errors.begin(), errors.end(), "\n"));
}

void Build::HandleMigration()
{
    if (!m_parsedInfo.NeedsSecondaryLink || m_link.empty() || m_secondaryLink.empty())
        return;

    auto const a = ChatLink::Decode(m_link);
    auto const b = ChatLink::Decode(m_secondaryLink);
    if (!a || !b)
        return;

    ChatLink::BuildTemplate result;

    if (!std::visit(overloaded
    {
        [&](ChatLink::ArcDPSCode<ChatLink::ArcDPSSkillTemplate> const& skills, ChatLink::ArcDPSCode<ChatLink::ArcDPSTraitTemplate> const& traits)
        {
            result = { skills, traits };
            return true;
        },
        [&](ChatLink::ArcDPSCode<ChatLink::ArcDPSTraitTemplate> const& traits, ChatLink::ArcDPSCode<ChatLink::ArcDPSSkillTemplate> const& skills)
        {
            result = { skills, traits };
            return true;
        },
        [](auto&&, auto&&) { return false; }
    }, *a, *b))
        return;

    m_secondaryLink = { };
    SetLink(ChatLink::Encode(result));
    m_parsedInfo.Info = "Converted from arcdps templates";
}

}
