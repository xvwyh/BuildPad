#pragma once
#include "Build.h"

namespace buildpad
{
class MumbleLink
{
public:
    struct MappedFile
    {
        uint32_t Version;
        uint32_t Tick;

        float AvatarPosition[3];
        float AvatarFront[3];
        float AvatarTop[3];
        wchar_t Name[256];
        float CameraPosition[3];
        float CameraFront[3];
        float CameraTop[3];
        wchar_t Identity[256];

        uint32_t ContextLength;

        unsigned char Context[256];
        wchar_t Description[2048];
    };

    ~MumbleLink();

    bool Update();

    [[nodiscard]] GW2::Profession GetProfession() const { return m_profession; }

private:
    using handle_t = void*;
    handle_t m_handle = nullptr;
    MappedFile* m_map = nullptr;
    GW2::Profession m_profession = GW2::Profession::None;
};
}
