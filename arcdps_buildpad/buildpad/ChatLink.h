#pragma once
#include "Defines.h"
#include "Build.h"

namespace buildpad
{
class ChatLink
{
public:
    static constexpr size_t AG_NAME_LEN = 64;
#pragma pack(push, 1)
    struct Item
    {
        struct Payload
        {
            uint32_t ID : 24;
            uint32_t Terminator : 8;

            explicit Payload(uint32_t id) : ID(id), Terminator(0) { }
        };
        uint8_t Type = 0x02;
        uint8_t Count;
        uint32_t ID : 24;
        uint32_t Flags : 8;

        explicit Item(uint32_t id, uint8_t count, uint8_t flags = 0) : Count(count), ID(id), Flags(flags) { }
    };
    struct Skill
    {
        uint8_t Type = 0x06;
        uint32_t ID : 24;
        uint32_t Terminator : 8;

        explicit Skill(uint32_t id) : ID(id), Terminator(0) { }
    };
    struct Trait
    {
        uint8_t Type = 0x07;
        uint32_t ID : 24;
        uint32_t Terminator : 8;

        explicit Trait(uint32_t id) : ID(id), Terminator(0) { }
    };
    template<template<typename> typename T>
    struct ArcDPSFile
    {
        uint64_t Created;
        T<uint32_t> Data;
        std::array<char, AG_NAME_LEN> Name;
    };
    template<template<typename> typename T>
    struct ArcDPSGearTemplateFile
    {
        uint64_t Created;
        T<uint32_t> Data;
    };
    template<template<typename> typename T>
    struct ArcDPSCode
    {
        char Type;
        T<uint16_t> Data;
    };
    template<typename T>
    struct ArcDPSSkillTemplate
    {
        T Profession;
        std::array<T, 5> Land;
        std::array<T, 5> Water;
    };
    template<typename T>
    struct ArcDPSTraitTemplate
    {
        T Profession;
        std::array<T, 3> Specializations;
        std::array<T, 9> Traits;
    };
    template<typename T>
    struct ArcDPSGearTemplateItem
    {
        T Item;
        T Stats;
        std::array<T, 2> Upgrades;
        std::array<T, 4> Infusions;
        T Slot;
    };
    template<typename T>
    struct ArcDPSLegendaryTemplate
    {
        T Profession;
        std::array<ArcDPSGearTemplateItem<T>, 19> Items;
        std::array<char, AG_NAME_LEN> Name;
    };
    template<typename T>
    struct ArcDPSGearTemplate : ArcDPSLegendaryTemplate<T>
    {
        std::array<char, AG_NAME_LEN> Character;
        std::array<uint8_t, 4> SlotHidden;

        [[nodiscard]] std::optional<bool> GetSlotVisibility(GW2::Slot slot) const
        {
            switch (slot)
            {
                case GW2::Slot::Helm:       return !SlotHidden[0];
                case GW2::Slot::Shoulders:  return !SlotHidden[1];
                case GW2::Slot::Gloves:     return !SlotHidden[2];
                case GW2::Slot::Back:       return !SlotHidden[3];
                default: return { };
            }
        }
    };
    struct BuildTemplate
    {
        struct SpecializationData
        {
            GW2::Specialization Specialization = GW2::Specialization::None;
            uint8_t Trait0 : 2;
            uint8_t Trait1 : 2;
            uint8_t Trait2 : 2;
            uint8_t Padding : 2;
        };
        template<typename T>
        struct LandWaterData
        {
            T Land { };
            T Water { };
        };

        uint8_t Type = 0x0D;
        GW2::Profession Profession = GW2::Profession::None;
        std::array<SpecializationData, 3> Specializations { };
        std::array<LandWaterData<uint16_t>, 5> Skills { };
        union
        {
            std::array<uint8_t, 16> Raw { };
            LandWaterData<std::array<uint8_t, 2>> RangerPets;
            struct
            {
                LandWaterData<std::array<GW2::RevenantLegend, 2>> Legends;
                LandWaterData<std::array<uint16_t, 3>> InactiveSkills { };
            } Revenant;
        } ProfessionSpecific { };

        BuildTemplate() = default;
        BuildTemplate(ArcDPSCode<ArcDPSSkillTemplate> const& skills, ArcDPSCode<ArcDPSTraitTemplate> const& traits) : Profession((GW2::Profession)skills.Data.Profession)
        {
            assert(traits.Data.Profession == skills.Data.Profession);
            for (uint8_t i = 0; i < 3; ++i)
            {
                auto const& info = GW2::GetSpecializationInfo((GW2::Specialization)traits.Data.Specializations[i]);
                Specializations[i].Specialization = info.Specialization;
                Specializations[i].Trait0 = info.GetTraitIndex(0, (uint16_t)traits.Data.Traits[i * 3 + 0]);
                Specializations[i].Trait1 = info.GetTraitIndex(1, (uint16_t)traits.Data.Traits[i * 3 + 1]);
                Specializations[i].Trait2 = info.GetTraitIndex(2, (uint16_t)traits.Data.Traits[i * 3 + 2]);
            }
            for (uint8_t i = 0; i < 5; ++i)
            {
                Skills[i].Land = skills.Data.Land[i];
                Skills[i].Water = skills.Data.Water[i];
            }
            switch (Profession)
            {
                case GW2::Profession::Ranger:
                    ProfessionSpecific.RangerPets = { };
                    break;
                case GW2::Profession::Revenant:
                    ProfessionSpecific.Revenant = { };
                    break;
                default:
                    ProfessionSpecific.Raw = { };
                    break;
            }
        }
    };
#pragma pack(pop)
    using link_t = std::variant<BuildTemplate, ArcDPSCode<ArcDPSSkillTemplate>, ArcDPSCode<ArcDPSTraitTemplate>>;

    static std::string Encode(unsigned char const* data, size_t length);
    static std::string Encode(link_t const& link);
    static std::optional<link_t> Decode(std::string_view code);

};
}
