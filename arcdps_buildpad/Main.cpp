#include "Common.h"
#include "ArcDPSDefines.h"
#include "buildpad/Handler.h"
#include <d3d9.h>
#include <Windows.h>

/* proto/globals */
arcdps::arcdps_exports arc_exports;
char* arcvers;
LPDIRECT3DDEVICE9 d3ddevice;

/* dll attach -- from winapi */
bool dll_init(HANDLE hModule)
{
    fs::path const bin64path = "./bin64";
    std::set<std::string> versionsFound;
    try
    {
        if (exists(bin64path))
            for (auto const& entry : fs::directory_iterator { bin64path })
                if (entry.is_regular_file() && util::starts_with(entry.path().filename().string(), "d3d9_arcdps_buildpad_") && entry.path().extension() == ".dll")
                    versionsFound.emplace(entry.path().filename().replace_extension().string().substr("d3d9_arcdps_buildpad_"s.length()));

        if (versionsFound.size() > 1)
        {
            std::string const newestVersion = *versionsFound.rbegin();

            // If we're not running the latest installed version, but there is a latest version DLL in bin64 folder - don't load this outdated version
            if (buildpad::BUILDPAD_VERSION != newestVersion)
                return false;

            // If we're running the latest installed version - delete all other versions (they shouldn't be in use right now)
            if (buildpad::BUILDPAD_VERSION == newestVersion)
                for (auto const& version : versionsFound)
                    if (version != newestVersion)
                        if (fs::path path = fmt::format("./bin64/d3d9_arcdps_buildpad_{}.dll", version); exists(path))
                            fs::remove(path);

            // No longer using this file - remove it if previous BuildPad version have been deleted
            fs::path const versionPath = "./addons/arcdps/arcdps.buildpad/version";
            if (exists(versionPath))
                fs::remove(versionPath);
        }
    }
    catch (...) { }

    return true;
}

/* dll detach -- from winapi */
void dll_exit()
{
}

/* dll main -- winapi */
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ulReasonForCall, LPVOID lpReserved)
{
    switch (ulReasonForCall)
    {
        case DLL_PROCESS_ATTACH: return dll_init(hModule);
        case DLL_PROCESS_DETACH: dll_exit(); break;

        case DLL_THREAD_ATTACH: break;
        case DLL_THREAD_DETACH: break;
    }
    return 1;
}

/* release mod -- return ignored */
uintptr_t mod_release()
{
    buildpad::Handler::Instance().SaveConfig();
    buildpad::Handler::Instance().Unload();
    return 0;
}

/* export -- arcdps looks for this exported function and calls the address it returns */
extern "C" __declspec(dllexport) void* get_release_addr()
{
    arcvers = nullptr;
    return (void*)&mod_release;
}

/* window callback -- return is assigned to umsg (return zero to not be processed by arcdps or game) */
uintptr_t mod_wnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            auto const key = (int)wParam;
            bool const down = uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN;
            if (key >= 256)
                break;

            auto& io = ImGui::GetIO();
            io.KeysDown[key] = down;
            io.KeysDownDuration[key] = down ? 0.0f : -1.0f;
            switch (key)
            {
                case VK_CONTROL: io.KeyCtrl  = down; break;
                case VK_MENU:    io.KeyAlt   = down; break;
                case VK_SHIFT:   io.KeyShift = down; break;
            }

            if (buildpad::Handler::Instance().HandleKeyBinds())
                return 0;

            io.KeysDownDuration[key] = -1;
            break;
        }
    }

    return uMsg;
}

void mod_imgui()
{
    static bool inited = false;
    if (!inited)
    {
        inited = true;
        buildpad::Handler::Instance().LoadTextures();
        buildpad::Handler::Instance().LoadConfig();
#ifndef DLL
        buildpad::Handler::Instance().LoadTest();
#endif
    }
    buildpad::Handler::Instance().Update();
}

void mod_options()
{
    buildpad::Handler::Instance().UpdateOptions();
}

/* initialize mod -- return table that arcdps will use for callbacks */
arcdps::arcdps_exports* mod_init()
{
    arc_exports.size = sizeof(arcdps::arcdps_exports);
    arc_exports.sig = 0x213CFFD2;
    arc_exports.imguivers = IMGUI_VERSION_NUM;
    arc_exports.out_name = "buildpad";
    arc_exports.out_build = buildpad::BUILDPAD_VERSION;
    arc_exports.wnd_nofilter = (void*)&mod_wnd;
    arc_exports.imgui = (void*)&mod_imgui;
    arc_exports.options = (void*)&mod_options;
    return &arc_exports;
}

/* export -- arcdps looks for this exported function and calls the address it returns */
extern "C" __declspec(dllexport) void* get_init_addr(char* arcversionstr, void* imguicontext, IDirect3DDevice9* id3dd9, HANDLE arcdll, void* mallocfn, void* freefn)
{
    arcvers = arcversionstr;
    ImGui::SetCurrentContext((ImGuiContext*)imguicontext);
    ImGui::SetAllocatorFunctions((void *(*)(size_t, void*))mallocfn, (void(*)(void*, void*))freefn);
    d3ddevice = id3dd9;
    return (void*)&mod_init;
}

#ifdef DLL
LPDIRECT3DDEVICE9 GetDirect3DDevice9()
{
    return d3ddevice;
}
#endif
