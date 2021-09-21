#include "StateIcon.h"
#include "API.h"

namespace buildpad
{
StateIcon::StateIcon(Handler::Icons const loadingIcon, Handler::Icons const errorIcon, Handler::Icons const altLoadingIcon, Handler::Icons const altErrorIcon)
    : StateIcon(Handler::Instance().GetIcon(loadingIcon),
                Handler::Instance().GetIcon(errorIcon),
                Handler::Instance().GetIcon(altLoadingIcon),
                Handler::Instance().GetIcon(altErrorIcon)) { }

void StateIcon::Download(std::string_view const url, std::function<TextureData(TextureData)> const& transform)
{
    API::Instance().DownloadIcon(*this, url, transform);
}
}
