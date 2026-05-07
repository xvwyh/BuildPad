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
    using SuccessCallback = std::function<void(std::string_view data)>;
    using ErrorCallback = std::function<void(std::exception const& e)>;

    void Request(std::string_view url, SuccessCallback onSuccess, ErrorCallback onError = { }, bool mainThreadCallback = true);
    bool RequestSync(std::string_view url, SuccessCallback onSuccess, ErrorCallback onError = { }, bool mainThreadCallback = false);
    void RequestAny(std::vector<std::string> urls, SuccessCallback onSuccess, ErrorCallback onError = { }, bool mainThreadCallback = true);
    bool RequestAnySync(std::vector<std::string> urls, SuccessCallback const& onSuccess, ErrorCallback onError = { }, bool mainThreadCallback = false);
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
