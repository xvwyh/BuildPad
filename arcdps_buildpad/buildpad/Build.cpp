#include "Build.h"
#include "ChatLink.h"
#include "Handler.h"

namespace buildpad
{
Build::ParsedInfo Build::ParseInfo(std::string_view code)
{
    ParsedInfo parsed { };
    if (auto link = ChatLink::Decode(code))
    {
        std::visit(overloaded
        {
            [&parsed](ChatLink::BuildTemplate& link)
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
                if (auto itr = std::find_if(land.begin(), land.end(), [&parsed](uint32_t palette) { return GW2::GetSpecializationInfo(Handler::Instance().GetPaletteSpecialization(palette, parsed.RevenantLegendsLand[0])).Elite; }); itr != land.end())
                    parsed.Specialization = Handler::Instance().GetPaletteSpecialization(*itr, parsed.RevenantLegendsLand[0]);
                else if (itr = std::find_if(water.begin(), water.end(), [&parsed](uint32_t palette) { return GW2::GetSpecializationInfo(Handler::Instance().GetPaletteSpecialization(palette, parsed.RevenantLegendsWater[0])).Elite; }); itr != water.end())
                    parsed.Specialization = Handler::Instance().GetPaletteSpecialization(*itr, parsed.RevenantLegendsWater[0]);
                switch (parsed.Profession)
                {
                    case GW2::Profession::Ranger:
                        std::copy(link.ProfessionSpecific.RangerPets.Land.begin(), link.ProfessionSpecific.RangerPets.Land.end(), parsed.RangerPetsLand.begin());
                        std::copy(link.ProfessionSpecific.RangerPets.Water.begin(), link.ProfessionSpecific.RangerPets.Water.end(), parsed.RangerPetsWater.begin());
                        break;
                    case GW2::Profession::Revenant:
                        std::copy(link.ProfessionSpecific.RevenantLegends.Land.begin(), link.ProfessionSpecific.RevenantLegends.Land.end(), parsed.RevenantLegendsLand.begin());
                        std::copy(link.ProfessionSpecific.RevenantLegends.Water.begin(), link.ProfessionSpecific.RevenantLegends.Water.end(), parsed.RevenantLegendsWater.begin());
                        if (auto itr = std::find_if(parsed.RevenantLegendsLand.begin(), parsed.RevenantLegendsLand.end(), [&parsed](GW2::RevenantLegend legend) { return GW2::GetRevenantLegendInfo(legend).RequiredSpecialization != GW2::Specialization::None; }); itr != parsed.RevenantLegendsLand.end())
                            parsed.Specialization = GW2::GetRevenantLegendInfo(*itr).RequiredSpecialization;
                        else if (itr = std::find_if(parsed.RevenantLegendsWater.begin(), parsed.RevenantLegendsWater.end(), [&parsed](GW2::RevenantLegend legend) { return GW2::GetRevenantLegendInfo(legend).RequiredSpecialization != GW2::Specialization::None; }); itr != parsed.RevenantLegendsWater.end())
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
                if (auto itr = std::find_if(land.begin(), land.end(), [&parsed](uint32_t palette) { return GW2::GetSpecializationInfo(Handler::Instance().GetPaletteSpecialization(palette, parsed.RevenantLegendsLand[0])).Elite; }); itr != land.end())
                    parsed.Specialization = Handler::Instance().GetPaletteSpecialization(*itr, parsed.RevenantLegendsLand[0]);
                else if (itr = std::find_if(water.begin(), water.end(), [&parsed](uint32_t palette) { return GW2::GetSpecializationInfo(Handler::Instance().GetPaletteSpecialization(palette, parsed.RevenantLegendsWater[0])).Elite; }); itr != water.end())
                    parsed.Specialization = Handler::Instance().GetPaletteSpecialization(*itr, parsed.RevenantLegendsWater[0]);
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
                parsed.NeedsSecondaryLink = true;
                parsed.SecondaryLinkIsTraits = false;
            },
        }, *link);
    }
    else if (!code.empty())
        parsed.Error = "Unrecognized code";

    return parsed;
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
        [&](ChatLink::ArcDPSCode<ChatLink::ArcDPSSkillTemplate> const& skills)
        {
            return std::visit(overloaded
            {
                [&](ChatLink::ArcDPSCode<ChatLink::ArcDPSTraitTemplate> const& traits)
                {
                    if (skills.Data.Profession != traits.Data.Profession)
                        return false;

                    result = { skills, traits };
                    return true;
                },
                [](auto&&) { return false; }
            }, *b);
        },
        [&](ChatLink::ArcDPSCode<ChatLink::ArcDPSTraitTemplate> const& traits)
        {
            return std::visit(overloaded
            {
                [&](ChatLink::ArcDPSCode<ChatLink::ArcDPSSkillTemplate> const& skills)
                {
                    if (skills.Data.Profession != traits.Data.Profession)
                        return false;

                    result = { skills, traits };
                    return true;
                },
                [](auto&&) { return false; }
            }, *b);
        },
        [](auto&&) { return false; }
    }, *a))
        return;

    m_secondaryLink = { };
    SetLink(ChatLink::Encode(result));
    m_parsedInfo.Info = "Converted from arcdps templates";
}

}
