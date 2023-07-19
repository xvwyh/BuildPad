#include "Common.h"
#include "ArcdpsDefines.h"
#include "buildpad/Handler.h"
#include <d3d9.h>
#include <Windows.h>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Easy.hpp>
#include "json/json.hpp"

/* proto/globals */
arcdps::arcdps_exports arc_exports;
char* arcvers;
IDirect3DDevice9* d3dDevice9 = nullptr;
ID3D11Device* d3dDevice11 = nullptr;

void CleanupPreviousVersions()
{
    try
    {
        std::set<std::string> versionsFound;
        if (fs::path const bin64path = "./bin64"; exists(bin64path))
            for (auto const& entry : fs::directory_iterator { bin64path })
                if (entry.is_regular_file() && util::starts_with(entry.path().filename().string(), "d3d9_arcdps_buildpad_") && entry.path().extension() == ".dll")
                    versionsFound.emplace(entry.path().filename().replace_extension().string().substr("d3d9_arcdps_buildpad_"s.length()));

        // Delete all other versions
        for (auto const& version : versionsFound)
        {
            try
            {
                if (fs::path path = fmt::format("./bin64/d3d9_arcdps_buildpad_{}.dll", version); exists(path))
                    fs::remove(path);
            }
            catch (...) { }
        }

        // No longer using this file - remove it if previous BuildPad version have been deleted
        fs::path const versionPath = "./addons/arcdps/arcdps.buildpad/version";
        if (exists(versionPath))
            fs::remove(versionPath);
    }
    catch (...) { }
}

/* dll attach -- from winapi */
void dll_init()
{
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
        case DLL_PROCESS_ATTACH: dll_init(); break;
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

    // Try to rename the currently running DLL
    try
    {
        if (fs::path const path = fmt::format("./bin64/d3d9_arcdps_buildpad_{}.dll", buildpad::BUILDPAD_VERSION); exists(path))
            fs::rename(path, "./bin64/arcdps_buildpad.dll");
    }
    catch (...) { }
    // Then delete the remaining old versions, in case they failed to be deleted at startup
    CleanupPreviousVersions();

    return 0;
}

/* export -- arcdps looks for this exported function and calls the address it returns */
extern "C" __declspec(dllexport) void* get_release_addr()
{
    arcvers = nullptr;
    return (void*)&mod_release;
}

extern "C" __declspec(dllexport) wchar_t* get_update_url()
{
    try
    {
        std::stringstream response;
        curlpp::Cleanup clean;
        curlpp::Easy request;
        request.setOpt(new curlpp::options::Url("https://buildpad.gw2archive.eu/version.json"));
        request.setOpt(new curlpp::options::WriteStream(&response));
        request.perform();
        std::string data = response.str();

        auto json = nlohmann::json::parse(response, nullptr, false);
        if (buildpad::BUILDPAD_VERSION < json["latest"])
        {
            std::string const url = json["url"];
            static std::wstring wurl(MultiByteToWideChar(CP_UTF8, 0, url.c_str(), -1, nullptr, 0), L'\0');
            MultiByteToWideChar(CP_UTF8, 0, url.c_str(), -1, wurl.data(), (int)wurl.length());
            return wurl.data();
        }
    }
    catch (...) { }

    return nullptr;
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
    arc_exports.sig = 0x92F80465; // Old: 0x213CFFD2
    arc_exports.imguivers = IMGUI_VERSION_NUM;
    arc_exports.out_name = "BuildPad";
    arc_exports.out_build = buildpad::BUILDPAD_VERSION;
    arc_exports.wnd_nofilter = (void*)&mod_wnd;
    arc_exports.imgui = (void*)&mod_imgui;
    arc_exports.options = (void*)&mod_options;
    return &arc_exports;
}

/* export -- arcdps looks for this exported function and calls the address it returns */
extern "C" __declspec(dllexport) void* get_init_addr(char* arcversion, ImGuiContext* imguictx, void* id3dptr, HANDLE arcdll, void* mallocfn, void* freefn, uint32_t d3dversion)
{
    // Try to unload the old version if it got loaded and delete its DLL
    if (FARPROC freeExtension; (freeExtension = GetProcAddress((HMODULE)arcdll, "freeextension2")) || (freeExtension = GetProcAddress((HMODULE)arcdll, "removeextension2")))
    {
        std::this_thread::sleep_for(3s); // Artifical delay. Arcdps loads extensions asynchronously. If there's an old version present - we want to ensure that it has enough time to load
        ((HINSTANCE(*)(uint32_t))freeExtension)(0x213CFFD2);
    }
    CleanupPreviousVersions();

    arcvers = arcversion;
    ImGui::SetCurrentContext(imguictx);
    ImGui::SetAllocatorFunctions((void *(*)(size_t, void*))mallocfn, (void(*)(void*, void*))freefn);
    switch (d3dversion)
    {
        case 9:
        {
            d3dDevice9 = static_cast<IDirect3DDevice9*>(id3dptr);
            break;
        }
        case 11:
        {
            if (static_cast<IDXGISwapChain*>(id3dptr)->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&d3dDevice11)) != S_OK)
                return nullptr;
            break;
        }
        default: return nullptr;
    }
    return (void*)&mod_init;
}

#ifdef DLL
LPDIRECT3DDEVICE9 GetDirect3DDevice9()
{
    return d3dDevice9;
}
ID3D11Device* GetDirect3DDevice11()
{
    return d3dDevice11;
}
#endif
