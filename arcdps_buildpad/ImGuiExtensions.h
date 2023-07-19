#pragma once
#include "Common.h"
#include "Handler.h"
#include "imgui/imgui_internal.h"

namespace ImGui
{
[[nodiscard]] ImVec4 GetStyleColorVec4ModColor(ImGuiCol idx, float colorMultiplier)
{
    ImVec4 color = GetStyleColorVec4(idx);
    color.x *= colorMultiplier;
    color.y *= colorMultiplier;
    color.z *= colorMultiplier;
    return color;
}
[[nodiscard]] ImVec4 GetStyleColorVec4ModAlpha(ImGuiCol idx, float alphaMultiplier)
{
    ImVec4 color = GetStyleColorVec4(idx);
    color.w *= alphaMultiplier;
    return color;
}
[[nodiscard]] ImVec4 GetStyleColorVec4Grayscale(ImGuiCol idx)
{
    ImVec4 color = GetStyleColorVec4(idx);
    color.x = color.y = color.z = (color.x + color.y + color.z) / 3;
    return color;
}

bool ImageButtonWithText(ImTextureID texId,
                         char const* label,
                         ImVec2 const& buttonSize = { 0, 0 },
                         ImVec2 const& imageSize = { 0, 0 },
                         ImVec2 const& uv0 = { 0, 0 },
                         ImVec2 const& uv1 = { 1, 1 },
                         ImDrawCornerFlags_ rounding_corners_flags = ImDrawCornerFlags_All,
                         int frame_padding = 0, // -1
                         ImVec4 const& bg_col = { 0, 0, 0, 0 },
                         ImVec4 const& tint_col = { 1, 1, 1, 1 },
                         bool borders = false,
                         char const* rightSideLabel = nullptr,
                         std::vector<buildpad::TextureData const*> additionalImages = { })
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
    ImRect image_bb(ImFloor(start), ImFloor(start + size));
    start = window->DC.CursorPos + padding;
    start.x += size.x * (1 + additionalImages.size()) + innerSpacing;
    if (size.y > textSize.y)
        start.y += (size.y - textSize.y) * .5f;
    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered = false, held = false;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    // Render
    ImU32 const col = GetColorU32(hovered && held
                                ? ImGuiCol_ButtonActive
                                : hovered
                                ? ImGuiCol_ButtonHovered
                                : ImGuiCol_Button);
    RenderNavHighlight(bb, id);
    //RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
    {
        window->DrawList->AddRectFilled(bb.Min, bb.Max, col, style.FrameRounding, rounding_corners_flags);
        const float border_size = g.Style.FrameBorderSize;
        if (borders && border_size > 0.0f)
        {
            window->DrawList->AddRect(bb.Min + ImVec2(1, 1), bb.Max + ImVec2(1, 1), GetColorU32(ImGuiCol_BorderShadow), style.FrameRounding, rounding_corners_flags, border_size);
            window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(ImGuiCol_Border), style.FrameRounding, rounding_corners_flags, border_size);
        }
    }
    if (bg_col.w > 0.0f)
        window->DrawList->AddRectFilled(image_bb.Min, image_bb.Max, GetColorU32(bg_col));

    window->DrawList->AddImage(texId, image_bb.Min, image_bb.Max, uv0, uv1, GetColorU32(tint_col));
    for (auto const& image : additionalImages)
    {
        image_bb.Min.x = std::exchange(image_bb.Max.x, image_bb.Max.x + image_bb.Max.x - image_bb.Min.x);
        window->DrawList->AddImage(image->Texture, image_bb.Min, image_bb.Max, image->GetUV0(), image->GetUV1(), GetColorU32(tint_col));
    }

    if (textSize.x > 0)
    {
        ImRect clip { bb.Min, bb.Max };
        if (rightSideLabel)
        {
            size = CalcTextSize(rightSideLabel);
            ImRect const rbb { { std::max<float>(start.x, bb.Max.x - style.ItemInnerSpacing.x - size.x - style.ItemInnerSpacing.x), bb.Min.y }, bb.Max };
            PushStyleColor(ImGuiCol_Text, GetStyleColorVec4ModAlpha(ImGuiCol_Text, 0.5f));
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
                         ImDrawCornerFlags_ rounding_corners_flags = ImDrawCornerFlags_All,
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

    if (!ItemAdd(total_bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed)
    {
        *v = !(*v);
        MarkItemEdited(id);
    }

    RenderNavHighlight(total_bb, id);
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

    if (!ItemAdd(total_bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed)
    {
        *v = !(*v);
        MarkItemEdited(id);
    }

    float a = buildpad::Handler::Instance().IsLessTransparentButtonsEnabled() ? GetStyleColorVec4(ImGuiCol_WindowBg).w / 0.75f : 1.0f;

    auto const floatToVec = [a](float v) { return ImVec4 { v, v, v, util::lerp(1.0f, v, std::clamp(a, 0.0f, 1.0f)) }; };

    RenderNavHighlight(total_bb, id);
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

    ImGuiContext& g = *GImGui;

    float x1 = window->DC.CursorPos.x;
    float x2 = window->DC.CursorPos.x + GetContentRegionAvailWidth() - 1;
    if (g.GroupStack.Size > 0 && g.GroupStack.back().WindowID == window->ID)
        x1 += window->DC.Indent.x;

    const ImRect bb(ImVec2(x1 + padding, window->DC.CursorPos.y + offset), ImVec2(x2 - padding, window->DC.CursorPos.y + offset + 1.0f));

    ImVec4 color = GetStyleColorVec4(ImGuiCol_Border);
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
    const ImVec2 padding = (display_frame || (flags & ImGuiTreeNodeFlags_FramePadding)) ? style.FramePadding : ImVec2(style.FramePadding.x, ImMin(window->DC.CurrLineTextBaseOffset, style.FramePadding.y));

    if (!label_end)
        label_end = FindRenderedTextEnd(label);
    const ImVec2 label_size = CalcTextSize(label, label_end, false);

    // We vertically grow up to current line height up the typical widget height.
    const float frame_height = ImMax(ImMin(window->DC.CurrLineSize.y, g.FontSize + style.FramePadding.y * 2), label_size.y + padding.y * 2);
    ImRect frame_bb;
    frame_bb.Min.x = (flags & ImGuiTreeNodeFlags_SpanFullWidth) ? window->WorkRect.Min.x : window->DC.CursorPos.x;
    frame_bb.Min.y = window->DC.CursorPos.y;
    frame_bb.Max.x = window->WorkRect.Max.x;
    frame_bb.Max.y = window->DC.CursorPos.y + frame_height;
    if (display_frame)
    {
        // Framed header expand a little outside the default padding, to the edge of InnerClipRect
        // (FIXME: May remove this at some point and make InnerClipRect align with WindowPadding.x instead of WindowPadding.x*0.5f)
        frame_bb.Min.x -= IM_FLOOR(window->WindowPadding.x * 0.5f - 1.0f);
        frame_bb.Max.x += IM_FLOOR(window->WindowPadding.x * 0.5f);
    }

    const float text_offset_x = g.FontSize + (display_frame ? padding.x * 3 : padding.x * 2);           // Collapser arrow width + Spacing
    const float text_offset_y = ImMax(padding.y, window->DC.CurrLineTextBaseOffset);                    // Latch before ItemSize changes it
    const float text_width = g.FontSize + (label_size.x > 0.0f ? label_size.x + padding.x * 2 : 0.0f);  // Include collapser
    ImVec2 text_pos(window->DC.CursorPos.x + text_offset_x, window->DC.CursorPos.y + text_offset_y);
    ItemSize(ImVec2(text_width, frame_height), padding.y);

    // For regular tree nodes, we arbitrary allow to click past 2 worth of ItemSpacing
    ImRect interact_bb = frame_bb;
    if (!display_frame && (flags & (ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth)) == 0)
        interact_bb.Max.x = frame_bb.Min.x + text_width + style.ItemSpacing.x * 2.0f;

    // Store a flag for the current depth to tell if we will allow closing this node when navigating one of its child.
    // For this purpose we essentially compare if g.NavIdIsAlive went from 0 to 1 between TreeNode() and TreePop().
    // This is currently only support 32 level deep and we are fine with (1 << Depth) overflowing into a zero.
    const bool is_leaf = (flags & ImGuiTreeNodeFlags_Leaf) != 0;
    bool is_open = TreeNodeBehaviorIsOpen(id, flags);
    if (is_open && !g.NavIdIsAlive && (flags & ImGuiTreeNodeFlags_NavLeftJumpsBackHere) && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
        window->DC.TreeJumpToParentOnPopMask |= (1 << window->DC.TreeDepth);

    bool item_add = ItemAdd(interact_bb, id);
    window->DC.LastItemStatusFlags |= ImGuiItemStatusFlags_HasDisplayRect;
    window->DC.LastItemDisplayRect = frame_bb;

    if (!item_add)
    {
        if (is_open && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
            TreePushOverrideID(id);
        IMGUI_TEST_ENGINE_ITEM_INFO(window->DC.LastItemId, label, window->DC.ItemFlags | (is_leaf ? 0 : ImGuiItemStatusFlags_Openable) | (is_open ? ImGuiItemStatusFlags_Opened : 0));
        return is_open;
    }

    ImGuiButtonFlags button_flags = ImGuiTreeNodeFlags_None;
    if (flags & ImGuiTreeNodeFlags_AllowItemOverlap)
        button_flags |= ImGuiButtonFlags_AllowItemOverlap;
    if (!is_leaf)
        button_flags |= ImGuiButtonFlags_PressedOnDragDropHold;

    // We allow clicking on the arrow section with keyboard modifiers held, in order to easily
    // allow browsing a tree while preserving selection with code implementing multi-selection patterns.
    // When clicking on the rest of the tree node we always disallow keyboard modifiers.
    const float arrow_hit_x1 = (text_pos.x - text_offset_x) - style.TouchExtraPadding.x;
    const float arrow_hit_x2 = (text_pos.x - text_offset_x) + (g.FontSize + padding.x * 2.0f) + style.TouchExtraPadding.x;
    const bool is_mouse_x_over_arrow = (g.IO.MousePos.x >= arrow_hit_x1 && g.IO.MousePos.x < arrow_hit_x2);
//    if (window != g.HoveredWindow || !is_mouse_x_over_arrow)
//        button_flags |= ImGuiButtonFlags_NoKeyModifiers;

    // Open behaviors can be altered with the _OpenOnArrow and _OnOnDoubleClick flags.
    // Some alteration have subtle effects (e.g. toggle on MouseUp vs MouseDown events) due to requirements for multi-selection and drag and drop support.
    // - Single-click on label = Toggle on MouseUp (default, when _OpenOnArrow=0)
    // - Single-click on arrow = Toggle on MouseDown (when _OpenOnArrow=0)
    // - Single-click on arrow = Toggle on MouseDown (when _OpenOnArrow=1)
    // - Double-click on label = Toggle on MouseDoubleClick (when _OpenOnDoubleClick=1)
    // - Double-click on arrow = Toggle on MouseDoubleClick (when _OpenOnDoubleClick=1 and _OpenOnArrow=0)
    // It is rather standard that arrow click react on Down rather than Up.
    // We set ImGuiButtonFlags_PressedOnClickRelease on OpenOnDoubleClick because we want the item to be active on the initial MouseDown in order for drag and drop to work.
    if (is_mouse_x_over_arrow)
        button_flags |= ImGuiButtonFlags_PressedOnClick;
    else if (flags & ImGuiTreeNodeFlags_OpenOnDoubleClick)
        button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick;
    else
        button_flags |= ImGuiButtonFlags_PressedOnClickRelease;

    bool selected = (flags & ImGuiTreeNodeFlags_Selected) != 0;
    const bool was_selected = selected;

    bool hovered, held;
    bool pressed = ButtonBehavior(interact_bb, id, &hovered, &held, button_flags);
    bool toggled = false;
    if (!is_leaf)
    {
        if (pressed && g.DragDropHoldJustPressedId != id)
        {
            if ((flags & (ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick)) == 0 || (g.NavActivateId == id))
                toggled = true;
            if (flags & ImGuiTreeNodeFlags_OpenOnArrow)
                toggled |= is_mouse_x_over_arrow && !g.NavDisableMouseHover; // Lightweight equivalent of IsMouseHoveringRect() since ButtonBehavior() already did the job
            if ((flags & ImGuiTreeNodeFlags_OpenOnDoubleClick) && g.IO.MouseDoubleClicked[0])
                toggled = true;
        }
        else if (pressed && g.DragDropHoldJustPressedId == id)
        {
            IM_ASSERT(button_flags & ImGuiButtonFlags_PressedOnDragDropHold);
            if (!is_open) // When using Drag and Drop "hold to open" we keep the node highlighted after opening, but never close it again.
                toggled = true;
        }

        if (g.NavId == id && g.NavMoveRequest && g.NavMoveDir == ImGuiDir_Left && is_open)
        {
            toggled = true;
            NavMoveRequestCancel();
        }
        if (g.NavId == id && g.NavMoveRequest && g.NavMoveDir == ImGuiDir_Right && !is_open) // If there's something upcoming on the line we may want to give it the priority?
        {
            toggled = true;
            NavMoveRequestCancel();
        }

        if (toggled)
        {
            is_open = !is_open;
            window->DC.StateStorage->SetInt(id, is_open);
            window->DC.LastItemStatusFlags |= ImGuiItemStatusFlags_ToggledOpen;
        }
    }
    if (flags & ImGuiTreeNodeFlags_AllowItemOverlap)
        SetItemAllowOverlap();

    // In this branch, TreeNodeBehavior() cannot toggle the selection so this will never trigger.
    if (selected != was_selected) //-V547
        window->DC.LastItemStatusFlags |= ImGuiItemStatusFlags_ToggledSelection;

    // Render
    const ImU32 text_col = GetColorU32(ImGuiCol_Text);
    ImGuiNavHighlightFlags nav_highlight_flags = ImGuiNavHighlightFlags_TypeThin;
    if (display_frame)
    {
        // Framed type
        const ImU32 bg_col = GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
        RenderFrame(frame_bb.Min, frame_bb.Max, bg_col, true, style.FrameRounding);
        RenderNavHighlight(frame_bb, id, nav_highlight_flags);
        if (flags & ImGuiTreeNodeFlags_Bullet)
            RenderBullet(window->DrawList, ImVec2(text_pos.x - text_offset_x * 0.60f, text_pos.y + g.FontSize * 0.5f), text_col);
        else if (!is_leaf)
            RenderArrow(window->DrawList, ImVec2(text_pos.x - text_offset_x + padding.x, text_pos.y), text_col, is_open ? ImGuiDir_Down : ImGuiDir_Right, 1.0f);
        else // Leaf without bullet, left-adjusted text
            text_pos.x -= text_offset_x;
        if (flags & ImGuiTreeNodeFlags_ClipLabelForTrailingButton)
            frame_bb.Max.x -= g.FontSize + style.FramePadding.x;
        if (g.LogEnabled)
        {
            // NB: '##' is normally used to hide text (as a library-wide feature), so we need to specify the text range to make sure the ## aren't stripped out here.
            const char log_prefix[] = "\n##";
            const char log_suffix[] = "##";
            LogRenderedText(&text_pos, log_prefix, log_prefix + 3);
            RenderTextClipped(text_pos, frame_bb.Max, label, label_end, &label_size);
            LogRenderedText(&text_pos, log_suffix, log_suffix + 2);
        }
        else
        {
            RenderTextClipped(text_pos, frame_bb.Max, label, label_end, &label_size);
        }
    }
    else
    {
        // Unframed typed for tree nodes
        if (hovered || selected)
        {
            const ImU32 bg_col = GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
            RenderFrame(frame_bb.Min, frame_bb.Max, bg_col, false);
            RenderNavHighlight(frame_bb, id, nav_highlight_flags);
        }
        if (flags & ImGuiTreeNodeFlags_Bullet)
            RenderBullet(window->DrawList, ImVec2(text_pos.x - text_offset_x * 0.5f, text_pos.y + g.FontSize * 0.5f), text_col);
        else if (!is_leaf)
            RenderArrow(window->DrawList, ImVec2(text_pos.x - text_offset_x + padding.x, text_pos.y + g.FontSize * 0.15f), text_col, is_open ? ImGuiDir_Down : ImGuiDir_Right, 0.70f);
        if (g.LogEnabled)
            LogRenderedText(&text_pos, ">");
        RenderText(text_pos, label, label_end, false);
    }

    if (is_open && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
        TreePushOverrideID(id);
    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags | (is_leaf ? 0 : ImGuiItemStatusFlags_Openable) | (is_open ? ImGuiItemStatusFlags_Opened : 0));
    return is_open;
}

// CollapsingHeader returns true when opened but do not indent nor push into the ID stack (because of the ImGuiTreeNodeFlags_NoTreePushOnOpen flag).
// This is basically the same as calling TreeNodeEx(label, ImGuiTreeNodeFlags_CollapsingHeader). You can remove the _NoTreePushOnOpen flag if you want behavior closer to normal TreeNode().
bool FixedCollapsingHeader(const char* label, ImGuiTreeNodeFlags flags = 0)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    return FixedTreeNodeBehavior(window->GetID(label), flags | ImGuiTreeNodeFlags_CollapsingHeader, label);
}

// p_visible == NULL                        : regular collapsing header
// p_visible != NULL && *p_visible == true  : show a small close button on the corner of the header, clicking the button will set *p_visible = false
// p_visible != NULL && *p_visible == false : do not show the header at all
// Do not mistake this with the Open state of the header itself, which you can adjust with SetNextItemOpen() or ImGuiTreeNodeFlags_DefaultOpen.
bool FixedCollapsingHeader(const char* label, bool* p_visible, ImGuiTreeNodeFlags flags = 0)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    if (p_visible && !*p_visible)
        return false;

    ImGuiID id = window->GetID(label);
    flags |= ImGuiTreeNodeFlags_CollapsingHeader;
    if (p_visible)
        flags |= ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_ClipLabelForTrailingButton;
    bool is_open = FixedTreeNodeBehavior(id, flags, label);
    if (p_visible != NULL)
    {
        // Create a small overlapping close button
        // FIXME: We can evolve this into user accessible helpers to add extra buttons on title bars, headers, etc.
        // FIXME: CloseButton can overlap into text, need find a way to clip the text somehow.
        ImGuiContext& g = *GImGui;
        ImGuiLastItemDataBackup last_item_backup;
        float button_size = g.FontSize;
        float button_x = ImMax(window->DC.LastItemRect.Min.x, window->DC.LastItemRect.Max.x - g.Style.FramePadding.x * 2.0f - button_size);
        float button_y = window->DC.LastItemRect.Min.y;
        ImGuiID close_button_id = GetIDWithSeed("#CLOSE", NULL, id);
        if (CloseButton(close_button_id, ImVec2(button_x, button_y)))
            *p_visible = false;
        last_item_backup.Restore();
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
        auto* window = GetCurrentWindow();
        PushTextWrapPos(std::max<float>(window->DC.CursorMaxPos.x - (window->Pos.x + window->DC.Indent.x + window->DC.ColumnsOffset.x), 200.0f * window->FontWindowScale));
        TextColored(GetStyleColorVec4ModAlpha(ImGuiCol_Text, 0.5f), "%s", fmt::format(sub, args...).c_str());
        PopTextWrapPos();
    }
    EndTooltip();
}

static struct
{
    ImVec2 CursorPosPrevLine;
    ImVec2 CursorPos;
    ImVec2 CursorMaxPos;
    ImVec2 IdealMaxPos;
    ImVec2 CurrLineSize;
    ImVec2 PrevLineSize;
    float CurrLineTextBaseOffset;
    float PrevLineTextBaseOffset;
} storedCursor;

ImVec2 StoreCursor()
{
    ImGuiWindow* window = GetCurrentWindow();
    storedCursor.CursorPosPrevLine = window->DC.CursorPosPrevLine;
    storedCursor.CursorPos = window->DC.CursorPos;
    storedCursor.CursorMaxPos = window->DC.CursorMaxPos;
    storedCursor.IdealMaxPos = window->DC.IdealMaxPos;
    storedCursor.CurrLineSize = window->DC.CurrLineSize;
    storedCursor.PrevLineSize = window->DC.PrevLineSize;
    storedCursor.CurrLineTextBaseOffset = window->DC.CurrLineTextBaseOffset;
    storedCursor.PrevLineTextBaseOffset = window->DC.PrevLineTextBaseOffset;
    return GetCursorPos();
}

ImVec2 RestoreCursor()
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.CursorPosPrevLine = storedCursor.CursorPosPrevLine;
    window->DC.CursorPos = storedCursor.CursorPos;
    window->DC.CursorMaxPos = storedCursor.CursorMaxPos;
    window->DC.IdealMaxPos = storedCursor.IdealMaxPos;
    window->DC.CurrLineSize = storedCursor.CurrLineSize;
    window->DC.PrevLineSize = storedCursor.PrevLineSize;
    window->DC.CurrLineTextBaseOffset = storedCursor.CurrLineTextBaseOffset;
    window->DC.PrevLineTextBaseOffset = storedCursor.PrevLineTextBaseOffset;
    return GetCursorPos();
}
}
