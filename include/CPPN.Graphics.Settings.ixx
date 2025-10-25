#pragma once
export module CPPN.Graphics.Settings;
#include <string>

export namespace CPPN::Graphics {
    export struct WindowSettings {
        int WINDOW_WIDTH = 680;
        int WINDOW_HEIGHT = 480;
        std::string WINDOW_TITLE = "Example Project";
    };
} // namespace CPPN::Graphics