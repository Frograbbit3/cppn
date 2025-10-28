#pragma once
#include <string>
#include "SDL2/SDL.h"
#include "graphics_utils.hpp"
#include <fmt/core.h>

namespace CPPN {
namespace Graphics {

struct RectangleProperties {
    int width = 0;
    int height = 0;

    CPPN::Graphics::Color fill = {0, 0, 0, 255};
    CPPN::Graphics::Color stroke = {0, 0, 0, 255};

    double strokeWidth = 0.0;   // 0 = no stroke
    double cornerRadius = 0.0;  // rounded corners

    // ---- Builder methods ----
    RectangleProperties& setSize(int w, int h) {
        width = w; height = h; return *this;
    }
    RectangleProperties& setFill(const CPPN::Graphics::Color& c) {
        fill = c; return *this;
    }
    RectangleProperties& setStroke(const CPPN::Graphics::Color& c) {
        stroke = c; return *this;
    }
    RectangleProperties& setStrokeWidth(double w) {
        strokeWidth = w; return *this;
    }
    RectangleProperties& setCornerRadius(double r) {
        cornerRadius = r; return *this;
    }

    // ---- SVG Generator ----
    std::string generateSVG() const {
        int w = width  > 0 ? width  : 1;
        int h = height > 0 ? height : 1;

        return fmt::format(
            "<svg xmlns='http://www.w3.org/2000/svg' width='{}' height='{}'>"
            "<rect width='{}' height='{}' rx='{}' ry='{}' "
            "fill='rgb({},{},{})' fill-opacity='{}' "
            "stroke='rgb({},{},{})' stroke-width='{}' stroke-opacity='{}'/>"
            "</svg>",
            w, h,
            w, h,
            cornerRadius, cornerRadius,
            fill.red, fill.green, fill.blue, fill.alpha / 255.0,
            stroke.red, stroke.green, stroke.blue, strokeWidth, stroke.alpha / 255.0);
    }
};

// ---------------------------------------------------------

struct OvalProperties {
    int width = 0;
    int height = 0;

    CPPN::Graphics::Color fill = {0, 0, 0, 255};
    CPPN::Graphics::Color stroke = {0, 0, 0, 255};

    double strokeWidth = 0.0; // 0 = no stroke

    // ---- Builder methods ----
    OvalProperties& setSize(int w, int h) {
        width = w; height = h; return *this;
    }
    OvalProperties& setFill(const CPPN::Graphics::Color& c) {
        fill = c; return *this;
    }
    OvalProperties& setStroke(const CPPN::Graphics::Color& c) {
        stroke = c; return *this;
    }
    OvalProperties& setStrokeWidth(double w) {
        strokeWidth = w; return *this;
    }

    // ---- SVG Generator ----
    std::string generateSVG() const {
        int w = width  > 0 ? width  : 1;
        int h = height > 0 ? height : 1;

        return fmt::format(
            "<svg xmlns='http://www.w3.org/2000/svg' width='{}' height='{}'>"
            "<ellipse cx='{}' cy='{}' rx='{}' ry='{}' "
            "fill='rgb({},{},{})' fill-opacity='{}' "
            "stroke='rgb({},{},{})' stroke-width='{}' stroke-opacity='{}'/>"
            "</svg>",
            w+strokeWidth, h+strokeWidth,
            w / 2.0, h / 2.0, w / 2.0, h / 2.0,
            fill.red, fill.green, fill.blue, fill.alpha / 255.0,
            stroke.red, stroke.green, stroke.blue, strokeWidth, stroke.alpha / 255.0);
    }
};

} // namespace Graphics
} // namespace CPPN
