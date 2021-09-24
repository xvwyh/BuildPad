#pragma once
#include "../Common.h"

struct IDirect3DTexture9;
struct ID3D11ShaderResourceView;

namespace buildpad
{
    using Time = std::chrono::milliseconds;
    union TextureID
    {
        IDirect3DTexture9* D3D9;
        ID3D11ShaderResourceView* D3D11;
        void* Raw = nullptr;

        operator ImTextureID() const { return Raw; }
    };
    struct TextureData
    {
        TextureID Texture;
        uint32_t Width;
        uint32_t Height;
        uint32_t TrimLeft = 0;
        uint32_t TrimTop = 0;
        uint32_t TrimRight = 0;
        uint32_t TrimBottom = 0;
        bool FlipH = false;
        bool FlipV = false;

        TextureData(TextureID texture, uint32_t width, uint32_t height) : Texture(texture), Width(width), Height(height) { }

        TextureData& Trim(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom)
        {
            TrimLeft = left;
            TrimTop = top;
            TrimRight = right;
            TrimBottom = bottom;
            return *this;
        }
        TextureData& Trim(uint32_t horizontal, uint32_t vertical) { return Trim(horizontal, vertical, horizontal, vertical); }
        TextureData& Trim(uint32_t edge) { return Trim(edge, edge); }
        TextureData& FlipHorizontally() { FlipH = true; return *this; }
        TextureData& FlipVertically() { FlipV = true; return *this; }

        [[nodiscard]] ImVec2 Size() const { return { (float)Width, (float)Height }; }
        [[nodiscard]] ImVec2 TrimmedSize() const { return { (float)Width - TrimLeft - TrimRight, (float)Height - TrimTop - TrimBottom }; }
        [[nodiscard]] ImVec2 GetUV0() const { return { FlipH ? 1.0f - TrimRight / (float)Width : 0.0f + TrimLeft  / (float)Width, FlipV ? 1.0f - TrimBottom / (float)Height : 0.0f + TrimTop    / (float)Height }; }
        [[nodiscard]] ImVec2 GetUV1() const { return { FlipH ? 0.0f + TrimLeft  / (float)Width : 1.0f - TrimRight / (float)Width, FlipV ? 0.0f + TrimTop    / (float)Height : 1.0f - TrimBottom / (float)Height }; }

        TextureData& operator=(std::optional<TextureData> const& texture) { if (texture) *this = *texture; return *this; }
        TextureData& operator=(std::optional<TextureData>&& texture)      { if (texture) *this = *texture; return *this; }
    };
}
