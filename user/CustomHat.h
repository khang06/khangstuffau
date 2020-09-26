#pragma once
#include "pch-il2cpp.h"

#include <filesystem>
#include <string>
#include <vector>

class CustomHat {
public:
    CustomHat(std::filesystem::path path);
    void Apply();

    static std::vector<CustomHat>& GetHatList() {
        return custom_hats;
    }

    // can't save the texture2ds, so they have to be decoded every time
    // TODO: decode the png independently and save the decompressed output if it gets too slow
    std::vector<char> front_png;
    std::vector<char> back_png;
    std::vector<char> floor_png;

    std::string name = "Placeholder Name";
    std::string author = "Placeholder Author";
    bool no_bounce = false;
    float pivot_x = 0.5;
    float pivot_y = 0.5;
private:
    app::Sprite* CreateSprite(std::vector<char>& png);

    static std::vector<CustomHat> custom_hats;
    int inject_index = -1;
    int custom_index = -1;
};