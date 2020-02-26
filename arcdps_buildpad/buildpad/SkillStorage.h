#pragma once
#include "GW2.h"

namespace buildpad
{
class SkillStorage : public Singleton<SkillStorage>
{
public:
    enum class SkillType : uint8_t
    {
        None,
        Profession,
        Heal,
        Utility,
        Elite,
    };
    struct Skill
    {
        uint32_t ID = 0;
        SkillType Type = SkillType::None;
        GW2::Profession Profession = GW2::Profession::None;
        GW2::Specialization Specialization = GW2::Specialization::None;
        std::vector<uint32_t> Palettes;

        [[nodiscard]] uint32_t ToPalette() const { return !Palettes.empty() ? Palettes.front() : 0; }
    };
    struct RevenantLegendSkills
    {
        uint32_t SwapSkill = 0;
        std::array<uint32_t, 5> Skills { };
    };
    enum class LoadingState
    {
        None,
        Loading,
        Done,
        Failed,
    };

    [[nodiscard]] Skill* GetSkill(uint32_t const skill)
    {
        auto itr = m_skills.find(skill);
        return itr != m_skills.end() ? &itr->second : nullptr;
    }
    [[nodiscard]] Skill *GetSkillByPalette(uint32_t const palette, GW2::RevenantLegend const legend)
    {
        if (uint32_t const skill = FromPalette(palette, legend))
            return GetSkill(skill);
        return nullptr;
    }

    void AddPalette(uint32_t const palette, uint32_t const skill, bool overrideAPI = false)
    {
        if (overrideAPI)
        {
            m_skills[skill].Palettes.clear();
            m_palettes[palette].clear();
        }

        m_skills[skill].Palettes.push_back(palette);
        m_palettes[palette].push_back(skill);
    }
    [[nodiscard]] uint32_t FromPalette(uint32_t const palette, GW2::RevenantLegend const legend) const
    {
        if (auto const index = GW2::RevenantLegendInfo::GetSkillIndex(palette))
        {
            if (auto const* skills = GetRevenantLegendSkills(legend))
                return skills->Skills[*index];

            return 0;
        }

        auto const itr = m_palettes.find(palette);
        return itr != m_palettes.end() ? itr->second.front() : 0;
    }
    [[nodiscard]] uint32_t ToPalette(uint32_t const skill) const
    {
        auto const itr = m_skills.find(skill);
        return itr != m_skills.end() ? itr->second.ToPalette() : 0;
    }
    [[nodiscard]] GW2::Profession GetPaletteProfession(uint32_t const palette, GW2::RevenantLegend const legend) const
    {
        auto const itr = m_skills.find(FromPalette(palette, legend));
        return itr != m_skills.end() ? itr->second.Profession : GW2::Profession::None;
    }
    [[nodiscard]] GW2::Specialization GetPaletteSpecialization(uint32_t const palette, GW2::RevenantLegend const legend) const
    {
        auto const itr = m_skills.find(FromPalette(palette, legend));
        return itr != m_skills.end() ? itr->second.Specialization : GW2::Specialization::None;
    }

    void AddRevenantLegendSkills(GW2::RevenantLegend const legend, RevenantLegendSkills const& skills)
    {
        m_revenantLegendSkills[legend] = skills;
    }
    [[nodiscard]] RevenantLegendSkills const* GetRevenantLegendSkills(GW2::RevenantLegend const legend) const
    {
        auto const itr = m_revenantLegendSkills.find(legend);
        return itr != m_revenantLegendSkills.end() ? &itr->second : nullptr;
    }
    [[nodiscard]] uint32_t GetRevenantLegendSwapSkill(GW2::RevenantLegend const legend) const
    {
        if (auto const* skills = GetRevenantLegendSkills(legend))
            return skills->SwapSkill;

        return 0;
    }

    void AddProfessionSkill(GW2::Profession const profession, uint32_t const id, SkillType const type)
    {
        Skill& skill = *m_professionSkills[(size_t)profession].emplace_back(&m_skills[id]);
        skill.ID = id;
        skill.Type = type;
        skill.Profession = profession;
    }
    void ClearProfessionSkills()
    {
        for (auto& profession : m_professionSkills)
            profession.clear();
    }
    [[nodiscard]] std::vector<Skill*> const& GetProfessionSkills(GW2::Profession const profession) const
    {
        return m_professionSkills[(size_t)profession];
    }

    void SetLoadingState(LoadingState const state) { m_state = state; }
    [[nodiscard]] LoadingState GetLoadingState() const { return m_state; }
    [[nodiscard]] bool AreSkillsLoaded() const { return GetLoadingState() == LoadingState::Done; }

private:
    std::unordered_map<uint32_t, Skill> m_skills;
    std::unordered_map<uint32_t, std::vector<uint32_t>> m_palettes;
    std::unordered_map<GW2::RevenantLegend, RevenantLegendSkills> m_revenantLegendSkills;
    std::array<std::vector<Skill*>, 10> m_professionSkills;
    LoadingState m_state = LoadingState::None;
};
}
