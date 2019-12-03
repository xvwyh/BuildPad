#pragma once
#include "Defines.h"
#include "GW2.h"
#include "KeyBind.h"

namespace buildpad
{
class Build
{
public:
    using id_t = uint32_t;
    enum class Flags : uint32_t
    {
        None,
        Favorite    = BITMASK(0),
        PvE         = BITMASK(1),
        PvP         = BITMASK(2),
        WvW         = BITMASK(3),
        OpenWorld   = BITMASK(4),
        Dungeons    = BITMASK(5),
        Fractals    = BITMASK(6),
        Raids       = BITMASK(7),
        Power       = BITMASK(8),
        Condition   = BITMASK(9),
        Tank        = BITMASK(10),
        Support     = BITMASK(11),
        Heal        = BITMASK(12),
    };
    DEFINE_BITMASK_OPERATORS(Flags)
    struct FlagInfo
    {
        Flags Flag;
        std::string Name;
        bool Separator = false;
    };
    static std::vector<FlagInfo> const& GetFlagInfos()
    {
        static std::vector<FlagInfo> const instance
        {
            { Flags::Favorite,  "Favorite",        },
            { Flags::PvE,       "PvE",        true },
            { Flags::PvP,       "PvP",             },
            { Flags::WvW,       "WvW",             },
            { Flags::OpenWorld, "Open World", true },
            { Flags::Dungeons,  "Dungeons",        },
            { Flags::Fractals,  "Fractals",        },
            { Flags::Raids,     "Raids",           },
            { Flags::Power,     "Power",      true },
            { Flags::Condition, "Condition",       },
            { Flags::Tank,      "Tank",            },
            { Flags::Support,   "Support",         },
            { Flags::Heal,      "Heal",            },
        };
        return instance;
    }

    struct ParsedInfo
    {
        GW2::Profession Profession = GW2::Profession::None;
        GW2::Specialization Specialization = GW2::Specialization::None;
        std::optional<std::array<uint32_t, 5>> SkillsLand;
        std::optional<std::array<uint32_t, 5>> SkillsWater;
        struct TraitLine
        {
            GW2::Specialization Specialization = GW2::Specialization::None;
            std::array<uint8_t, 3> Traits { };
        };
        std::optional<std::array<TraitLine, 3>> TraitLines;
        std::array<GW2::RevenantLegend, 2> RevenantLegendsLand { GW2::RevenantLegend::None, GW2::RevenantLegend::None };
        std::array<GW2::RevenantLegend, 2> RevenantLegendsWater { GW2::RevenantLegend::None, GW2::RevenantLegend::None };
        std::array<uint32_t, 3> RevenantInactiveSkillsLand { 0, 0, 0 };
        std::array<uint32_t, 3> RevenantInactiveSkillsWater { 0, 0, 0 };
        std::array<uint8_t, 2> RangerPetsLand { 0, 0 };
        std::array<uint8_t, 2> RangerPetsWater { 0, 0 };
        std::string Error;
        std::string Info;
        bool NeedsSecondaryLink = false;
        bool SecondaryLinkIsTraits = false;
    };

    Build(id_t id) : m_id(id) { }

    [[nodiscard]] id_t GetID() const { return m_id; }
    [[nodiscard]] std::string_view GetName() const { return m_name; }
    [[nodiscard]] std::string_view GetLink() const { return m_link; }
    [[nodiscard]] std::string_view GetSecondaryLink() const { return m_secondaryLink; }
    [[nodiscard]] Flags GetFlags() const { return m_flags; }
    [[nodiscard]] bool HasFlag(Flags flag) const { return (m_flags & flag) != Flags::None; }
    [[nodiscard]] Flags GetFlagIcons() const { return m_flagIcons; }
    [[nodiscard]] bool HasFlagIcon(Flags flag) const { return (m_flagIcons & flag) != Flags::None; }
    [[nodiscard]] KeyBind const& GetKeyBind() const { return m_keyBind; }
    [[nodiscard]] std::string_view GetNormalizedName() const { return m_normalizedName; }
    [[nodiscard]] ParsedInfo const& GetParsedInfo() const { return m_parsedInfo; }

    [[nodiscard]] GW2::Profession GetParsedProfession() const { return GetParsedInfo().Profession; }
    [[nodiscard]] GW2::Specialization GetParsedSpecialization() const { return GetParsedInfo().Specialization; }

    void SetName(std::string_view name)
    {
        if (m_name == name)
            return;

        m_name = name;
        m_normalizedName = name;
        std::transform(m_normalizedName.begin(), m_normalizedName.end(), m_normalizedName.begin(), toupper);
        m_needsSave = true;
    }
    void SetLink(std::string_view link)
    {
        if (m_link == link)
            return;

        m_link = link;
        m_parsedInfo = ParseInfo(m_link);
        m_needsSave = true;

        HandleMigration();
    }
    void SetSecondaryLink(std::string_view link)
    {
        if (m_secondaryLink == link)
            return;

        m_secondaryLink = link;
        if (m_secondaryLink.empty())
            m_parsedInfo = ParseInfo(m_link);
        else
        {
            m_parsedInfo = ParseInfo(m_secondaryLink);
            m_parsedInfo.NeedsSecondaryLink = ParseInfo(m_link).NeedsSecondaryLink;
        }
        m_needsSave = true;

        HandleMigration();
    }
    void ToggleFlag(Flags flag, bool on)
    {
        if (HasFlag(flag) == on)
            return;

        if (on) m_flags |=  flag;
        else    m_flags &= ~flag;
        m_needsSave = true;
    }
    void ToggleFlagIcon(Flags flag, bool on)
    {
        if (HasFlagIcon(flag) == on)
            return;

        if (on) m_flagIcons |=  flag;
        else    m_flagIcons &= ~flag;
        m_needsSave = true;
    }
    void SetKeyBind(std::string_view keyBind) { SetKeyBind(KeyBind { keyBind }); }
    void SetKeyBind(KeyBind const keyBind)
    {
        if (m_keyBind == keyBind)
            return;

        m_keyBind = keyBind;
        m_needsSave = true;
    }

    [[nodiscard]] bool IsSaveNeeded() const { return m_needsSave; }
    void SetSaved() { m_needsSave = false; }

    void PostLoad();

private:
    id_t m_id;
    std::string m_name;
    std::string m_link;
    std::string m_secondaryLink;
    Flags m_flags = Flags::None;
    Flags m_flagIcons = Flags::None;
    KeyBind m_keyBind;
    ParsedInfo m_parsedInfo;
    std::string m_normalizedName;
    bool m_needsSave = false;

    static ParsedInfo ParseInfo(std::string_view code);
    static std::optional<std::string> ValidateParsedInfo(ParsedInfo& parsed);
    void HandleMigration();
};
}
