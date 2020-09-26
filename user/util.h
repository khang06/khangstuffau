#pragma once
#include "pch-il2cpp.h"
#include <filesystem>
#include <string>

#define CONFIG_DIR "khangstuffau"
#define HATS_DIR CONFIG_DIR "/hats"

__declspec(noreturn) void fatal(std::string text);
std::tuple<app::Texture2D*, int, int> TextureFromPNG(char* data, size_t size);
void ReadIntoVec(std::vector<char>& vec, std::filesystem::path path);