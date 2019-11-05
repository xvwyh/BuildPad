#pragma once
#include "Defines.h"

namespace buildpad
{
struct KeyBind
{
    using key_t = uint8_t;

    bool Control = false;
    bool Alt = false;
    bool Shift = false;
    key_t Key { };
    bool PassThrough = false;

    KeyBind() = default;
    explicit KeyBind(std::string_view const str) { FromString(str); }

    [[nodiscard]] bool IsPressed() const;
    bool FromString(std::string_view str);
    [[nodiscard]] std::optional<std::string> ToString() const;

    operator bool() const { return Key; }
    bool operator==(KeyBind const& other) const
    {
        return Control == other.Control
            && Alt == other.Alt
            && Shift == other.Shift
            && Key == other.Key
            && PassThrough == other.PassThrough;
    }

    static std::optional<std::string> KeyToString(key_t key);
    static std::map<key_t, std::string_view> const& GetKeyMap();
};
}
