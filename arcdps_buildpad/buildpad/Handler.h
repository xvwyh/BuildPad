#pragma once
#include "../Common.h"
#include "Defines.h"
#include "Build.h"
#include "MumbleLink.h"
#include "ChatLink.h"
#include "KeyBind.h"
#include <d3d9.h>

namespace buildpad
{
extern char const* const BUILDPAD_VERSION;

class Window;
class StateIcon;

class Handler : public Singleton<Handler>
{
public:
    Handler();
    ~Handler();

    void LoadTextures();
    void LoadConfig();
    void SaveConfig();
    void LoadTest();
    void Unload();

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
        MissingProfession,
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
        PaletteNotUnderwater,
        CheckBoxUnchecked,
        CheckBoxChecked,
    };
    template<typename Key>
    [[nodiscard]] TextureData const& GetIcon(Key key) const { return GetIconContainer<Key>().at(key); }
    [[nodiscard]] std::optional<TextureData> LoadTexture(std::variant<fs::path, std::pair<char const*, size_t>>&& source);
    void UnloadTexture(TextureData& texture) { UnloadTexture(std::exchange(texture.Texture, TextureID { })); }
    void UnloadTexture(TextureID const& texture);

    [[nodiscard]] bool ArePetsLoaded() const { return m_petsLoaded; }
    [[nodiscard]] std::vector<uint32_t> const& GetPets() const { return m_petIDs; }

    [[nodiscard]] float GetUIScale() const { return m_config.UIScale / 100.0f; }
    [[nodiscard]] bool IsLessTransparentButtonsEnabled() const { return m_config.LessTransparentButtons; }

private:
    bool m_loaded = false;
    IDirect3DDevice9* m_d3dDevice = nullptr;
    KeyBind m_keyBindToggleBuilds;
    using Clock = std::chrono::high_resolution_clock;
    Clock::time_point m_previousUpdate { };
    std::list<TextureID> m_loadedTextures;

    std::future<void> m_skillLoading;

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

    using KeyBindCallback = std::function<void(KeyBind const&)>;
    void EditKeyBind(KeyBind const& keyBind, KeyBindCallback&& callback);
    void RenderKeyBindEditor();
    bool m_keyBindEditing = false;
    KeyBind m_keyBindEdited;
    KeyBindCallback m_keyBindCallback;

    void BeginRenderBuildList(GW2::Profession profession, bool& firstVisible, bool& firstSorted, bool singleProfession, GW2::Profession professionColor, float colorMultiplier = 1.0f) const;
    void EndRenderBuildList(bool singleProfession) const;

    void RenderBuildTooltip(Build const& build, bool footer = true, bool errorMissing = false, Build* editTarget = nullptr, bool allowChangeProfession = false) const;

    template<typename DataType, typename InfoType, typename InfoSourceType, typename APIType>
    struct PaletteContext
    {
        Build* EditTarget;
        std::string Context;

        std::function<void()> Preload;

        std::function<DataType(bool water, uint8_t index)> Getter;
        std::function<void(ChatLink::BuildTemplate& data, bool water, uint8_t index, DataType selection)> Setter;

        bool PaletteSourceLoaded;
        InfoSourceType const& PaletteSource;
        std::function<bool(InfoType const& info, bool water, uint8_t index)> PaletteFilter;
        std::function<bool(InfoType const& info, bool water, uint8_t index)> PaletteActive;
        std::function<bool(InfoType const& info, bool water, uint8_t index)> PaletteUsable;
        std::function<bool(InfoType const* a, InfoType const* b)> PaletteSorter;

        Icons MissingAPIIcon;
        std::string APIType::*APIName;
        StateIcon APIType::*APIIcon;
        std::function<TextureData const&(DataType const& selection, bool palette)> IconGetter;
        std::function<DataType(InfoType const& info)> InfoToDataTransform;
        std::function<uint32_t(DataType const& selection, bool water, uint8_t index)> DataToAPITransform;

        bool Water;
        bool DarkenFirstHalf = false;
        bool DarkenSecondHalf = false;
        ImVec2 TypeSize;
        bool BarVertical;
        float BarSpacing;
        uint8_t BarButtonCount;
        uint8_t BarButtonPerRow = 0;
        ImVec2 ButtonSize;
        float ButtonSpacing;
        ImVec2 PaletteSize;
        ImVec2 PaletteSpacing;
        uint8_t PalettePerRow;
        bool PaletteReverse = false;

        std::string ButtonTooltip;

        PaletteContext(std::string_view context, InfoSourceType const& paletteSource, std::string APIType::*apiName, StateIcon APIType::*apiIcon) : Context(context), PaletteSource(paletteSource), APIName(apiName), APIIcon(apiIcon) { }
    };
    template<typename DataType, typename InfoType, typename InfoSourceType, typename APIType>
    void RenderPaletteBar(PaletteContext<DataType, InfoType, InfoSourceType, APIType> const& context) const;

    struct BuildEditContext
    {
        static inline uint32_t NextWindowID = 0;
        uint32_t WindowID = NextWindowID++;

        Build::id_t ID;
        Build Original;
        Build TemporaryEditTarget;
        bool BuildStorageEditedBuild = false;
        bool Closed = false;

        bool IsChanged();
        void RevertChanges();
        Build const& GetOriginal();
        Build& GetEditTarget();

        BuildEditContext(Build const& build, bool buildStorageEditedBuild) : ID(build.GetID()), Original(build), TemporaryEditTarget(build), BuildStorageEditedBuild(buildStorageEditedBuild) { }
    };
    std::list<BuildEditContext> m_editedBuilds;
    void RenderBuildEditor(BuildEditContext& context) const;
    void RenderBuildEditors();
    void CloseBuildEditor(Build const& build) { m_editedBuilds.remove_if(util::member_equals(&BuildEditContext::ID, build.GetID())); }
    void OpenBuildEditor(Build const& build, bool buildStorageEditedBuild) { CloseBuildEditor(build); m_editedBuilds.emplace_back(build, buildStorageEditedBuild); }

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
        std::string LastLaunchedVersion;
        std::string SkipUpdateVersion;
        bool ArcDPSMigrationHintHidden = false;
        std::string KeyBindToggleBuilds = "ALT+SHIFT+D";
        std::string APILocale = "en";
        bool ShowFilterPanel = true;
        bool ShowProfessionFilter = true;
        bool ShowNameFilter = true;
        bool ShowFlagsFilter = true;
        bool ShowSettingsButton = true;
        bool SimpleFlagsFilter = false;
        bool ClearFiltersOnWindowClose = false;
        bool LessTransparentButtons = false;
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
        uint32_t UIScale = 100;
        bool LockWindowPosition = false;
        int32_t WindowPositionX = 0;
        int32_t WindowPositionY = 0;
        bool LockWindowSize = false;
        uint32_t WindowSizeW = 0;
        uint32_t WindowSizeH = 0;
        bool AutoWindowHeight = false;
        bool HideWindowHeader = false;
        bool KeepWindowInBounds = true;
        std::string GearIconSet;
        bool Snow = true;

        using field_t = std::variant<bool(Config::*), uint32_t(Config::*), int32_t(Config::*), std::string(Config::*), std::array<uint32_t, 10>(Config::*)>;
        [[nodiscard]] static std::vector<std::pair<std::string, field_t>> const& GetFields()
        {
#define FIELD(name) { #name, &Config::##name }
            static std::vector<std::pair<std::string, field_t>> const instance
            {
                FIELD(LastLaunchedVersion),
                FIELD(SkipUpdateVersion),
                FIELD(ArcDPSMigrationHintHidden),
                FIELD(KeyBindToggleBuilds),
                FIELD(APILocale),
                FIELD(ShowFilterPanel),
                FIELD(ShowProfessionFilter),
                FIELD(ShowNameFilter),
                FIELD(ShowFlagsFilter),
                FIELD(ShowSettingsButton),
                FIELD(SimpleFlagsFilter),
                FIELD(ClearFiltersOnWindowClose),
                FIELD(LessTransparentButtons),
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
                FIELD(UIScale),
                FIELD(LockWindowPosition),
                FIELD(WindowPositionX),
                FIELD(WindowPositionY),
                FIELD(LockWindowSize),
                FIELD(WindowSizeW),
                FIELD(WindowSizeH),
                FIELD(AutoWindowHeight),
                FIELD(HideWindowHeader),
                FIELD(KeepWindowInBounds),
                FIELD(GearIconSet),
                FIELD(Snow),
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
