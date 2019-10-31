#include "BuildStorage.h"

namespace buildpad
{
Build& BuildStorage::AddBuild()
{
    auto const itr = std::max_element(m_builds.begin(), m_builds.end(), [](Build const& a, Build const& b) { return a.GetID() < b.GetID(); });
    m_needsSave = true;
    return m_builds.emplace_back(itr != m_builds.end() ? itr->GetID() + 1 : 1);
}

void BuildStorage::RemoveBuild(Build const& build)
{
    if (auto const itr = GetBuildIterator(build); itr != m_builds.end())
    {
        if (IsEditingBuild(build))
            CancelBuildEdit();

        m_builds.erase(itr);
        m_needsSave = true;
    }
}

void BuildStorage::MoveBuildBefore(Build const& build, Build const& before)
{
    if (auto const itr = GetBuildIterator(build); itr != m_builds.end())
    {
        if (auto const dest = GetBuildIterator(before); dest != m_builds.end())
        {
            m_builds.insert(dest, *itr);
            m_builds.erase(itr);
            m_needsSave = true;
        }
    }
}

void BuildStorage::MoveBuildAfter(Build const& build, Build const& after)
{
    if (auto const itr = GetBuildIterator(build); itr != m_builds.end())
    {
        if (auto dest = GetBuildIterator(after); dest != m_builds.end())
        {
            m_builds.insert(++dest, *itr);
            m_builds.erase(itr);
            m_needsSave = true;
        }
    }
}

void BuildStorage::MoveBuildToTop(Build const& build)
{
    if (auto const itr = GetBuildIterator(build); itr != m_builds.end())
    {
        m_builds.push_front(*itr);
        m_builds.erase(itr);
        m_needsSave = true;
    }
}

void BuildStorage::MoveBuildToBottom(Build const& build)
{
    if (auto const itr = GetBuildIterator(build); itr != m_builds.end())
    {
        m_builds.push_back(*itr);
        m_builds.erase(itr);
        m_needsSave = true;
    }
}

void BuildStorage::MoveBuildUp(Build const& build)
{
    if (auto const itr = GetBuildIterator(build); itr != m_builds.begin())
    {
        auto dest = itr;
        std::iter_swap(itr, --dest);
        m_needsSave = true;
    }
}

void BuildStorage::MoveBuildDown(Build const& build)
{
    if (auto const itr = GetBuildIterator(build); itr != --m_builds.end())
    {
        auto dest = itr;
        std::iter_swap(itr, ++dest);
        m_needsSave = true;
    }
}

Build& BuildStorage::EditBuild(Build const& build)
{
    if (IsEditingBuild())
        CancelBuildEdit();

    return *(m_editedBuild = build);
}

void BuildStorage::AcceptBuildEdit()
{
    if (!IsEditingBuild())
        return;

    *GetBuildIterator(*m_editedBuild) = *m_editedBuild;
    m_editedBuild.reset();
}

void BuildStorage::CancelBuildEdit()
{
    if (!IsEditingBuild())
        return;

    m_editedBuild.reset();
}

bool BuildStorage::IsBuildMatchingFilter(Build const& build) const
{
    if (IsFilteringProfession() && build.GetParsedProfession() != m_currentProfession)
        return false;

    if (IsFilteringFlags() && !IsFilteringFlag(build.GetFlags()))
        return false;

    if (IsFilteringName() && build.GetNormalizedName().find(GetNormalizedNameFilter()) == std::string::npos)
        return false;

    return true;
}

void BuildStorage::Save(std::ofstream& file)
{
    file << "[Builds]\n";
    for (Build const& build : GetBuilds())
        file << fmt::format("={}|{}|{}|{}\n", 1, build.GetLink(), (uint32_t)build.GetFlags(), build.GetName());
    file << "\n[Filter]\n";
    file << fmt::format("Profession = {}\n", (uint32_t)m_professionFilter);
    file << fmt::format("Flags = {}\n", (uint32_t)m_flagsFilter);

    SetSaved();
}

bool BuildStorage::Load(std::string_view section, std::string_view name, std::string_view value)
{
    if (section == "Builds")
    {
        Build& build = AddBuild();

        std::stringstream str { std::string { value } };
        std::array<char, 100> buffer { };

        uint32_t version;
        if (str.getline(buffer.data(), buffer.size(), '|'))
            version = std::strtoul(buffer.data(), nullptr, 0);
        else
        {
            build.SetName("<MALFORMED BUILD>");
            build.SetSaved();
            return false;
        }

        switch (version)  // NOLINT(hicpp-multiway-paths-covered)
        {
            case 1:
            {
                if (str.getline(buffer.data(), buffer.size(), '|'))
                    build.SetLink(buffer.data());
                if (str.getline(buffer.data(), buffer.size(), '|'))
                    build.ToggleFlag((Build::Flags)std::strtoul(buffer.data(), nullptr, 0), true);
                if (str.getline(buffer.data(), buffer.size()))
                    build.SetName(buffer.data());
                build.SetSaved();
                return true;
            }
            default:
            {
                build.SetName(fmt::format("<INVALID BUILD #{} VERSION: {}>", build.GetID(), version));
                build.SetSaved();
                return false;
            }
        }
    }
    if (section == "Filter")
    {
        if (name == "Profession")
            return m_professionFilter = (bool)util::from_string<uint32_t>(value), true;
        if (name == "Flags")
            return m_flagsFilter = (Build::Flags)util::from_string<uint32_t>(value), true;

        return false;
    }
    return false;
}
}
