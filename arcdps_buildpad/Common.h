#pragma once
#include <optional>
#include <variant>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <deque>
#include <algorithm>
#include <random>
#include <filesystem>
#include <chrono>
#include <mutex>
#include <shared_mutex>
#include <sstream>
#include <fstream>
#include <charconv>
#include <future>
#include "dep/fmt/format.h"
#include "dep/gsl/gsl"
#include "dep/imgui/imgui.h"

#define ZLIB_CONST
#define INI_ALLOW_INLINE_COMMENTS 0

using std::uintptr_t;
using std::uint64_t;
using std::uint32_t;
using std::uint16_t;
using std::uint8_t;
using std::int64_t;
using std::int32_t;
using std::int16_t;
using std::int8_t;
using namespace std::literals::chrono_literals;
using namespace std::literals::string_literals;
using namespace std::literals::string_view_literals;
namespace fs = std::filesystem;

constexpr uint32_t BITMASK(uint8_t bit) { return 1u << bit; }

template<typename T>
struct Singleton
{
    static T& Instance()
    {
        static T instance;
        return instance;
    }
};

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

namespace util
{

template<typename T, typename = std::enable_if_t<!std::is_same_v<T, float> || std::is_same_v<T, double> || std::is_same_v<T, long double>>>
T from_string(std::string_view str, int base = 10)
{
    T result { };
    if (!str.empty())
        std::from_chars(&*str.begin(), &*str.rbegin() + 1, result, base);
    return result;
}
template<typename T, typename = std::enable_if_t<std::is_same_v<T, float> || std::is_same_v<T, double> || std::is_same_v<T, long double>>>
T from_string(std::string_view str, std::chars_format format = std::chars_format::general)
{
    T result { };
    if (!str.empty())
        std::from_chars(&*str.begin(), &*str.rbegin() + 1, result, format);
    return result;
}

inline std::vector<std::string_view> split(std::string_view str, std::string_view delimiters = " ")
{
    std::vector<std::string_view> result;
    size_t first = 0;

    while (first < str.size())
    {
        auto const second = str.find_first_of(delimiters, first);

        if (first != second)
            result.emplace_back(str.substr(first, second - first));

        if (second == std::string_view::npos)
            break;

        first = second + 1;
    }

    return result;
}

inline bool starts_with(std::string_view str, std::string_view start) { return str.size() >= start.size() && str.substr(0, start.size()) == start; }
inline bool ends_with(std::string_view str, std::string_view end) { return str.size() >= end.size() && str.substr(str.size() - end.size(), end.size()) == end; }

template<size_t N = 200>
auto to_buffer(std::string_view str)
{
    std::array<char, N> buffer { };
    str.copy(buffer.data(), buffer.size());
    buffer[buffer.size() - 1] = '\0';
    return buffer;
}

template<typename T>
T reverse_bytes(T const& value)
{
    T result;
    std::byte const* in = (std::byte const*)&value;
    std::byte* out = (std::byte*)&result;
    for (size_t i = 0; i < sizeof(T); ++i)
        out[sizeof(T) - 1 - i] = in[i];
    return result;
}

template<class InputIt, class OutputIt, class Predicate, class Transform>
void transform_if(InputIt first, InputIt last, OutputIt dest, Predicate predicate, Transform transform)
{
    while (first != last)
    {
        if (predicate(*first))
            *dest++ = transform(*first);

        ++first;
    }
}

template<typename T>
T lerp(T a, T b, float t)
{
    return a + (b - a) * t;
}
}
