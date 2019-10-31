#pragma once
#include <utility>
#include "Defines.h"
#include "Build.h"

namespace buildpad
{
class BuildStorage : public Singleton<BuildStorage>
{
public:
    using Container = std::list<Build>;

    class SortedContainerWrapper
    {
        using PointerContainer = std::list<Build*>;

    public:
        class iterator
        {
        public:
            iterator(PointerContainer::iterator cur, PointerContainer::iterator end) : m_cur(std::move(cur)), m_end(std::move(end)) { }
            iterator& operator++() { ++m_cur; return *this; }
            bool operator==(iterator const& b) const { return m_cur == b.m_cur && m_end == b.m_end; }
            bool operator!=(iterator const& b) const { return !(*this == b); }
            operator bool() const { return m_cur != m_end; }
            Build& operator*() const { return **m_cur; }
            Build* operator->() const { return *m_cur; }

        private:
            PointerContainer::iterator m_cur;
            PointerContainer::iterator m_end;
        };

        explicit SortedContainerWrapper(Container& container)
        {
            std::transform(container.begin(), container.end(), std::back_inserter(m_sorted), [](Build& build) { return &build; });
            m_sorted.sort([](Build* a, Build* b) { return a->GetNormalizedName() < b->GetNormalizedName(); });
        }

        iterator begin() { return { m_sorted.begin(), m_sorted.end() }; }
        iterator end()   { return { m_sorted.  end(), m_sorted.end() }; }

    private:
        PointerContainer m_sorted;
    };

    // Builds
    [[nodiscard]] Container& GetBuilds() { return m_builds; }
    [[nodiscard]] Container const& GetBuilds() const { return m_builds; }
    [[nodiscard]] Build* FindBuild(Build::id_t id) { auto const itr = GetBuildIterator(id); return itr != m_builds.end() ? &*itr : nullptr; }
    [[nodiscard]] SortedContainerWrapper GetSortedBuilds() { return SortedContainerWrapper { m_builds } ; }

    Build& AddBuild();
    void RemoveBuild(Build const& build);
    void MoveBuildBefore(Build const& build, Build const& before);
    void MoveBuildAfter(Build const& build, Build const& after);
    void MoveBuildToTop(Build const& build);
    void MoveBuildToBottom(Build const& build);
    void MoveBuildUp(Build const& build);
    void MoveBuildDown(Build const& build);

    // Editing
    [[nodiscard]] bool IsEditingBuild() const { return m_editedBuild.has_value(); }
    [[nodiscard]] bool IsEditingBuild(Build const& build) const { return IsEditingBuild() && m_editedBuild->GetID() == build.GetID(); }
    [[nodiscard]] Build& GetEditedBuild() { return *m_editedBuild; }
    [[nodiscard]] Build& GetEditTargetBuild() { return *GetBuildIterator(*m_editedBuild) ; }
    Build& EditBuild(Build const& build);
    void AcceptBuildEdit();
    void CancelBuildEdit();

    // Filtering
    [[nodiscard]] bool CanFilterProfession() const { return m_currentProfession != GW2::Profession::None; }
    [[nodiscard]] GW2::Profession GetCurrentProfession() const { return m_currentProfession; }
    [[nodiscard]] bool IsFilteringProfession() const { return CanFilterProfession() && m_professionFilter; }
    void SetCurrentProfession(GW2::Profession profession) { m_currentProfession = profession; }
    void ToggleProfessionFilter(bool on)
    {
        m_professionFilter = on;
        m_needsSave = true;
    }

    [[nodiscard]] bool IsFilteringFlags() const { return m_flagsFilter != Build::Flags::None; }
    [[nodiscard]] bool IsFilteringFlag(Build::Flags flag) const { return ((std::underlying_type_t<Build::Flags>)m_flagsFilter & (std::underlying_type_t<Build::Flags>)flag) != 0; }
    void ToggleFlagFilter(Build::Flags flag, bool on)
    {
        if (IsFilteringFlag(flag) == on)
            return;

        m_flagsFilter = (Build::Flags)(on
            ? (std::underlying_type_t<Build::Flags>)m_flagsFilter | (std::underlying_type_t<Build::Flags>)flag
            : (std::underlying_type_t<Build::Flags>)m_flagsFilter & ~(std::underlying_type_t<Build::Flags>)flag);
        m_needsSave = true;
    }

    [[nodiscard]] bool IsFilteringName() const { return !m_nameFilter.empty(); }
    [[nodiscard]] std::string_view GetNameFilter() const { return m_nameFilter; }
    [[nodiscard]] std::string_view GetNormalizedNameFilter() const { return m_normalizedNameFilter; }
    void SetNameFilter(std::string_view filter)
    {
        m_nameFilter = filter;
        m_normalizedNameFilter = filter;
        std::transform(m_normalizedNameFilter.begin(), m_normalizedNameFilter.end(), m_normalizedNameFilter.begin(), toupper);
    }

    [[nodiscard]] bool IsBuildMatchingFilter(Build const& build) const;

    // Saving
    [[nodiscard]] bool IsSaveNeeded() const { return m_needsSave || std::any_of(m_builds.begin(), m_builds.end(), [](Build const& build) { return build.IsSaveNeeded(); }); }
    void Save(std::ofstream& file);
    void LoadBegin()
    {
        m_builds.clear();
        m_editedBuild.reset();
        m_professionFilter = true;
        m_flagsFilter = Build::Flags::None;
    }
    bool Load(std::string_view section, std::string_view name, std::string_view value);
    void LoadEnd() { SetSaved(); }

private:
    // Builds
    Container m_builds;
    Container::iterator GetBuildIterator(Build const& build) { return GetBuildIterator(build.GetID()); }
    Container::iterator GetBuildIterator(Build::id_t id) { return std::find_if(m_builds.begin(), m_builds.end(), [id](Build const& b) { return b.GetID() == id; }); }

    // Editing
    std::optional<Build> m_editedBuild;

    // Filtering
    bool m_professionFilter = true;
    GW2::Profession m_currentProfession = GW2::Profession::None;
    Build::Flags m_flagsFilter = Build::Flags::None;
    std::string m_nameFilter;
    std::string m_normalizedNameFilter;

    // Saving
    bool m_needsSave = false;
    void SetSaved()
    {
        m_needsSave = false;
        for (Build& build : m_builds)
            build.SetSaved();
    }
};
}
