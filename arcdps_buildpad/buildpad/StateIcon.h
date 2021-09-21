#pragma once
#include "Defines.h"
#include "Handler.h"

namespace buildpad
{
class StateIcon
{
public:
    enum class States : uint8_t
    {
        Loading,
        Loaded,
        Error,
    };

    StateIcon(Handler::Icons loadingIcon, Handler::Icons errorIcon) : StateIcon(loadingIcon, errorIcon, loadingIcon, errorIcon) { }
    StateIcon(Handler::Icons loadingIcon, Handler::Icons errorIcon, Handler::Icons altLoadingIcon, Handler::Icons altErrorIcon);
    StateIcon(TextureData const& loadingIcon, TextureData const& errorIcon) : StateIcon(loadingIcon, errorIcon, loadingIcon, errorIcon) { }
    StateIcon(TextureData const& loadingIcon, TextureData const& errorIcon, TextureData const& altLoadingIcon, TextureData const& altErrorIcon) : m_loadingIcon(loadingIcon), m_errorIcon(errorIcon), m_altLoadingIcon(altLoadingIcon), m_altErrorIcon(altErrorIcon) { }

    [[nodiscard]] TextureData const& Get() const
    {
        switch (m_state)
        {
            case States::Loading:
                return m_alt ? m_altLoadingIcon : m_loadingIcon;
            case States::Loaded:
                return *m_icon;
            case States::Error:
                return m_alt ? m_altErrorIcon : m_errorIcon;
            default:
                return m_errorIcon;
        }
    }
    void Set(TextureData icon, bool unloadable)
    {
        m_icon = icon;
        m_unloadable = unloadable;
        m_state = States::Loaded;
    }
    void SetAlt(bool alt)
    {
        m_alt = alt;
    }
    void SetError()
    {
        m_state = States::Error;
    }

    void Download(std::string_view url, std::function<TextureData(TextureData)> const& transform = {});
    void Unload()
    {
        if (m_unloadable && m_icon)
        {
            Handler::Instance().UnloadTexture(*m_icon);
            m_icon.reset();
            m_unloadable = false;
            m_state = States::Error;
        }
    }

private:
    bool m_unloadable = false;
    bool m_alt = false;
    States m_state = States::Loading;
    std::optional<TextureData> m_icon;
    TextureData const& m_loadingIcon;
    TextureData const& m_errorIcon;
    TextureData const& m_altLoadingIcon;
    TextureData const& m_altErrorIcon;
};
}
