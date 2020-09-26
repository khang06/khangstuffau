#pragma once

#include <string>

class Config {
public:
    static Config* GetInstance() {
        if (!instance)
            instance = new Config();
        return instance;
    }
    static void Load(std::string path);

    bool unlock_framerate = false;
private:
    static Config* instance;
};