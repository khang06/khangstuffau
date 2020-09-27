// Generated C++ file by Il2CppInspector - http://www.djkaty.com - https://github.com/djkaty
// Custom injected code entry point

#include "pch-il2cpp.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Shlobj.h>
#include <fmt/format.h>
#include <filesystem>
#include "helpers.h"

#include "CustomHat.h"
#include "Config.h"
#include "util.h"
#include "Hooks.h"
#include "main.h"

// Set the name of your log file here
extern const LPCWSTR LOG_FILE = L"khangstuffau.txt";

using namespace app; // this will probably cause problems later

// why do i even have to make 3 macros for 1???
// TODO: i should abstract this better
#define MAKE_HOOK(x) fmt::print("Installing hook for {}\n", #x); \
                     MAKE_HOOK_HIDDEN1(x).setSubs((void*)x, (void*)MAKE_HOOK_HIDDEN2(x)); \
                     MAKE_HOOK_HIDDEN1(x).installHook();
#define MAKE_HOOK_HIDDEN1(x) hook_##x
#define MAKE_HOOK_HIDDEN2(x) custom_##x

C_Hook hook_AmongUsClient__ctor;
C_Hook hook_AmongUsClient_Awake;
C_Hook hook_VersionShower_Start;
C_Hook hook_SceneManager_Internal_ActiveSceneChanged;

// prevent race conditions
void custom_AmongUsClient__ctor(AmongUsClient* thisptr, MethodInfo* method) {
    hook_AmongUsClient__ctor.removeHook();
    AmongUsClient__ctor(thisptr, method);
    Run();
}

void custom_AmongUsClient_Awake(AmongUsClient* thisptr, MethodInfo* method) {
    hook_AmongUsClient_Awake.removeHook();
    AmongUsClient_Awake(thisptr, method);
    if (Config::GetInstance()->unlock_framerate)
        Application_set_targetFrameRate(-1, nullptr);
    else
        Application_set_targetFrameRate(30, nullptr); // for some reason i can't just not install this hook to lock the framerate
    hook_AmongUsClient_Awake.installHook();
}

void custom_VersionShower_Start(VersionShower* thisptr, MethodInfo* method) {
    hook_VersionShower_Start.removeHook();
    VersionShower_Start(thisptr, method);
    hook_VersionShower_Start.installHook();

    auto custom_suffix = Marshal_PtrToStringAnsi((void*)" / khangstuffau " __DATE__, nullptr);
    auto& source_str = thisptr->fields.text->fields.Text;
    source_str = String_Concat_1((Object*)source_str, (Object*)custom_suffix, nullptr);
}

// delegates are too hard
void custom_SceneManager_Internal_ActiveSceneChanged(Scene previousActiveScene, Scene newActiveScene, MethodInfo* method) {
    hook_SceneManager_Internal_ActiveSceneChanged.removeHook();
    SceneManager_Internal_ActiveSceneChanged(previousActiveScene, newActiveScene, method);
    hook_SceneManager_Internal_ActiveSceneChanged.installHook();

    for (auto& hat : CustomHat::GetHatList())
        hat.Apply();
}

void Init() {
    MAKE_HOOK(AmongUsClient__ctor);
}

// Custom injected code entry point
void Run() {
    il2cppi_new_console();
    SetConsoleTitle(L"khangstuffau debug");

    fmt::print("khangstuffau " __DATE__ "\n");

    auto str = Application_get_version(nullptr);
    fmt::print("Among Us version {}\n", il2cppi_to_string(str));

    fmt::print("GameAssembly.dll base: {}\n", (void*)il2cppi_get_base_address());

    std::filesystem::create_directories(HATS_DIR);
    Config::Load(CONFIG_DIR "/config.ini");

    for (const auto& entry : std::filesystem::directory_iterator(HATS_DIR)) {
        if (!entry.is_directory() && entry.path().extension().string() == ".ini") {
            CustomHat::GetHatList().push_back(CustomHat(entry.path()));
        }
    }

    MAKE_HOOK(AmongUsClient_Awake);
    MAKE_HOOK(VersionShower_Start);
    MAKE_HOOK(SceneManager_Internal_ActiveSceneChanged);
}