#pragma once
#include "Common.h"
#include "Handler.h"
#include "imgui/imgui_internal.h"

namespace ImGui
{
bool ImageButtonWithText(ImTextureID texId,
                         char const* label,
                         ImVec2 const& buttonSize = { 0, 0 },
                         ImVec2 const& imageSize = { 0, 0 },
                         ImVec2 const& uv0 = { 0, 0 },
                         ImVec2 const& uv1 = { 1, 1 },
                         int rounding_corners_flags = -1,
                         int frame_padding = 0, // -1
                         ImVec4 const& bg_col = { 0, 0, 0, 0 },
                         ImVec4 const& tint_col = { 1, 1, 1, 1 },
                         bool borders = false,
                         char const* rightSideLabel = nullptr)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImVec2 size = imageSize;
    if (size.x <= 0 && size.y <= 0) { size.x = size.y = GetTextLineHeightWithSpacing(); }
    else
    {
        if (size.x <= 0) size.x = size.y;
        else if (size.y <= 0) size.y = size.x;
        //size *= window->FontWindowScale * GetIO().FontGlobalScale;
    }

    ImGuiContext& g = *GImGui;
    ImGuiStyle const& style = g.Style;

    ImGuiID const id = window->GetID(label);
    ImVec2 const textSize = CalcTextSize(label, nullptr, true);
    bool const hasText = textSize.x > 0;

    float const innerSpacing = hasText ? (frame_padding >= 0 ? (float)frame_padding + 2 : style.ItemInnerSpacing.x) : 0.f;
    ImVec2 const padding = frame_padding >= 0 ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
    ImVec2 totalSizeWithoutPadding(size.x + innerSpacing + textSize.x, buttonSize.y > 0 ? buttonSize.y - padding.y * 2 : size.y > textSize.y ? size.y : textSize.y);
    if (buttonSize.x > 0)
        totalSizeWithoutPadding.x = buttonSize.x - padding.x * 2;
    else if (buttonSize.x < 0)
        totalSizeWithoutPadding.x = CalcItemWidth();
    ImRect const bb(window->DC.CursorPos, window->DC.CursorPos + totalSizeWithoutPadding + padding * 2);
    ImVec2 start = window->DC.CursorPos + padding;
    if (buttonSize.x > 0 && size.x < buttonSize.x && !hasText)
        start.x += (buttonSize.x - size.x) * .5f;
    if (buttonSize.y > 0 && size.y < buttonSize.y)
        start.y += (buttonSize.y - size.y) * .5f;
    else if (size.y < textSize.y)
        start.y += (textSize.y - size.y) * .5f;
    ImRect const image_bb(ImFloor(start), ImFloor(start + size));
    start = window->DC.CursorPos + padding;
    start.x += size.x + innerSpacing;
    if (size.y > textSize.y)
        start.y += (size.y - textSize.y) * .5f;
    ItemSize(bb);
    if (!ItemAdd(bb, &id))
        return false;

    bool hovered = false, held = false;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    // Render
    ImU32 const col = GetColorU32(hovered && held
                                ? ImGuiCol_ButtonActive
                                : hovered
                                ? ImGuiCol_ButtonHovered
                                : ImGuiCol_Button);
    //RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
    {
        window->DrawList->AddRectFilled(bb.Min, bb.Max, col, style.FrameRounding, rounding_corners_flags);
        if (borders || window->Flags & ImGuiWindowFlags_ShowBorders)
        {
            window->DrawList->AddRect(bb.Min + ImVec2(1, 1), bb.Max + ImVec2(1, 1), GetColorU32(ImGuiCol_BorderShadow), style.FrameRounding, rounding_corners_flags);
            window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(ImGuiCol_Border), style.FrameRounding, rounding_corners_flags);
        }
    }
    if (bg_col.w > 0.0f)
        window->DrawList->AddRectFilled(image_bb.Min, image_bb.Max, GetColorU32(bg_col));

    window->DrawList->AddImage(texId, image_bb.Min, image_bb.Max, uv0, uv1, GetColorU32(tint_col));

    if (textSize.x > 0)
    {
        ImRect clip { bb.Min, bb.Max };
        if (rightSideLabel)
        {
            size = CalcTextSize(rightSideLabel);
            ImRect const rbb { { std::max<float>(start.x, bb.Max.x - style.ItemInnerSpacing.x - size.x - style.ItemInnerSpacing.x), bb.Min.y }, bb.Max };
            ImVec4 color = ColorConvertU32ToFloat4(GetColorU32(ImGuiCol_Text));
            color.w /= 2;
            PushStyleColor(ImGuiCol_Text, color);
            RenderTextClipped({ rbb.Min.x + style.ItemInnerSpacing.x, rbb.Min.y + (rbb.GetHeight() - size.y) / 2 }, rbb.Max, rightSideLabel, nullptr, &size, { 0, 0 }, &rbb);
            PopStyleColor();

            clip.Max.x = rbb.Min.x;
        }

        RenderTextClipped(start, bb.Max, label, nullptr, &textSize, { 0, 0 }, &clip);
    }

    return pressed;
}
bool ImageButtonWithText(buildpad::TextureData const& icon,
                         char const* label,
                         ImVec2 const& buttonSize = { 0, 0 },
                         int rounding_corners_flags = -1,
                         int frame_padding = 0, // -1
                         ImVec4 const& bg_col = { 0, 0, 0, 0 },
                         ImVec4 const& tint_col = { 1, 1, 1, 1 })
{
    return ImageButtonWithText(icon.Texture, label, buttonSize, { buttonSize.y, buttonSize.y }, icon.GetUV0(), icon.GetUV1(), rounding_corners_flags, frame_padding, bg_col, tint_col);
}
bool CheckboxWithImage(ImTextureID texId,
                       const char* label,
                       bool* v,
                       ImVec2 const& uv0 = { 0, 0 },
                       ImVec2 const& uv1 = { 1, 1 },
                       ImVec4 const& tint_col = { 1, 1, 1, 1 })
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    const ImRect check_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(label_size.y + style.FramePadding.y * 2, label_size.y + style.FramePadding.y * 2));
    ItemSize(check_bb, style.FramePadding.y);

    ImRect total_bb = check_bb;
    if (label_size.x > 0)
        SameLine(0, style.ItemInnerSpacing.x);
    const ImRect text_bb(window->DC.CursorPos + ImVec2(0, style.FramePadding.y), window->DC.CursorPos + ImVec2(0, style.FramePadding.y) + label_size);
    if (label_size.x > 0)
    {
        ItemSize(ImVec2(text_bb.GetWidth(), check_bb.GetHeight()), style.FramePadding.y);
        total_bb = ImRect(ImMin(check_bb.Min, text_bb.Min), ImMax(check_bb.Max, text_bb.Max));
    }

    if (!ItemAdd(total_bb, &id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed)
        *v = !(*v);

    RenderFrame(check_bb.Min, check_bb.Max, GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), true, style.FrameRounding);
    if (*v)
    {
        const float check_sz = ImMin(check_bb.GetWidth(), check_bb.GetHeight());
        const float pad = ImMax(1.0f, (float)(int)(check_sz / 6.0f));
        window->DrawList->AddRectFilled(check_bb.Min + ImVec2(pad, pad), check_bb.Max - ImVec2(pad, pad), GetColorU32(ImGuiCol_CheckMark), style.FrameRounding);
    }

    window->DrawList->AddImage(texId, check_bb.Min, check_bb.Max, uv0, uv1, GetColorU32(tint_col));

    if (label_size.x > 0.0f)
        RenderText(text_bb.GetTL(), label);

    return pressed;
}
bool CheckboxImage(ImTextureID texId,
                   const char* label,
                   bool* v,
                   ImVec2 const& uv0,
                   ImVec2 const& uv1,
                   float opacity_off,
                   float opacity_off_hover,
                   float opacity_off_active,
                   float opacity_on,
                   float opacity_on_hover,
                   float opacity_on_active)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    const ImRect check_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(label_size.y + style.FramePadding.y * 2, label_size.y + style.FramePadding.y * 2));
    ItemSize(check_bb, style.FramePadding.y);

    ImRect total_bb = check_bb;
    if (label_size.x > 0)
        SameLine(0, style.ItemInnerSpacing.x);
    const ImRect text_bb(window->DC.CursorPos + ImVec2(0, style.FramePadding.y), window->DC.CursorPos + ImVec2(0, style.FramePadding.y) + label_size);
    if (label_size.x > 0)
    {
        ItemSize(ImVec2(text_bb.GetWidth(), check_bb.GetHeight()), style.FramePadding.y);
        total_bb = ImRect(ImMin(check_bb.Min, text_bb.Min), ImMax(check_bb.Max, text_bb.Max));
    }

    if (!ItemAdd(total_bb, &id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed)
        *v = !(*v);

    float a = buildpad::Handler::Instance().IsLessTransparentButtonsEnabled() ? ColorConvertU32ToFloat4(GetColorU32(ImGuiCol_WindowBg)).w / 0.75f : 1.0f;

    auto const floatToVec = [a](float v) { return ImVec4 { v, v, v, util::lerp(1.0f, v, std::clamp(a, 0.0f, 1.0f)) }; };

    window->DrawList->AddImage(texId, check_bb.Min, check_bb.Max, uv0, uv1, GetColorU32(held && hovered
        ? floatToVec(*v ? opacity_on_active : opacity_off_active)
        : hovered
        ? floatToVec(*v ? opacity_on_hover : opacity_off_hover)
        : floatToVec(*v ? opacity_on : opacity_off)));

    if (label_size.x > 0.0f)
        RenderText(text_bb.GetTL(), label);

    return pressed;
}
void HorizontalLine(float offset = 0.0f, float padding = 0.0f)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    float x1 = window->DC.CursorPos.x;
    float x2 = window->DC.CursorPos.x + GetContentRegionAvailWidth() - 1;
    if (!window->DC.GroupStack.empty())
        x1 += window->DC.IndentX;

    const ImRect bb(ImVec2(x1 + padding, window->DC.CursorPos.y + offset), ImVec2(x2 - padding, window->DC.CursorPos.y + offset + 1.0f));

    ImVec4 color = ColorConvertU32ToFloat4(GetColorU32(ImGuiCol_Border));
    color.w = std::max<float>(0.75f, color.w);
    window->DrawList->AddLine(bb.Min, ImVec2(bb.Max.x, bb.Min.y), ColorConvertFloat4ToU32(color));
}

bool FixedTreeNodeBehavior(ImGuiID id, ImGuiTreeNodeFlags flags, const char* label, const char* label_end = NULL)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const bool display_frame = (flags & ImGuiTreeNodeFlags_Framed) != 0;
    const ImVec2 padding = display_frame ? style.FramePadding : ImVec2(style.FramePadding.x, 0.0f);

    if (!label_end)
        label_end = FindRenderedTextEnd(label);
    const ImVec2 label_size = CalcTextSize(label, label_end, false);

    // We vertically grow up to current line height up the typical widget height.
    const float text_base_offset_y = ImMax(0.0f, window->DC.CurrentLineTextBaseOffset - padding.y); // Latch before ItemSize changes it
    const float frame_height = ImMax(ImMin(window->DC.CurrentLineHeight, g.FontSize + style.FramePadding.y * 2), label_size.y + padding.y * 2);
    ImRect bb = ImRect(window->DC.CursorPos, ImVec2(window->Pos.x + GetContentRegionMax().x, window->DC.CursorPos.y + frame_height));
    if (display_frame)
    {
        // Framed header expand a little outside the default padding
        bb.Min.x -= (float)(int)(window->WindowPadding.x*0.5f) - 1;
        bb.Max.x += (float)(int)(window->WindowPadding.x*0.5f) - 1;
    }

    const float text_offset_x = (g.FontSize + (display_frame ? padding.x * 3 : padding.x * 2));   // Collapser arrow width + Spacing
    const float text_width = g.FontSize + (label_size.x > 0.0f ? label_size.x + padding.x * 2 : 0.0f);   // Include collapser
    ItemSize(ImVec2(text_width, frame_height), text_base_offset_y);

    // For regular tree nodes, we arbitrary allow to click past 2 worth of ItemSpacing
    // (Ideally we'd want to add a flag for the user to specify we want want the hit test to be done up to the right side of the content or not)
    const ImRect interact_bb = display_frame ? bb : ImRect(bb.Min.x, bb.Min.y, bb.Min.x + text_width + style.ItemSpacing.x * 2, bb.Max.y);
    bool is_open = TreeNodeBehaviorIsOpen(id, flags);
    if (!ItemAdd(interact_bb, &id))
    {
        if (is_open && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
            TreePushRawID(id);
        return is_open;
    }

    // Flags that affects opening behavior:
    // - 0(default) ..................... single-click anywhere to open
    // - OpenOnDoubleClick .............. double-click anywhere to open
    // - OpenOnArrow .................... single-click on arrow to open
    // - OpenOnDoubleClick|OpenOnArrow .. single-click on arrow or double-click anywhere to open
    ImGuiButtonFlags button_flags = /*ImGuiButtonFlags_NoKeyModifiers | */((flags & ImGuiTreeNodeFlags_AllowOverlapMode) ? ImGuiButtonFlags_AllowOverlapMode : 0);
    if (flags & ImGuiTreeNodeFlags_OpenOnDoubleClick)
        button_flags |= ImGuiButtonFlags_PressedOnDoubleClick | ((flags & ImGuiTreeNodeFlags_OpenOnArrow) ? ImGuiButtonFlags_PressedOnClickRelease : 0);
    bool hovered, held, pressed = ButtonBehavior(interact_bb, id, &hovered, &held, button_flags);
    if (pressed && !(flags & ImGuiTreeNodeFlags_Leaf))
    {
        bool toggled = !(flags & (ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick));
        if (flags & ImGuiTreeNodeFlags_OpenOnArrow)
            toggled |= IsMouseHoveringRect(interact_bb.Min, ImVec2(interact_bb.Min.x + text_offset_x, interact_bb.Max.y));
        if (flags & ImGuiTreeNodeFlags_OpenOnDoubleClick)
            toggled |= g.IO.MouseDoubleClicked[0];
        if (toggled)
        {
            is_open = !is_open;
            window->DC.StateStorage->SetInt(id, is_open);
        }
    }
    if (flags & ImGuiTreeNodeFlags_AllowOverlapMode)
        SetItemAllowOverlap();

    // Render
    const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
    const ImVec2 text_pos = bb.Min + ImVec2(text_offset_x, padding.y + text_base_offset_y);
    if (display_frame)
    {
        // Framed type
        RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);
        RenderCollapseTriangle(bb.Min + padding + ImVec2(0.0f, text_base_offset_y), is_open, 1.0f);
        RenderTextClipped(text_pos, bb.Max, label, label_end, &label_size);
    }
    else
    {
        // Unframed typed for tree nodes
        if (hovered || (flags & ImGuiTreeNodeFlags_Selected))
            RenderFrame(bb.Min, bb.Max, col, false);

        if (flags & ImGuiTreeNodeFlags_Bullet)
            RenderBullet(bb.Min + ImVec2(text_offset_x * 0.5f, g.FontSize*0.50f + text_base_offset_y));
        else if (!(flags & ImGuiTreeNodeFlags_Leaf))
            RenderCollapseTriangle(bb.Min + ImVec2(padding.x, g.FontSize*0.15f + text_base_offset_y), is_open, 0.70f);
        RenderText(text_pos, label, label_end, false);
    }

    if (is_open && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
        TreePushRawID(id);
    return is_open;
}

// CollapsingHeader returns true when opened but do not indent nor push into the ID stack (because of the ImGuiTreeNodeFlags_NoTreePushOnOpen flag).
// This is basically the same as calling TreeNodeEx(label, ImGuiTreeNodeFlags_CollapsingHeader | ImGuiTreeNodeFlags_NoTreePushOnOpen). You can remove the _NoTreePushOnOpen flag if you want behavior closer to normal TreeNode().
bool FixedCollapsingHeader(const char* label, ImGuiTreeNodeFlags flags = 0)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    return FixedTreeNodeBehavior(window->GetID(label), flags | ImGuiTreeNodeFlags_CollapsingHeader | ImGuiTreeNodeFlags_NoTreePushOnOpen, label);
}

bool FixedCollapsingHeader(const char* label, bool* p_open, ImGuiTreeNodeFlags flags = 0)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    if (p_open && !*p_open)
        return false;

    ImGuiID id = window->GetID(label);
    bool is_open = FixedTreeNodeBehavior(id, flags | ImGuiTreeNodeFlags_CollapsingHeader | ImGuiTreeNodeFlags_NoTreePushOnOpen | (p_open ? ImGuiTreeNodeFlags_AllowOverlapMode : 0), label);
    if (p_open)
    {
        // Create a small overlapping close button // FIXME: We can evolve this into user accessible helpers to add extra buttons on title bars, headers, etc.
        ImGuiContext& g = *GImGui;
        float button_sz = g.FontSize * 0.5f;
        if (CloseButton(window->GetID((void*)(intptr_t)(id + 1)), ImVec2(ImMin(window->DC.LastItemRect.Max.x, window->ClipRect.Max.x) - g.Style.FramePadding.x - button_sz, window->DC.LastItemRect.Min.y + g.Style.FramePadding.y + button_sz), button_sz))
            *p_open = false;
    }

    return is_open;
}

bool Hyperlink(char const* text, wchar_t const* url)
{
    TextColored(ColorConvertU32ToFloat4(0xFFD66603), text);
    if (IsItemHovered())
    {
        ImRect const bb { GetItemRectMin(), GetItemRectMax() + ImVec2 { 0, 1 } };
        GetCurrentWindow()->DrawList->AddLine(bb.GetBL(), bb.GetBR(), 0xFFD66603);
    }
    if (IsItemClicked())
    {
        ShellExecute(nullptr, nullptr, url, nullptr, nullptr, SW_SHOW);
        return true;
    }
    return false;
}

template<typename... T>
void Tooltip(char const* text, T&&... args)
{
    BeginTooltip();
    SetWindowFontScale(buildpad::Handler::Instance().GetUIScale());
    Text("%s", fmt::format(text, args...).c_str());
    EndTooltip();
}

template<typename... T>
void TooltipWithHeader(char const* header, char const* sub, T&&... args)
{
    BeginTooltip();
    SetWindowFontScale(buildpad::Handler::Instance().GetUIScale());
    if (header)
        Text("%s", fmt::format(header, args...).c_str());
    if (sub)
    {
        ImVec4 color = ColorConvertU32ToFloat4(GetColorU32(ImGuiCol_Text));
        color.w /= 2;
        TextColored(color, "%s", fmt::format(sub, args...).c_str());
    }
    EndTooltip();
}

bool IsPopupOpenPublic(char const* str_id)
{
    ImGuiContext& g = *GImGui;
    return g.OpenPopupStack.Size > g.CurrentPopupStack.Size && g.OpenPopupStack[g.CurrentPopupStack.Size].PopupId == g.CurrentWindow->GetID(str_id);
}

static struct
{
    ImVec2 CursorPosPrevLine;
    ImVec2 CursorPos;
    ImVec2 CursorMaxPos;
    float PrevLineHeight;
    float PrevLineTextBaseOffset;
    float CurrentLineHeight;
    float CurrentLineTextBaseOffset;
} storedCursor;

ImVec2 StoreCursor()
{
    ImGuiWindow* window = GetCurrentWindow();
    storedCursor.CursorPosPrevLine = window->DC.CursorPosPrevLine;
    storedCursor.CursorPos = window->DC.CursorPos;
    storedCursor.CursorMaxPos = window->DC.CursorMaxPos;
    storedCursor.PrevLineHeight = window->DC.PrevLineHeight;
    storedCursor.PrevLineTextBaseOffset = window->DC.PrevLineTextBaseOffset;
    storedCursor.CurrentLineHeight = window->DC.CurrentLineHeight;
    storedCursor.CurrentLineTextBaseOffset = window->DC.CurrentLineTextBaseOffset;
    return GetCursorPos();
}

ImVec2 RestoreCursor()
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.CursorPosPrevLine = storedCursor.CursorPosPrevLine;
    window->DC.CursorPos = storedCursor.CursorPos;
    window->DC.CursorMaxPos = storedCursor.CursorMaxPos;
    window->DC.PrevLineHeight = storedCursor.PrevLineHeight;
    window->DC.PrevLineTextBaseOffset = storedCursor.PrevLineTextBaseOffset;
    window->DC.CurrentLineHeight = storedCursor.CurrentLineHeight;
    window->DC.CurrentLineTextBaseOffset = storedCursor.CurrentLineTextBaseOffset;
    return GetCursorPos();
}
}
