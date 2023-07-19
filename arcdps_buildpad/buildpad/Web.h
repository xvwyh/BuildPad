#pragma once
#include "Defines.h"

namespace buildpad
{
class Web : public Singleton<Web>
{
public:
    void Request(std::string_view url, std::function<void(std::string_view data)>&& onSuccess, std::function<void(std::exception const& e)>&& onError = { }, bool mainThreadCallback = true);
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
