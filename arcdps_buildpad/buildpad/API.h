#pragma once
#include "Defines.h"
#include "Handler.h"
#include "GW2.h"
#include "Web.h"
#include "StateIcon.h"

namespace buildpad
{
class Build;

class API : public Singleton<API>
{
    template<typename T>
    struct InfoHandlers;

    using Icons = Handler::Icons;

public:
    template<typename T>
    struct Info
    {
        uint32_t ID = 0;
        bool Loaded = false;

        Info(uint32_t id) : ID(id) { }

        static T& Get(uint32_t const id) { return Instance().Get<T>(id); }
        operator bool() const { return Loaded; }
    };
    struct Profession : Info<Profession>
    {
        using Info::Info;
        std::string Name;
        std::vector<GW2::Specialization> Specializations;
        std::vector<uint32_t> Skills;
        StateIcon Icon { Icons::MissingProfession, Icons::MissingProfession };
    };
    struct Specialization : Info<Specialization>
    {
        using Info::Info;
        std::string Name;
        std::array<uint32_t, 3> MinorTraits { };
        std::array<uint32_t, 9> MajorTraits { };
        uint32_t WeaponTrait = 0;
        std::string IconURL;
        bool IconLoaded = false;
        StateIcon Icon { Icons::LoadingSpecialization, Icons::ErrorSpecialization };

        [[nodiscard]] uint16_t GetTrait(uint8_t rank, uint8_t order) const
        {
            if (order)
                if (size_t const index = rank * 3 + (order - 1); index < MajorTraits.size())
                    return MajorTraits[index];

            return 0;
        }
    };
    struct Trait : Info<Trait>
    {
        using Info::Info;
        std::string Name;
        bool Major = false;
        StateIcon Icon { Icons::LoadingTrait, Icons::ErrorTrait, Icons::LoadingTraitMinor, Icons::ErrorTraitMinor };

        [[nodiscard]] std::string ToChatLink() const
        {
            ChatLink::Trait data { ID };
            return ID ? ChatLink::Encode((unsigned char const*)&data, sizeof data) : std::string { };
        }
    };
    struct Skill : Info<Skill>
    {
        using Info::Info;
        std::string Name;
        bool NoUnderwater = false;
        StateIcon Icon { Icons::LoadingSkill, Icons::ErrorSkill };

        [[nodiscard]] std::string ToChatLink() const
        {
            ChatLink::Skill data { ID };
            return ID ? ChatLink::Encode((unsigned char const*)&data, sizeof data) : std::string { };
        }
    };
    struct Item : Info<Item>
    {
        using Info::Info;
        std::string Name = "...";
        GW2::Rarity Rarity = GW2::Rarity::Junk;
        StateIcon Icon { Icons::LoadingItem, Icons::ErrorItem };

        [[nodiscard]] std::string ToChatLink() const
        {
            ChatLink::Item data { ID, 1 };
            return ID ? ChatLink::Encode((unsigned char const*)&data, sizeof data) : std::string { };
        }
    };
    struct ItemStats : Info<ItemStats>
    {
        using Info::Info;
        std::string Name = "...";
    };
    struct Pet : Info<Pet>
    {
        using Info::Info;
        std::string Name;
        StateIcon Icon { Icons::LoadingPet, Icons::ErrorPet };
    };

    struct LanguageInfo
    {
        std::string Tag;
        std::string Name;
    };
    static std::vector<LanguageInfo> const& GetLanguageInfos()
    {
        static std::vector<LanguageInfo> const instance
        {
            { "en", "English" },
            { "es", "Spanish" },
            { "de", "German"  },
            { "fr", "French"  },
            { "zh", "Chinese" },
        };
        return instance;
    }
    [[nodiscard]] std::string_view GetLanguage() const { return m_lang; }
    void SetLanguage(std::string_view const lang)
    {
        if (m_lang == lang)
            return;

        m_lang = lang;
        Unload();
    }

    void Unload()
    {
        ++m_generation;
        UnloadContainer<Profession>(&Profession::Icon);
        UnloadContainer<Specialization>(&Specialization::Icon);
        UnloadContainer<Trait>(&Trait::Icon);
        UnloadContainer<Skill>(&Skill::Icon);
        UnloadContainer<Item>(&Item::Icon);
        UnloadContainer<ItemStats>();
        UnloadContainer<Pet>(&Pet::Icon);
        m_professionsRequested = false;
    }

    void LoadSkillData() const;
    void PreloadAllPets();
    void PreloadAllProfessions();
    void PreloadAllProfessionSkills(GW2::Profession profession);
    void PreloadAllProfessionSpecializations(GW2::Profession profession);
    bool PreloadAllBuildInfos(Build const& build);
    bool PreloadAllGearInfos(ChatLink::ArcDPSLegendaryTemplate<uint32_t> const& gear);

private:
    friend class StateIcon;

    template<typename T>
    using InfoContainer = std::unordered_map<uint32_t, T>;
    template<typename T>
    struct InfoHandlers;

    std::string m_lang;
    std::string m_schema = "2019-12-19T00:00:00.000Z";

    bool m_professionsRequested = false;
    uint32_t m_generation = 0;
    InfoContainer<Profession> m_professions;
    InfoContainer<Specialization> m_specializations;
    InfoContainer<Trait> m_traits;
    InfoContainer<Skill> m_skills;
    InfoContainer<Item> m_items;
    InfoContainer<ItemStats> m_itemStats;
    InfoContainer<Pet> m_pets;

    template<typename T>
    void UnloadContainer() { GetInfoContainer<T>().clear(); }
    template<typename T>
    void UnloadContainer(StateIcon T::*icon)
    {
        for (auto&& [id, info] : GetInfoContainer<T>())
            (info.*icon).Unload();
        UnloadContainer<T>();
    }

    template<typename T>
    [[nodiscard]] bool Has(uint32_t id) const { return GetInfoContainer<T>().find(id) != GetInfoContainer<T>().end(); }
    template<typename T> T& Get(uint32_t id) { return GetInfoContainer<T>().try_emplace(id, id).first->second; }
    template<typename T, typename Range> bool Reserve(uint32_t id, Range& range, bool& loaded)
    {
        bool const missing = !Has<T>(id);
        if (missing)
            range.emplace(id);
        T& info = Get<T>(id);
        loaded &= info;
        return missing;
    }

    template<typename T>
    [[nodiscard]] InfoContainer<T> const& GetInfoContainer() const;
    template<typename T>
    InfoContainer<T>& GetInfoContainer() { return const_cast<InfoContainer<T>&>(const_cast<API const*>(this)->GetInfoContainer<T>()); }

    template<typename T, typename Range>
    void Request(Range&& ids)
    {
        if (std::empty(ids))
            return;

        Web::Instance().Request(
            fmt::format(InfoHandlers<T>::URL, m_lang, m_schema, fmt::join(std::begin(ids), std::end(ids), ",")),
            [this, generation = m_generation](std::string_view data) { if (generation == m_generation) InfoHandlers<T>::Success(data); },
            [this, generation = m_generation, ids](auto&&)           { if (generation == m_generation) InfoHandlers<T>::Error(ids); });
    }
    void DownloadIcon(StateIcon& icon, std::string_view const url, std::function<TextureData(TextureData)> const& transform = {})
    {
        Web::Instance().Request(url,
            [this, &icon, transform, generation = m_generation](std::string_view data)
            {
                if (generation == m_generation)
                    if (auto texture = Handler::Instance().LoadTexture(std::pair { data.data(), data.size() }))
                        icon.Set(transform ? transform(*texture) : *texture, true);
            },
            [this, &icon, generation = m_generation](auto&&)
            {
                if (generation == m_generation)
                    icon.SetError();
            });
    }
};

template<> [[nodiscard]] inline auto API::GetInfoContainer() const -> InfoContainer<Profession>       const& { return m_professions; }
template<> [[nodiscard]] inline auto API::GetInfoContainer() const -> InfoContainer<Specialization>   const& { return m_specializations; }
template<> [[nodiscard]] inline auto API::GetInfoContainer() const -> InfoContainer<Trait>            const& { return m_traits; }
template<> [[nodiscard]] inline auto API::GetInfoContainer() const -> InfoContainer<Skill>            const& { return m_skills; }
template<> [[nodiscard]] inline auto API::GetInfoContainer() const -> InfoContainer<Item>             const& { return m_items; }
template<> [[nodiscard]] inline auto API::GetInfoContainer() const -> InfoContainer<ItemStats>        const& { return m_itemStats; }
template<> [[nodiscard]] inline auto API::GetInfoContainer() const -> InfoContainer<Pet>              const& { return m_pets; }

template<> struct API::InfoHandlers<API::Profession>        { static void Success(std::string_view data); static void Error(std::set<uint32_t> const& ids); inline static char const* URL = "https://api.guildwars2.com/v2/professions?lang={}&v={}&ids=all"; };
template<> struct API::InfoHandlers<API::Specialization>    { static void Success(std::string_view data); static void Error(std::set<uint32_t> const& ids); inline static char const* URL = "https://api.guildwars2.com/v2/specializations?lang={}&v={}&ids={}"; };
template<> struct API::InfoHandlers<API::Trait>             { static void Success(std::string_view data); static void Error(std::set<uint32_t> const& ids); inline static char const* URL = "https://api.guildwars2.com/v2/traits?lang={}&v={}&ids={}"; };
template<> struct API::InfoHandlers<API::Skill>             { static void Success(std::string_view data); static void Error(std::set<uint32_t> const& ids); inline static char const* URL = "https://api.guildwars2.com/v2/skills?lang={}&v={}&ids={}"; };
template<> struct API::InfoHandlers<API::Item>              { static void Success(std::string_view data); static void Error(std::set<uint32_t> const& ids); inline static char const* URL = "https://api.guildwars2.com/v2/items?lang={}&v={}&ids={}"; };
template<> struct API::InfoHandlers<API::ItemStats>         { static void Success(std::string_view data); static void Error(std::set<uint32_t> const& ids); inline static char const* URL = "https://api.guildwars2.com/v2/itemstats?lang={}&v={}&ids={}"; };
template<> struct API::InfoHandlers<API::Pet>               { static void Success(std::string_view data); static void Error(std::set<uint32_t> const& ids); inline static char const* URL = "https://api.guildwars2.com/v2/pets?lang={}&v={}&ids={}"; };
}
