#pragma once
#include "Common.h"
#include "Handler.h"
#include "imgui/imgui_internal.h"

void TreeNodeStoreStackData(ImGuiTreeNodeFlags flags, float x1);
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

bool ImageButtonWithText(ImTextureRef texId,
                         char const* label,
                         ImVec2 const& buttonSize = { 0, 0 },
                         ImVec2 const& imageSize = { 0, 0 },
                         ImVec2 const& uv0 = { 0, 0 },
                         ImVec2 const& uv1 = { 1, 1 },
                         ImDrawFlags_ flags = ImDrawFlags_RoundCornersAll,
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
    ImRect const bb(window->DC.CursorPos, window->DC.CursorPos + totalSizeWithoutPadding + padding * 2.0f);
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
    //RenderFrame(bb.Min, bb.Max, col, true, g.Style.FrameRounding);
    {
        window->DrawList->AddRectFilled(bb.Min, bb.Max, col, style.FrameRounding, flags);
        const float border_size = g.Style.FrameBorderSize;
        if (borders && border_size > 0.0f)
        {
            window->DrawList->AddRect(bb.Min + ImVec2(1, 1), bb.Max + ImVec2(1, 1), GetColorU32(ImGuiCol_BorderShadow), style.FrameRounding, flags, border_size);
            window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(ImGuiCol_Border), style.FrameRounding, flags, border_size);
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
                         ImDrawFlags_ flags = ImDrawFlags_RoundCornersAll,
                         int frame_padding = 0, // -1
                         ImVec4 const& bg_col = { 0, 0, 0, 0 },
                         ImVec4 const& tint_col = { 1, 1, 1, 1 })
{
    return ImageButtonWithText(icon.Texture, label, buttonSize, { buttonSize.y, buttonSize.y }, icon.GetUV0(), icon.GetUV1(), flags, frame_padding, bg_col, tint_col);
}
bool CheckboxWithImage(ImTextureRef texId,
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

    const bool is_visible = ItemAdd(total_bb, id);

    // Range-Selection/Multi-selection support (header)
    bool checked = *v;

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);

    // Range-Selection/Multi-selection support (footer)
    if (pressed)
        checked = !checked;

    if (*v != checked)
    {
        *v = checked;
        pressed = true; // return value
        MarkItemEdited(id);
    }

    if (is_visible)
    {
        RenderNavCursor(total_bb, id);
        RenderFrame(check_bb.Min, check_bb.Max, GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), true, style.FrameRounding);
        ImU32 check_col = GetColorU32(ImGuiCol_CheckMark);
        if (*v)
        {
            const float check_sz = ImMin(check_bb.GetWidth(), check_bb.GetHeight());
            const float pad = ImMax(1.0f, (float)(int)(check_sz / 6.0f));
            window->DrawList->AddRectFilled(check_bb.Min + ImVec2(pad, pad), check_bb.Max - ImVec2(pad, pad), GetColorU32(ImGuiCol_CheckMark), style.FrameRounding);
        }

        window->DrawList->AddImage(texId, check_bb.Min, check_bb.Max, uv0, uv1, GetColorU32(tint_col));
    }

    if (is_visible && label_size.x > 0.0f)
        RenderText(text_bb.GetTL(), label);

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
    return pressed;
}
bool CheckboxImage(ImTextureRef texId,
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

    const bool is_visible = ItemAdd(total_bb, id);

    // Range-Selection/Multi-selection support (header)
    bool checked = *v;

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);

    // Range-Selection/Multi-selection support (footer)
    if (pressed)
        checked = !checked;

    if (*v != checked)
    {
        *v = checked;
        pressed = true; // return value
        MarkItemEdited(id);
    }

    if (is_visible)
    {
        float a = buildpad::Handler::Instance().IsLessTransparentButtonsEnabled() ? GetStyleColorVec4(ImGuiCol_WindowBg).w / 0.75f : 1.0f;

        auto const floatToVec = [a](float v) { return ImVec4 { v, v, v, util::lerp(1.0f, v, std::clamp(a, 0.0f, 1.0f)) }; };

        RenderNavCursor(total_bb, id);
        window->DrawList->AddImage(texId, check_bb.Min, check_bb.Max, uv0, uv1, GetColorU32(held && hovered
            ? floatToVec(*v ? opacity_on_active : opacity_off_active)
            : hovered
            ? floatToVec(*v ? opacity_on_hover : opacity_off_hover)
            : floatToVec(*v ? opacity_on : opacity_off)));
    }

    if (is_visible && label_size.x > 0.0f)
        RenderText(text_bb.GetTL(), label);

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
    return pressed;
}
void HorizontalLine(float offset = 0.0f, float padding = 0.0f)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;

    float x1 = window->DC.CursorPos.x;
    float x2 = window->DC.CursorPos.x + GetContentRegionAvail().x - 1;
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

    // When not framed, we vertically increase height up to typical framed widget height
    const bool display_frame = (flags & ImGuiTreeNodeFlags_Framed) != 0;
    const bool use_frame_padding = (display_frame || (flags & ImGuiTreeNodeFlags_FramePadding));
    const ImVec2 padding = use_frame_padding ? style.FramePadding : ImVec2(style.FramePadding.x, ImMin(window->DC.CurrLineTextBaseOffset, style.FramePadding.y));

    if (!label_end)
        label_end = FindRenderedTextEnd(label);
    const ImVec2 label_size = CalcTextSize(label, label_end, false);

    const float text_offset_x = g.FontSize + (display_frame ? padding.x * 3 : padding.x * 2);   // Collapsing arrow width + Spacing
    const float text_offset_y = use_frame_padding ? ImMax(style.FramePadding.y, window->DC.CurrLineTextBaseOffset) : window->DC.CurrLineTextBaseOffset; // Latch before ItemSize changes it
    const float text_width = g.FontSize + label_size.x + padding.x * 2;                         // Include collapsing arrow

    const float frame_height = label_size.y + padding.y * 2;
    const bool span_all_columns = (flags & ImGuiTreeNodeFlags_SpanAllColumns) != 0 && (g.CurrentTable != NULL);
    const bool span_all_columns_label = (flags & ImGuiTreeNodeFlags_LabelSpanAllColumns) != 0 && (g.CurrentTable != NULL);
    ImRect frame_bb;
    frame_bb.Min.x = span_all_columns ? window->ParentWorkRect.Min.x : (flags & ImGuiTreeNodeFlags_SpanFullWidth) ? window->WorkRect.Min.x : window->DC.CursorPos.x;
    frame_bb.Min.y = window->DC.CursorPos.y + (text_offset_y - padding.y);
    frame_bb.Max.x = span_all_columns ? window->ParentWorkRect.Max.x : (flags & ImGuiTreeNodeFlags_SpanLabelWidth) ? window->DC.CursorPos.x + text_width + padding.x : window->WorkRect.Max.x;
    frame_bb.Max.y = window->DC.CursorPos.y + (text_offset_y - padding.y) + frame_height;
    if (display_frame)
    {
        const float outer_extend = IM_TRUNC(window->WindowPadding.x * 0.5f); // Framed header expand a little outside of current limits
        frame_bb.Min.x -= outer_extend;
        frame_bb.Max.x += outer_extend;
    }

    ImVec2 text_pos(window->DC.CursorPos.x + text_offset_x, window->DC.CursorPos.y + text_offset_y);
    ItemSize(ImVec2(text_width, frame_height), padding.y);

    // For regular tree nodes, we arbitrary allow to click past 2 worth of ItemSpacing
    ImRect interact_bb = frame_bb;
    if ((flags & (ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_SpanLabelWidth | ImGuiTreeNodeFlags_SpanAllColumns)) == 0)
        interact_bb.Max.x = frame_bb.Min.x + text_width + (label_size.x > 0.0f ? style.ItemSpacing.x * 2.0f : 0.0f);

    // Compute open and multi-select states before ItemAdd() as it clear NextItem data.
    ImGuiID storage_id = (g.NextItemData.HasFlags & ImGuiNextItemDataFlags_HasStorageID) ? g.NextItemData.StorageId : id;
    bool is_open = TreeNodeUpdateNextOpen(storage_id, flags);

    bool is_visible;
    if (span_all_columns || span_all_columns_label)
    {
        // Modify ClipRect for the ItemAdd(), faster than doing a PushColumnsBackground/PushTableBackgroundChannel for every Selectable..
        const float backup_clip_rect_min_x = window->ClipRect.Min.x;
        const float backup_clip_rect_max_x = window->ClipRect.Max.x;
        window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
        window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
        is_visible = ItemAdd(interact_bb, id);
        window->ClipRect.Min.x = backup_clip_rect_min_x;
        window->ClipRect.Max.x = backup_clip_rect_max_x;
    }
    else
    {
        is_visible = ItemAdd(interact_bb, id);
    }
    g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HasDisplayRect;
    g.LastItemData.DisplayRect = frame_bb;

    // If a NavLeft request is happening and ImGuiTreeNodeFlags_NavLeftJumpsToParent enabled:
    // Store data for the current depth to allow returning to this node from any child item.
    // For this purpose we essentially compare if g.NavIdIsAlive went from 0 to 1 between TreeNode() and TreePop().
    // It will become tempting to enable ImGuiTreeNodeFlags_NavLeftJumpsToParent by default or move it to ImGuiStyle.
    bool store_tree_node_stack_data = false;
    if ((flags & ImGuiTreeNodeFlags_DrawLinesMask_) == 0)
        flags |= g.Style.TreeLinesFlags;
    const bool draw_tree_lines = (flags & (ImGuiTreeNodeFlags_DrawLinesFull | ImGuiTreeNodeFlags_DrawLinesToNodes)) && (frame_bb.Min.y < window->ClipRect.Max.y) && (g.Style.TreeLinesSize > 0.0f);
    if (!(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
    {
        store_tree_node_stack_data = draw_tree_lines;
        if ((flags & ImGuiTreeNodeFlags_NavLeftJumpsToParent) && !g.NavIdIsAlive)
            if (g.NavMoveDir == ImGuiDir_Left && g.NavWindow == window && NavMoveRequestButNoResultYet())
                store_tree_node_stack_data = true;
    }

    const bool is_leaf = (flags & ImGuiTreeNodeFlags_Leaf) != 0;
    if (!is_visible)
    {
        if ((flags & ImGuiTreeNodeFlags_DrawLinesToNodes) && (window->DC.TreeRecordsClippedNodesY2Mask & (1 << (window->DC.TreeDepth - 1))))
        {
            ImGuiTreeNodeStackData* parent_data = &g.TreeNodeStack.Data[g.TreeNodeStack.Size - 1];
            parent_data->DrawLinesToNodesY2 = ImMax(parent_data->DrawLinesToNodesY2, window->DC.CursorPos.y); // Don't need to aim to mid Y position as we are clipped anyway.
            if (frame_bb.Min.y >= window->ClipRect.Max.y)
                window->DC.TreeRecordsClippedNodesY2Mask &= ~(1 << (window->DC.TreeDepth - 1)); // Done
        }
        if (is_open && store_tree_node_stack_data)
            TreeNodeStoreStackData(flags, text_pos.x - text_offset_x); // Call before TreePushOverrideID()
        if (is_open && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
            TreePushOverrideID(id);
        IMGUI_TEST_ENGINE_ITEM_INFO(g.LastItemData.ID, label, g.LastItemData.StatusFlags | (is_leaf ? 0 : ImGuiItemStatusFlags_Openable) | (is_open ? ImGuiItemStatusFlags_Opened : 0));
        return is_open;
    }

    if (span_all_columns || span_all_columns_label)
    {
        TablePushBackgroundChannel();
        g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HasClipRect;
        g.LastItemData.ClipRect = window->ClipRect;
    }

    ImGuiButtonFlags button_flags = ImGuiTreeNodeFlags_None;
    if ((flags & ImGuiTreeNodeFlags_AllowOverlap) || (g.LastItemData.ItemFlags & ImGuiItemFlags_AllowOverlap))
        button_flags |= ImGuiButtonFlags_AllowOverlap;
    if (!is_leaf)
        button_flags |= ImGuiButtonFlags_PressedOnDragDropHold;

    // We allow clicking on the arrow section with keyboard modifiers held, in order to easily
    // allow browsing a tree while preserving selection with code implementing multi-selection patterns.
    // When clicking on the rest of the tree node we always disallow keyboard modifiers.
    const float arrow_hit_x1 = (text_pos.x - text_offset_x) - style.TouchExtraPadding.x;
    const float arrow_hit_x2 = (text_pos.x - text_offset_x) + (g.FontSize + padding.x * 2.0f) + style.TouchExtraPadding.x;
    const bool is_mouse_x_over_arrow = (g.IO.MousePos.x >= arrow_hit_x1 && g.IO.MousePos.x < arrow_hit_x2);

    const bool is_multi_select = (g.LastItemData.ItemFlags & ImGuiItemFlags_IsMultiSelect) != 0;
    if (is_multi_select) // We absolutely need to distinguish open vs select so _OpenOnArrow comes by default
        flags |= (flags & ImGuiTreeNodeFlags_OpenOnMask_) == 0 ? ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick : ImGuiTreeNodeFlags_OpenOnArrow;

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
    if (flags & ImGuiTreeNodeFlags_NoNavFocus)
        button_flags |= ImGuiButtonFlags_NoNavFocus;

    bool selected = (flags & ImGuiTreeNodeFlags_Selected) != 0;
    const bool was_selected = selected;

    // Multi-selection support (header)
    if (is_multi_select)
    {
        // Handle multi-select + alter button flags for it
        MultiSelectItemHeader(id, &selected, &button_flags);
        if (is_mouse_x_over_arrow)
            button_flags = (button_flags | ImGuiButtonFlags_PressedOnClick) & ~ImGuiButtonFlags_PressedOnClickRelease;
    }
    else
    {
        //if (window != g.HoveredWindow || !is_mouse_x_over_arrow)
        //    button_flags |= ImGuiButtonFlags_NoKeyModsAllowed;
    }

    bool hovered, held;
    bool pressed = ButtonBehavior(interact_bb, id, &hovered, &held, button_flags);
    bool toggled = false;
    if (!is_leaf)
    {
        if (pressed && g.DragDropHoldJustPressedId != id)
        {
            if ((flags & ImGuiTreeNodeFlags_OpenOnMask_) == 0 || (g.NavActivateId == id && !is_multi_select))
                toggled = true; // Single click
            if (flags & ImGuiTreeNodeFlags_OpenOnArrow)
                toggled |= is_mouse_x_over_arrow && !g.NavHighlightItemUnderNav; // Lightweight equivalent of IsMouseHoveringRect() since ButtonBehavior() already did the job
            if ((flags & ImGuiTreeNodeFlags_OpenOnDoubleClick) && g.IO.MouseClickedCount[0] == 2)
                toggled = true; // Double click
        }
        else if (pressed && g.DragDropHoldJustPressedId == id)
        {
            IM_ASSERT(button_flags & ImGuiButtonFlags_PressedOnDragDropHold);
            if (!is_open) // When using Drag and Drop "hold to open" we keep the node highlighted after opening, but never close it again.
                toggled = true;
            else
                pressed = false; // Cancel press so it doesn't trigger selection.
        }

        if (g.NavId == id && g.NavMoveDir == ImGuiDir_Left && is_open)
        {
            toggled = true;
            NavClearPreferredPosForAxis(ImGuiAxis_X);
            NavMoveRequestCancel();
        }
        if (g.NavId == id && g.NavMoveDir == ImGuiDir_Right && !is_open) // If there's something upcoming on the line we may want to give it the priority?
        {
            toggled = true;
            NavClearPreferredPosForAxis(ImGuiAxis_X);
            NavMoveRequestCancel();
        }

        if (toggled)
        {
            is_open = !is_open;
            window->DC.StateStorage->SetInt(storage_id, is_open);
            g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledOpen;
        }
    }

    // Multi-selection support (footer)
    if (is_multi_select)
    {
        bool pressed_copy = pressed && !toggled;
        MultiSelectItemFooter(id, &selected, &pressed_copy);
        if (pressed)
            SetNavID(id, window->DC.NavLayerCurrent, g.CurrentFocusScopeId, interact_bb);
    }

    if (selected != was_selected)
        g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledSelection;

    // Render
    {
        const ImU32 text_col = GetColorU32(ImGuiCol_Text);
        ImGuiNavRenderCursorFlags nav_render_cursor_flags = ImGuiNavRenderCursorFlags_Compact;
        if (is_multi_select)
            nav_render_cursor_flags |= ImGuiNavRenderCursorFlags_AlwaysDraw; // Always show the nav rectangle
        if (display_frame)
        {
            // Framed type
            const ImU32 bg_col = GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
            RenderFrame(frame_bb.Min, frame_bb.Max, bg_col, true, style.FrameRounding);
            RenderNavCursor(frame_bb, id, nav_render_cursor_flags);
            if (span_all_columns && !span_all_columns_label)
                TablePopBackgroundChannel();
            if (flags & ImGuiTreeNodeFlags_Bullet)
                RenderBullet(window->DrawList, ImVec2(text_pos.x - text_offset_x * 0.60f, text_pos.y + g.FontSize * 0.5f), text_col);
            else if (!is_leaf)
                RenderArrow(window->DrawList, ImVec2(text_pos.x - text_offset_x + padding.x, text_pos.y), text_col, is_open ? ((flags & ImGuiTreeNodeFlags_UpsideDownArrow) ? ImGuiDir_Up : ImGuiDir_Down) : ImGuiDir_Right, 1.0f);
            else // Leaf without bullet, left-adjusted text
                text_pos.x -= text_offset_x - padding.x;
            if (flags & ImGuiTreeNodeFlags_ClipLabelForTrailingButton)
                frame_bb.Max.x -= g.FontSize + style.FramePadding.x;
            if (g.LogEnabled)
                LogSetNextTextDecoration("###", "###");
        }
        else
        {
            // Unframed typed for tree nodes
            if (hovered || selected)
            {
                const ImU32 bg_col = GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
                RenderFrame(frame_bb.Min, frame_bb.Max, bg_col, false);
            }
            RenderNavCursor(frame_bb, id, nav_render_cursor_flags);
            if (span_all_columns && !span_all_columns_label)
                TablePopBackgroundChannel();
            if (flags & ImGuiTreeNodeFlags_Bullet)
                RenderBullet(window->DrawList, ImVec2(text_pos.x - text_offset_x * 0.5f, text_pos.y + g.FontSize * 0.5f), text_col);
            else if (!is_leaf)
                RenderArrow(window->DrawList, ImVec2(text_pos.x - text_offset_x + padding.x, text_pos.y + g.FontSize * 0.15f), text_col, is_open ? ((flags & ImGuiTreeNodeFlags_UpsideDownArrow) ? ImGuiDir_Up : ImGuiDir_Down) : ImGuiDir_Right, 0.70f);
            if (g.LogEnabled)
                LogSetNextTextDecoration(">", NULL);
        }

        if (draw_tree_lines)
            TreeNodeDrawLineToChildNode(ImVec2(text_pos.x - text_offset_x + padding.x, text_pos.y + g.FontSize * 0.5f));

        // Label
        if (display_frame)
            RenderTextClipped(text_pos, frame_bb.Max, label, label_end, &label_size);
        else
            RenderText(text_pos, label, label_end, false);

        if (span_all_columns_label)
            TablePopBackgroundChannel();
    }

    if (is_open && store_tree_node_stack_data)
        TreeNodeStoreStackData(flags, text_pos.x - text_offset_x); // Call before TreePushOverrideID()
    if (is_open && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
        TreePushOverrideID(id); // Could use TreePush(label) but this avoid computing twice

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | (is_leaf ? 0 : ImGuiItemStatusFlags_Openable) | (is_open ? ImGuiItemStatusFlags_Opened : 0));
    return is_open;
}

// CollapsingHeader returns true when opened but do not indent nor push into the ID stack (because of the ImGuiTreeNodeFlags_NoTreePushOnOpen flag).
// This is basically the same as calling TreeNodeEx(label, ImGuiTreeNodeFlags_CollapsingHeader). You can remove the _NoTreePushOnOpen flag if you want behavior closer to normal TreeNode().
bool FixedCollapsingHeader(const char* label, ImGuiTreeNodeFlags flags = 0)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiID id = window->GetID(label);
    return FixedTreeNodeBehavior(id, flags | ImGuiTreeNodeFlags_CollapsingHeader, label);
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
        flags |= ImGuiTreeNodeFlags_AllowOverlap | (ImGuiTreeNodeFlags)ImGuiTreeNodeFlags_ClipLabelForTrailingButton;
    bool is_open = FixedTreeNodeBehavior(id, flags, label);
    if (p_visible != NULL)
    {
        // Create a small overlapping close button
        // FIXME: We can evolve this into user accessible helpers to add extra buttons on title bars, headers, etc.
        // FIXME: CloseButton can overlap into text, need find a way to clip the text somehow.
        ImGuiContext& g = *GImGui;
        ImGuiLastItemData last_item_backup = g.LastItemData;

        float button_size = g.FontSize;
        float button_x = ImMax(g.LastItemData.Rect.Min.x, g.LastItemData.Rect.Max.x - g.Style.FramePadding.x - button_size);
        float button_y = g.LastItemData.Rect.Min.y + g.Style.FramePadding.y;

        ImGuiID close_button_id = GetIDWithSeed("#CLOSE", NULL, id);
        if (CloseButton(close_button_id, ImVec2(button_x, button_y)))
            *p_visible = false;
        g.LastItemData = last_item_backup;
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
    bool IsSameLine;
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
    storedCursor.IsSameLine = window->DC.IsSameLine;
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
    window->DC.IsSameLine = storedCursor.IsSameLine;
    return GetCursorPos();
}
}
