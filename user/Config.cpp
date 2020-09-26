#include "pch-il2cpp.h"

#include <fmt/format.h>
#include "inih/cpp/INIReader.h"
#include "Config.h"

Config* Config::instance = nullptr;

// TODO: do some c++ stuff to reduce duplicated code here
void Config::Load(std::string path) {
    INIReader reader(path.c_str());

    auto instance = GetInstance();
    instance->unlock_framerate = reader.GetBoolean("", "unlock_framerate", instance->unlock_framerate);

    fmt::print("unlock_framerate: {}\n", instance->unlock_framerate);
}