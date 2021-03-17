#include "MumbleLink.h"
#include <json/json.hpp>
#include <windows.h>

namespace buildpad
{
MumbleLink::~MumbleLink()
{
    if (m_map)
        UnmapViewOfFile(std::exchange(m_map, nullptr));
    if (m_handle)
        CloseHandle(std::exchange(m_handle, nullptr));
}

bool MumbleLink::Update()
{
    try
    {
        if (!m_handle && !(m_handle = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, sizeof(MappedFile), GetMumbleLinkName().c_str())))
            return false;

        if (!m_map && !(m_map = (MappedFile*)MapViewOfFile(m_handle, FILE_MAP_READ, 0, 0, sizeof(MappedFile))))
            return false;
    }
    catch (...)
    {
        m_handle = nullptr;
        m_map = nullptr;
    }

    try
    {
        std::string identity(std::size(m_map->Identity) * 4, '\0');
        WideCharToMultiByte(CP_UTF8, 0, m_map->Identity, -1, identity.data(), (int)identity.size(), nullptr, nullptr);
        if (identity.empty() || !identity[0])
        {
            m_profession = GW2::Profession::None;
            return true;
        }
        nlohmann::json json = nlohmann::json::parse(identity.begin(), identity.end(), nullptr, false);
        m_profession = (GW2::Profession)json["profession"].get<int>();
    }
    catch (...) { }

    return true;
}

std::wstring const& MumbleLink::GetMumbleLinkName()
{
    if (m_nameCache.empty())
    {
        m_nameCache = L"MumbleLink";

        try
        {
            static std::wstring const command = L"-mumble";
            std::wstring commandLine = GetCommandLine();
            if (auto const index = commandLine.find(command); index != std::wstring::npos && index + command.size() < commandLine.length() && commandLine[index + command.size()] == ' ')
            {
                auto start = index + command.size() + 1;
                decltype(start) end;
                if (commandLine[start] == '"')
                {
                    ++start;
                    end = commandLine.find('"', start);
                }
                else
                    end = commandLine.find(' ', start);
                m_nameCache = commandLine.substr(start, (end != std::wstring::npos ? end : commandLine.length()) - start);
            }
        }
        catch (...) { }
    }

    return m_nameCache;
}
}
