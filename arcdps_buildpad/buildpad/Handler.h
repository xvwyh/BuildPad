#pragma once
#include "../Common.h"
#include "Defines.h"
#include "Build.h"
#include "MumbleLink.h"
#include "ChatLink.h"
#include <d3d9.h>

namespace buildpad
{
extern char const* const BUILDPAD_VERSION;

class Window;

class Handler : public Singleton<Handler>
{
public:
    Handler();
    ~Handler();

    void LoadTextures();
    void LoadConfig();
    void SaveConfig();
    void LoadTest();

    bool HandleKeyBinds();
    void Update();
    void UpdateOptions();

    void OnMainThread(std::function<void()>&& function)
    {
        if (m_mainThreadQueueExecuting)
        {
            std::lock_guard lock { m_mainThreadQueueTailLock };
            m_mainThreadQueueTail.emplace_back(std::move(function));
        }
        else
        {
            std::lock_guard lock { m_mainThreadQueueLock };
            m_mainThreadQueue.emplace_back(std::move(function));
        }
    }
    void RepeatCurrentMainThreadTask() { m_mainThreadQueueRepeat = true; }

    enum class Icons : uint8_t
    {
        BuildPad,
        AddBuild,
        EditBuild,
        AcceptBuildEdit,
        CancelBuildEdit,
        ClearSearch,
        Settings,
        MissingSkill,
        LandSkills,
        WaterSkills,
        LoadingSkill,
        ErrorSkill,
        MissingSpecialization,
        LoadingSpecialization,
        ErrorSpecialization,
        MissingTrait,
        LoadingTrait,
        ErrorTrait,
        LoadingTraitMinor,
        ErrorTraitMinor,
        LoadingItem,
        ErrorItem,
        MissingPet,
        LoadingPet,
        ErrorPet,
        SelectionChevron,
    };
    template<typename Key>
    [[nodiscard]] TextureData const& GetIcon(Key key) const { return GetIconContainer<Key>().at(key); }
    [[nodiscard]] std::optional<TextureData> LoadTexture(std::variant<fs::path, std::pair<char const*, size_t>>&& source) const;

    [[nodiscard]] uint32_t SkillPaletteToSkill(uint32_t palette, GW2::RevenantLegend legend) const
    {
        if (auto const index = GW2::RevenantLegendInfo::GetSkillIndex(palette))
            return GW2::GetRevenantLegendInfo(legend).Skills[*index];

        auto const itr = m_palettes.find(palette);
        return itr != m_palettes.end() ? itr->second.front() : 0;
    }
    [[nodiscard]] GW2::Specialization GetPaletteSpecialization(uint32_t palette, GW2::RevenantLegend legend) const
    {
        auto const itr = m_skills.find(SkillPaletteToSkill(palette, legend));
        return itr != m_skills.end() ? itr->second.Specialization : GW2::Specialization::None;
    }

    [[nodiscard]] std::vector<uint32_t> const& GetPets() const { return m_petIDs; }

private:
    bool m_loaded = false;
    IDirect3DDevice9* m_d3dDevice = nullptr;
    struct KeyBind
    {
        using key_t = uint8_t;

        bool Control = false;
        bool Alt = false;
        bool Shift = false;
        key_t Key { };

        bool IsPressed() const;
        bool FromString(std::string_view str);
        std::optional<std::string> ToString() const;
        static std::optional<std::string> KeyToString(key_t key);
        static std::map<key_t, std::string_view> const& GetKeyMap();
    } m_keyBindToggleBuilds, m_keyBindEdited;
    using Clock = std::chrono::high_resolution_clock;
    Clock::time_point m_previousUpdate { };

    struct Skill
    {
        enum class SkillType : uint8_t
        {
            None,
            Profession,
            Heal,
            Utility,
            Elite,
        };
        uint32_t ID = 0;
        SkillType Type = SkillType::None;
        GW2::Specialization Specialization = GW2::Specialization::None;
        std::vector<uint32_t> Palettes;
        std::string Name;
    };
    std::unordered_map<uint32_t, Skill> m_skills;
    std::unordered_map<uint32_t, std::vector<uint32_t>> m_palettes;

    struct Pet
    {
        enum class PetType : uint8_t
        {
            None,
            Terrestrial,
            Aquatic,
            Amphibious,
        };
        uint32_t ID = 0;
        PetType Type = PetType::None;
    };
    std::vector<Pet> m_pets;
    std::vector<uint32_t> m_petIDs;
    bool m_petsLoaded = false;

    template<typename Key>
    using TextureContainer = std::unordered_map<Key, TextureData>;
    TextureContainer<Icons> m_icons;
    TextureContainer<GW2::Profession> m_professionIcons;
    TextureContainer<GW2::Specialization> m_specializationIcons;
    TextureContainer<GW2::Slot> m_slotIcons;
    TextureContainer<Build::Flags> m_flagIcons;
    MumbleLink m_mumbleLink;
    Time m_mumbleLinkUpdateTimer { };

    // Window Rendering
    void RenderMainWindow(Time const& delta);
    bool m_shown = false;

    void RenderSettings(bool menu);
    bool m_detachSettings = false;

    void BeginRenderBuildList(GW2::Profession profession, bool& firstVisible, bool& firstSorted, bool singleProfession, GW2::Profession professionColor, float colorMultiplier = 1.0f) const;
    void EndRenderBuildList(bool singleProfession) const;

    void RenderBuildTooltip(Build const& build, bool footer = true, bool errorMissing = false, Build* editTarget = nullptr) const;

    void RenderArcDPSMigration(Time const& delta);
    std::future<void> m_arcdpsMigrationDiscovery;
    bool m_arcdpsMigrationAvailable = false;
    bool m_arcdpsMigrationShown = false;
    std::list<Build> m_arcdpsTraits;
    std::list<Build> m_arcdpsSkills;

    void RenderArcDPSGear(Time const& delta);
    bool m_arcdpsGearAvailable = false;
    bool m_arcdpsGearShown = false;
    using ArcDPSGearTemplate = ChatLink::ArcDPSGearTemplate<uint32_t>;
    using ArcDPSLegendaryTemplate = ChatLink::ArcDPSLegendaryTemplate<uint32_t>;
    std::list<std::variant<ArcDPSGearTemplate, ArcDPSLegendaryTemplate>> m_arcdpsGear;

    void VersionCheck();
    void VersionUpdate();
    void RenderVersionHistory(bool all = false);
    struct VersionInfo
    {
        std::string Version;
        std::string URL;
        std::vector<std::pair<std::string, std::string>> Changes;
    };
    std::mutex m_versionLock;
    std::string m_versionCurrent;
    VersionInfo m_versionLatest;
    std::map<std::string, VersionInfo> m_versionHistory;
    std::map<std::string, uint32_t> m_versionHistoryColors;
    bool m_versionHistoryShown = false;
    bool m_versionHistoryShownAll = false;
    std::optional<bool> m_versionCheckResult;
    bool m_needsUpdate = false;
    enum class VersionUpdateState
    {
        Idle,
        Downloading,
        Manifesting,
        Done,
        Error,
    } m_versionUpdateState = VersionUpdateState::Idle;
    std::string m_versionUpdateError;

    void RenderAbout();
    bool m_aboutShown = false;

    // Threading
    std::deque<std::function<void()>> m_mainThreadQueue;
    std::deque<std::function<void()>> m_mainThreadQueueTail;
    std::mutex m_mainThreadQueueLock;
    std::mutex m_mainThreadQueueTailLock;
    std::atomic<bool> m_mainThreadQueueExecuting = false;
    bool m_mainThreadQueueRepeat = false;

    // Config
    struct Config
    {
        std::string SkipUpdateVersion;
        bool ArcDPSMigrationHintHidden = false;
        std::string KeyBindToggleBuilds = "ALT+SHIFT+D";
        std::string APILocale = "en";
        bool ShowFilterPanel = true;
        bool ShowProfessionFilter = true;
        bool ShowNameFilter = true;
        bool ShowFlagsFilter = true;
        bool ShowSettingsButton = true;
        uint32_t HideFlagsMask = 0;
        bool HiddenFiltersHintHidden = false;
        bool UseProfessionColors = true;
        bool ProfessionColorsUnfilteredOnly = true;
        std::array<uint32_t, 10> ProfessionColors
        {
#define COLOR(profession) ImGui::ColorConvertFloat4ToU32(GW2::GetProfessionInfo(profession).GetDefaultButtonColor())
            COLOR(GW2::Profession::None),
            COLOR(GW2::Profession::Guardian),
            COLOR(GW2::Profession::Warrior),
            COLOR(GW2::Profession::Engineer),
            COLOR(GW2::Profession::Ranger),
            COLOR(GW2::Profession::Thief),
            COLOR(GW2::Profession::Elementalist),
            COLOR(GW2::Profession::Mesmer),
            COLOR(GW2::Profession::Necromancer),
            COLOR(GW2::Profession::Revenant),
#undef COLOR
        };
        bool SortBuildsByProfession = true;
        bool SortBuildsByProfessionCurrentFirst = false;
        bool SortBuildsByProfessionShowIcons = false;
        bool SortBuildsByProfessionShowNames = false;
        uint32_t SortBuildsByProfessionSpacing = 4;
        bool SortBuildsAlphabetically = false;
        bool AllowBuildReordering = true;
        uint32_t TooltipDelay = 500;
        bool LockWindowPosition = false;
        int32_t WindowPositionX = 0;
        int32_t WindowPositionY = 0;
        bool LockWindowSize = false;
        uint32_t WindowSizeW = 0;
        uint32_t WindowSizeH = 0;
        bool AutoWindowHeight = false;
        bool KeepWindowInBounds = true;

        using field_t = std::variant<bool(Config::*), uint32_t(Config::*), int32_t(Config::*), std::string(Config::*), std::array<uint32_t, 10>(Config::*)>;
        [[nodiscard]] static std::vector<std::pair<std::string, field_t>> const& GetFields()
        {
#define FIELD(name) { #name, &Config::##name }
            static std::vector<std::pair<std::string, field_t>> const instance
            {
                FIELD(SkipUpdateVersion),
                FIELD(ArcDPSMigrationHintHidden),
                FIELD(KeyBindToggleBuilds),
                FIELD(APILocale),
                FIELD(ShowFilterPanel),
                FIELD(ShowProfessionFilter),
                FIELD(ShowNameFilter),
                FIELD(ShowFlagsFilter),
                FIELD(ShowSettingsButton),
                FIELD(HideFlagsMask),
                FIELD(HiddenFiltersHintHidden),
                FIELD(UseProfessionColors),
                FIELD(ProfessionColorsUnfilteredOnly),
                FIELD(ProfessionColors),
                FIELD(SortBuildsByProfession),
                FIELD(SortBuildsByProfessionCurrentFirst),
                FIELD(SortBuildsByProfessionShowIcons),
                FIELD(SortBuildsByProfessionShowNames),
                FIELD(SortBuildsByProfessionSpacing),
                FIELD(SortBuildsAlphabetically),
                FIELD(AllowBuildReordering),
                FIELD(TooltipDelay),
                FIELD(LockWindowPosition),
                FIELD(WindowPositionX),
                FIELD(WindowPositionY),
                FIELD(LockWindowSize),
                FIELD(WindowSizeW),
                FIELD(WindowSizeH),
                FIELD(AutoWindowHeight),
                FIELD(KeepWindowInBounds),
            };
#undef FIELD
            return instance;
        }
    } m_config, m_savedConfig;
    [[nodiscard]] static fs::path GetConfigPath();
    [[nodiscard]] bool IsSaveNeeded() const;
    bool Load(std::string_view section, std::string_view name, std::string_view value);

    // Textures
    [[nodiscard]] std::tuple<std::vector<char>, uint32_t, uint32_t> LoadImageFile(std::variant<fs::path, std::pair<char const*, size_t>>&& source) const;
    template<typename Key, size_t Size>
    TextureData& LoadIcon(Key key, byte const (&data)[Size]);
    template<typename T> TextureContainer<T>& GetIconContainer();
    template<typename T> TextureContainer<T> const& GetIconContainer() const;
};

template<typename Key, size_t Size>
TextureData& Handler::LoadIcon(Key key, byte const (&data)[Size])
{
    if (auto tex = LoadTexture(std::pair { (char const*)data, Size }))
        return GetIconContainer<Key>().emplace(key, *tex).first->second;

    assert(false);
    std::terminate();
}

template<> inline auto Handler::GetIconContainer()       -> TextureContainer<Icons>      & { return m_icons; }
template<> inline auto Handler::GetIconContainer() const -> TextureContainer<Icons> const& { return m_icons; }
template<> inline auto Handler::GetIconContainer()       -> TextureContainer<GW2::Profession>      & { return m_professionIcons; }
template<> inline auto Handler::GetIconContainer() const -> TextureContainer<GW2::Profession> const& { return m_professionIcons; }
template<> inline auto Handler::GetIconContainer()       -> TextureContainer<GW2::Specialization>      & { return m_specializationIcons; }
template<> inline auto Handler::GetIconContainer() const -> TextureContainer<GW2::Specialization> const& { return m_specializationIcons; }
template<> inline auto Handler::GetIconContainer()       -> TextureContainer<GW2::Slot>      & { return m_slotIcons; }
template<> inline auto Handler::GetIconContainer() const -> TextureContainer<GW2::Slot> const& { return m_slotIcons; }
template<> inline auto Handler::GetIconContainer()       -> TextureContainer<Build::Flags>      & { return m_flagIcons; }
template<> inline auto Handler::GetIconContainer() const -> TextureContainer<Build::Flags> const& { return m_flagIcons; }
}
