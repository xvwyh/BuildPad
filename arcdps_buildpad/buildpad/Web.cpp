#include "Web.h"
#include "Handler.h"
#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Easy.hpp>

namespace buildpad
{
Web::Web()
{
    m_renderCache = "./addons/arcdps/arcdps.buildpad/rendercache";
    try { create_directories(m_renderCache); }
    catch (...) { m_renderCache = ""; }
}

void Web::Request(std::string_view url, std::function<void(std::string_view data)>&& onSuccess, std::function<void(std::exception const& e)>&& onError, bool mainThreadCallback)
{
    //if (m_requests.find(url) != m_requests.end())
    //    return;

    m_requests.emplace_back(/*url, */std::async(std::launch::async, [this, url = std::string { url }, onSuccess = std::move(onSuccess), onError = std::move(onError), mainThreadCallback]() mutable
    {
        try
        {
            std::optional<fs::path> cache;
            if (!m_renderCache.empty() && util::ends_with(url, ".png"))
                if (auto const pos = url.find_last_of('/'); pos != std::string::npos)
                    cache = m_renderCache / url.substr(pos + 1);

            std::ostringstream response;
            if (cache && exists(*cache))
            {
                std::shared_lock lock { m_renderCacheLock };
                try
                {
                    response << std::ifstream { *cache, std::ifstream::in | std::ofstream::binary }.rdbuf();
                    response.flush();
                }
                catch (...) { }
            }

            std::string data = response.str();

            if (data.empty())
            {
                curlpp::Cleanup clean;
                curlpp::Easy request;
                request.setOpt(curlpp::options::Url(url));
                request.setOpt(curlpp::options::WriteStream(&response));
                request.setOpt(curlpp::options::Timeout(10));

                request.perform();

                data = response.str();
            }

            if (cache && !exists(*cache))
            {
                std::lock_guard lock { m_renderCacheLock };
                try { std::ofstream { *cache, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc } << data; }
                catch (...) { }
            }

            if (onSuccess)
            {
                if (mainThreadCallback)
                {
                    Handler::Instance().OnMainThread([onSuccess = std::move(onSuccess), onError = std::move(onError), data = std::move(data)]
                    {
                        try { onSuccess(data); }
                        catch (std::exception const& e) { if (onError) onError(e); }
                    });
                }
                else
                    onSuccess(data);
            }
        }
        catch (std::exception const& e)
        {
            if (onError)
            {
                if (mainThreadCallback)
                    Handler::Instance().OnMainThread([onError = std::move(onError), e] { onError(e); });
                else
                    onError(e);
            }
        }
    }));
}

void Web::RequestAny(std::vector<std::string> urls, std::function<void(std::string_view data)>&& onSuccess, std::function<void(std::exception const& e)>&& onError, bool mainThreadCallback)
{
    if (urls.empty())
    {
        if (onError)
        {
            if (mainThreadCallback)
                Handler::Instance().OnMainThread([onError = std::move(onError)] { onError(std::exception()); });
            else
                onError(std::exception());
        }
        return;
    }

    Request(urls.front(), std::move(onSuccess), [=](std::exception const&) mutable
    {
        if (!urls.empty())
            Instance().RequestAny({ urls.begin() + 1, urls.end() }, std::move(onSuccess), std::move(onError), mainThreadCallback);
    }, mainThreadCallback);
}

void Web::ResetRenderCache() const
{
    if (m_renderCache.empty())
        return;

    std::vector<fs::path> toRemove;
    for (auto const& entry : fs::directory_iterator { m_renderCache })
        if (entry.path().extension() == ".png")
            toRemove.push_back(entry.path());

    for (auto const& path : toRemove)
        try { fs::remove(path); }
        catch (...) { }
}
}
