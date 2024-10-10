#pragma once
#include "Defines.h"

#define BUILDPAD_URL "https://buildpad.gw2archive.eu"
#define FALLBACK_URL "https://raw.githubusercontent.com/xvwyh/BuildPad/refs/heads/master/arcdps_buildpad"
#define URL_WITH_FALLBACK(path) BUILDPAD_URL path, FALLBACK_URL path

namespace buildpad
{
class Web : public Singleton<Web>
{
public:
    void Request(std::string_view url, std::function<void(std::string_view data)>&& onSuccess, std::function<void(std::exception const& e)>&& onError = { }, bool mainThreadCallback = true);
    void RequestAny(std::vector<std::string> urls, std::function<void(std::string_view data)>&& onSuccess, std::function<void(std::exception const& e)>&& onError = { }, bool mainThreadCallback = true);
    void ResetRenderCache() const;

private:
    //std::map<std::string, std::future<void>, std::less<>> m_requests;
    std::list<std::future<void>> m_requests;
    fs::path m_renderCache;
    std::shared_mutex m_renderCacheLock;

    Web();
    friend struct Singleton<Web>;
};
}
