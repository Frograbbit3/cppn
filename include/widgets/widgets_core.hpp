#pragma once
#include <string>
#include "../graphics/graphics_core.hpp"
#include "../enums/enums.hpp"
#include "SDL2/SDL.h"
namespace CPPN {
    namespace Widgets {
        struct WidgetProperties {
            CPPN::Widgets::AlignTypes alignment;
            int x=50; 
            int y=50;
            int width=50;
            int height=50;
        };
        class Label {
            private:
                WidgetProperties properties;
                std::string value;
                SDL_Texture* texture;
            public:
                Label(const std::string& text, int x, int y, int width=0, int height=0): value(text) {
                    properties.x = x;
                    properties.y = y;                    
                    if (width > 0) {properties.width=width;}
                    if (height > 0) {properties.height=height;}
                };
        };
    }
}
