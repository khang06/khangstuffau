#include "pch-il2cpp.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <fmt/format.h>
#include "helpers.h"
#include "util.h"

using namespace app;

__declspec(noreturn) void fatal(std::string text) {
    fmt::print("FATAL ERROR: {}\nExiting in 5 seconds...\n", text);
    Sleep(5000);
    exit(1);
}

std::tuple<Texture2D*, int, int> TextureFromPNG(char* data, size_t size) {
    auto managed_img = il2cpp_array_new((Il2CppClass*)*Byte__Array__TypeInfo, size);
    //fmt::print("managed_img at {}, size {}\n", fmt::ptr(managed_img), size);
    memcpy(managed_img->vector, data, size);

    Texture2D* tex2d = nullptr;
    try {
        tex2d = Utils_1_GetTexture2DFromByteArray((Byte__Array*)managed_img, nullptr);
        //fmt::print("tex2d at {}\n", fmt::ptr(tex2d));
    }
    catch (...) {
        fatal("failed to create the texture!");
    }

    auto tex2dvtbl = tex2d->klass->vtable;
    // TODO: this looks really bad, can i make this any better?
    auto tex_width = ((int(__cdecl*)(Texture2D*, const MethodInfo*))tex2dvtbl.get_width.methodPtr)(tex2d, tex2dvtbl.get_width.method);
    auto tex_height = ((int(__cdecl*)(Texture2D*, const MethodInfo*))tex2dvtbl.get_height.methodPtr)(tex2d, tex2dvtbl.get_height.method);

    return { tex2d, tex_width, tex_height };
}

void ReadIntoVec(std::vector<char>& vec, std::filesystem::path path) {
    // iostreams stink
    auto file = fopen(path.string().c_str(), "rb");
    if (!file)
        return;

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    vec.resize(file_size);
    fseek(file, 0, SEEK_SET);
    fread(vec.data(), file_size, 1, file);
    fclose(file);
}