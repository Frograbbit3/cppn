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

    /*
        Sets the size of the rectangle. Will be overwritten unless you run .update on the rect.
        @param w The width of the rect.
        @param h The height of the rect.
    */
    RectangleProperties& setSize(int w, int h) {
        width = w; height = h; return *this;
    }
    /*
        Sets the fill of the rectangle.
        @param c CPPN::Graphics::Color color.
    */
    RectangleProperties& setFill(const CPPN::Graphics::Color& c) {
        fill = c; return *this;
    }   
    /*
        Sets the stroke color of the rectangle.
        @param c CPPN::Graphics::Color color.
    */
    RectangleProperties& setStroke(const CPPN::Graphics::Color& c) {
        stroke = c; return *this;
    }
    /*
        Sets the stroke size of the rectangle.
        @param w The stroke width.
    */
    RectangleProperties& setStrokeWidth(double w) {
        strokeWidth = w; return *this;
    }
    /*
        Sets the corner radius of the rectangle.
        @param r The corner radius.
    */
    RectangleProperties& setCornerRadius(double r) {
        cornerRadius = r; return *this;
    }

    // ---- SVG Generator ----
    /*
        Generates the SVG for the object. Used internally for rendering.
        @return A string with SVG data.
    */
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
    /*
        Sets the size of the oval. Will be overwritten unless you run .update on the oval.
        @param w The width of the oval.
        @param h The height of the oval.
    */
    OvalProperties& setSize(int w, int h) {
        width = w; height = h; return *this;
    }
    /*
        Sets the fill of the oval.
        @param c CPPN::Graphics::Color color.
    */
    OvalProperties& setFill(const CPPN::Graphics::Color& c) {
        fill = c; return *this;
    }
    /*
        Sets the stroke color of the oval.
        @param c CPPN::Graphics::Color color.
    */
    OvalProperties& setStroke(const CPPN::Graphics::Color& c) {
        stroke = c; return *this;
    }
    /*
        Sets the stroke radius of the rectangle.
        @param w The radius of the stroke.
    */
    OvalProperties& setStrokeWidth(double w) {
        strokeWidth = w; return *this;
    }

    /*
        Generates the SVG for the object. Used internally for rendering.
        @return A string with SVG data.
    */
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
