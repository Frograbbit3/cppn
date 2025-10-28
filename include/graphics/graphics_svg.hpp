#pragma once
#include <string>
#include "SDL2/SDL.h"
#include "graphics_utils.hpp"
#include <fmt/core.h>

namespace CPPN
{
    namespace Graphics
    {
        struct RectangleProperties
        {
            int width = 0;
            int height = 0;

            CPPN::Graphics::Color fill= {0,0,0,255};
            CPPN::Graphics::Color stroke= {0,0,0,255};

            double strokeWidth = 0.0;  // 0 = no stroke
            double cornerRadius = 0.0; // rounded corners

            std::string generateSVG() const
            {
                return fmt::format(
                    "<svg xmlns='http://www.w3.org/2000/svg' width='{}' height='{}'>"
                    "<rect width='{}' height='{}' rx='{}' ry='{}' "
                    "fill='rgb({},{},{})' fill-opacity='{}' "
                    "stroke='rgb({},{},{})' stroke-width='{}' stroke-opacity='{}'/>"
                    "</svg>",
                    width, height,
                    width, height,
                    cornerRadius, cornerRadius,
                    fill.red, fill.green, fill.blue, fill.alpha / 255.0,
                    stroke.red, stroke.green, stroke.blue, strokeWidth, stroke.alpha / 255.0);
            }
        };
        struct OvalProperties
        {
            int width = 0;
            int height = 0;

            CPPN::Graphics::Color fill = {0,0,0,255};
            CPPN::Graphics::Color stroke= {0,0,0,255};

            double strokeWidth = 0.0; // 0 = no stroke

            std::string generateSVG() const
            {
                return fmt::format(
                    "<svg xmlns='http://www.w3.org/2000/svg' width='{}' height='{}'>"
                    "<ellipse cx='{}' cy='{}' rx='{}' ry='{}' "
                    "fill='rgb({},{},{})' fill-opacity='{}' "
                    "stroke='rgb({},{},{})' stroke-width='{}' stroke-opacity='{}'/>"
                    "</svg>",
                    width, height,
                    width / 2.0, height / 2.0, width / 2.0, height / 2.0,
                    fill.red, fill.green, fill.blue, fill.alpha / 255.0,
                    stroke.red, stroke.green, stroke.blue, strokeWidth, stroke.alpha / 255.0);
            }
        };

    }
}