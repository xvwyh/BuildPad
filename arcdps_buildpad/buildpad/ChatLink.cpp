#include "ChatLink.h"
#include <base64/base64.h>

namespace buildpad
{
std::string ChatLink::BuildTemplateV2::Encode() const
{
    uint8_t const weaponsCount = std::min<uint8_t>(MAX_WEAPONS, Weapons.size());
    uint8_t const weaponSkillsCount = std::min<uint8_t>(MAX_WEAPON_SKILLS, WeaponSkills.size());

    // Precalculate payload size
    uint8_t length = sizeof BuildTemplateV1;
    if (weaponsCount || weaponSkillsCount)
        length += 1 + weaponsCount * 2;
    if (weaponsCount || weaponSkillsCount)
        length += 1 + weaponSkillsCount * 4;

    // Write payload
    std::vector<uint8_t> payload(length);
    uint8_t* p = payload.data();
    // V1
    *((BuildTemplateV1*&)p)++ = (BuildTemplateV1)*this;
    // V2
    if (weaponsCount || weaponSkillsCount)
    {
        *p++ = weaponsCount;
        for (uint8_t i = 0; i < weaponsCount; ++i)
            *((uint16_t*&)p)++ = (uint16_t)Weapons[i];
    }
    if (weaponsCount || weaponSkillsCount)
    {
        *p++ = weaponSkillsCount;
        for (uint8_t i = 0; i < weaponSkillsCount; ++i)
            *((uint32_t*&)p)++ = (uint16_t)WeaponSkills[i];
    }

    return ChatLink::Encode(payload.data(), payload.size());
}

bool ChatLink::BuildTemplateV2::Decode(std::span<uint8_t> const& data)
{
    if (data.size() < sizeof BuildTemplateV1)
        return false;

    if (data[0] != 0x0D)
        return false;

    uint8_t const* p = data.data();
    // V1
    *(BuildTemplateV1*)this = *((BuildTemplateV1*&)p)++;
    // V2
    if (data.size() > sizeof BuildTemplateV1)
    {
        class ReadOutOfRange : public std::exception { };
        auto read = [&data, &p, v2PayloadStart = p]<typename T>(std::optional<T> def = std::nullopt) -> T
        {
            if (data.size() < sizeof BuildTemplateV1 + p - v2PayloadStart + sizeof T)
            {
                if (def.has_value())
                    return def.value();

                throw ReadOutOfRange();
            }

            return *((T const*&)p)++;
        };

        try
        {
            Weapons.clear();
            WeaponSkills.clear();

            uint8_t const weaponsCount = read.operator()<uint8_t>();
            if (weaponsCount > MAX_WEAPONS)
                return false;
            for (uint8_t i = 0; i < weaponsCount; ++i)
                Weapons.push_back((GW2::Weapon)read.operator()<uint16_t>());

            uint8_t const weaponSkillCount = read.operator()<uint8_t>(0);
            for (uint8_t i = 0; i < weaponSkillCount; ++i)
                WeaponSkills.push_back(read.operator()<uint32_t>());
        }
        catch (ReadOutOfRange const&)
        {
            return false;
        }
    }

    return true;
}

std::string ChatLink::Encode(unsigned char const* data, size_t length)
{
    return fmt::format("[&{}]", base64_encode(data, (unsigned int)length));
}

std::string ChatLink::Encode(link_t const& link)
{
    return std::visit(overloaded
    {
        [](BuildTemplate const& link)                   { return link.Encode(); },
        [](ArcDPSCode<ArcDPSSkillTemplate> const& link) { return fmt::format("[*{}]", base64_encode(reinterpret_cast<unsigned char const*>(&link), sizeof(decltype(link)))); },
        [](ArcDPSCode<ArcDPSTraitTemplate> const& link) { return fmt::format("[*{}]", base64_encode(reinterpret_cast<unsigned char const*>(&link), sizeof(decltype(link)))); },
    }, link);
}

std::optional<ChatLink::link_t> ChatLink::Decode(std::string_view code)
{
    link_t result;
    {
        auto const start = code.find_first_of('[');
        auto const end = code.find_first_of(']', start);
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
                return result.Decode(data);
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
