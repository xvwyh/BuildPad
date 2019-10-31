#include "ChatLink.h"
#include <base64/base64.h>

namespace buildpad
{
std::string ChatLink::Encode(unsigned char const* data, size_t length)
{
    return fmt::format("[&{}]", base64_encode(data, (unsigned int)length));
}

std::string ChatLink::Encode(link_t const& link)
{
    return std::visit(overloaded
    {
        [](BuildTemplate const& link)                   { return fmt::format("[&{}]", base64_encode(reinterpret_cast<unsigned char const*>(&link), sizeof(decltype(link)))); },
        [](ArcDPSCode<ArcDPSSkillTemplate> const& link) { return fmt::format("[*{}]", base64_encode(reinterpret_cast<unsigned char const*>(&link), sizeof(decltype(link)))); },
        [](ArcDPSCode<ArcDPSTraitTemplate> const& link) { return fmt::format("[*{}]", base64_encode(reinterpret_cast<unsigned char const*>(&link), sizeof(decltype(link)))); },
    }, link);
}

std::optional<ChatLink::link_t> ChatLink::Decode(std::string_view code)
{
    link_t result;
    {
        auto const start = code.find_first_of('[');
        auto const end = code.find_last_of(']');
        if (start == std::string::npos || end == std::string::npos)
            return { };
        code = code.substr(start, end - start + 1);
        assert(util::starts_with(code, "[") && util::ends_with(code, "]"));
    }

    try
    {
        std::vector<unsigned char> data = base64_decode(code.substr(2, code.size() - 3));
        if (data.empty())
            return { };

        switch (code[1])
        {
            case '&': result = BuildTemplate { }; break;
            case '*':
                if (data[0] == 's')
                    result = ArcDPSCode<ArcDPSSkillTemplate> { };
                else if (data[0] == 't')
                    result = ArcDPSCode<ArcDPSTraitTemplate> { };
                else
            default: return { };
        }
        if (!std::visit(overloaded
        {
            [&data](BuildTemplate& result)
            {
                if (data.size() != sizeof result)
                    return false;

                if (data[0] != 0x0D)
                    return false;

                result = *reinterpret_cast<std::remove_reference_t<decltype(result)>*>(data.data());
                return true;
            },
            [&data](ArcDPSCode<ArcDPSSkillTemplate>& result)
            {
                if (data.size() != sizeof result)
                    return false;

                result = *reinterpret_cast<std::remove_reference_t<decltype(result)>*>(data.data());
                return true;
            },
            [&data](ArcDPSCode<ArcDPSTraitTemplate>& result)
            {
                if (data.size() != sizeof result)
                    return false;

                result = *reinterpret_cast<std::remove_reference_t<decltype(result)>*>(data.data());
                return true;
            },
        }, result))
            return { };
    }
    catch (...) { return { }; }

    return result;
}
}
