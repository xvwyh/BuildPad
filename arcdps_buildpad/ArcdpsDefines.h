#pragma once
#include "Common.h"

namespace arcdps
{

/* arcdps export table */
struct arcdps_exports
{
    uintptr_t size; /* size of exports table */
    uintptr_t sig; /* pick a number between 0 and uint64_t max that isn't used by other modules */
    char const* out_name; /* name string */
    char const* out_build; /* build string */
    void* wnd_nofilter; /* wndproc callback, fn(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) */
    void* combat; /* combat event callback, fn(cbtevent* ev, ag* src, ag* dst, char* skillname, uint64_t id, uint64_t revision) */
    void* imgui; /* id3dd9::present callback, before imgui::render, fn(uint32_t not_charsel_or_loading) */
    void* options; /* id3dd9::present callback, appending to the end of options window in arcdps, fn() */
    void* combat_local;  /* combat event callback like area but from chat log, fn(cbtevent* ev, ag* src, ag* dst, char* skillname, uint64_t id, uint64_t revision) */
    void* wnd_filter; /* wndproc callback like above, input filered using modifiers */
    void* options_windows; /* called once per 'window' option checkbox, with null at the end, non-zero return disables drawing that option, fn(char* windowname) */
};

} // namespace arcdps
