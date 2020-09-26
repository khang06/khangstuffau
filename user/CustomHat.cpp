#include "pch-il2cpp.h"

#include "util.h"
#include "helpers.h"
#include <fmt/format.h>
#include "inih/cpp/INIReader.h"
#include "CustomHat.h"

std::vector<CustomHat> CustomHat::custom_hats;

using namespace app;

// TODO: do some c++ stuff to reduce duplicated code here
CustomHat::CustomHat(std::filesystem::path path) {
    INIReader reader(path.string());

    // TODO: reduce duplicated code here
    ReadIntoVec(front_png, path.parent_path() / (path.stem().string() + std::string("-front.png")));
    ReadIntoVec(back_png, path.parent_path() / (path.stem().string() + std::string("-back.png")));
    ReadIntoVec(floor_png, path.parent_path() / (path.stem().string() + std::string("-floor.png")));

    name = reader.GetString("", "name", name);
    author = reader.GetString("", "author", author);
    no_bounce = reader.GetBoolean("", "no_bounce", no_bounce);
    pivot_x = reader.GetReal("", "pivot_x", pivot_x);
    pivot_y = reader.GetReal("", "pivot_y", pivot_y);

    custom_index = GetHatList().size();

    fmt::print("{} | Loaded {} by {} ({})\n", custom_index, name, author, path.stem().string());
}

void CustomHat::Apply() {
    // WARNING: HARDCODED PER VERSION, UPDATE THIS!!!
    // Il2CppDumper won't dump DestroyableSingleton_1_HatManager__get_Instance__MethodInfo for some reason...
    auto mi = *(MethodInfo**)(il2cppi_get_base_address() + 0x1474C38);
    HatManager* hat_manager = nullptr;
    if (!mi || !(hat_manager = DestroyableSingleton_1_HatManager__get_Instance(mi)))
        return;

    Sprite* front_sprite = CreateSprite(front_png);
    Sprite* back_sprite = CreateSprite(back_png);
    Sprite* floor_sprite = CreateSprite(floor_png);

    auto hat = (HatBehaviour*)il2cpp_object_new((Il2CppClass*)*HatBehaviour__TypeInfo);
    hat->fields.AltShader = hat_manager->fields.DefaultHatShader;
    hat->fields.BackImage = back_sprite;
    hat->fields.ChipOffset = Vector2{ 0.0f, 0.0f }; // does nothing?
    hat->fields.FloorImage = floor_sprite;
    hat->fields.Free = true;
    hat->fields.InFront = back_png.size() == 0;
    hat->fields.LimitedMonth = 0;
    hat->fields.LimitedYear = 0;
    hat->fields.MainImage = front_sprite;
    hat->fields.NoBounce = true;
    hat->fields.NotInStore = false;
    hat->fields.Order = -1 - custom_index;
    hat->fields.ProductId = Marshal_PtrToStringAnsi((void*)"", nullptr);
    hat->fields.RelatedSkin = nullptr;
    hat->fields.StoreName = Marshal_PtrToStringAnsi((void*)"", nullptr); // don't want to risk it with the weird garbage collection shit
    ScriptableObject__ctor((ScriptableObject*)hat, nullptr);

    auto hat_list = hat_manager->fields.AllHats;
    if (inject_index == -1) {
        // not yet injected into the hats list, so inject it and save the index
        inject_index = hat_list->fields._size;
        auto add_method = hat_list->klass->vtable.Add;
        // TODO: this is awful to look at
        ((void(*)(List_1_HatBehaviour_*, HatBehaviour*, const MethodInfo*))add_method.methodPtr)(hat_list, hat, add_method.method);
    } else {
        // already added, but the additional entries get zeroed out and have to be replaced
        auto set_item_method = hat_list->klass->vtable.set_Item;
        // TODO: this too
        ((void(*)(List_1_HatBehaviour_*, int, HatBehaviour*, const MethodInfo*))set_item_method.methodPtr)(hat_list, inject_index, hat, set_item_method.method);
    }
}

Sprite* CustomHat::CreateSprite(std::vector<char>& png) {
    if (png.size() == 0)
        return nullptr;

    Texture2D* tex2d = nullptr;
    int tex_width, tex_height = 0;
    std::tie(tex2d, tex_width, tex_height) = TextureFromPNG(png.data(), png.size());

    Sprite* sprite = nullptr;
    try {
        sprite = Sprite_Create_5(tex2d, Rect{ 0.0f, 0.0f, (float)tex_width, (float)tex_height }, Vector2{ pivot_x, pivot_y }, nullptr);
        //fmt::print("sprite at {} with size {}x{}\n", fmt::ptr(sprite), tex_width, tex_height);
    }
    catch (...) {
        fatal("failed to create the sprite!");
    }

    return sprite;
}
