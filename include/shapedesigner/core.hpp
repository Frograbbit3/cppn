#pragma once
#include "../graphics/graphics_core.hpp"
#include "../graphics/graphics_utils.hpp" //purely for the color class
#include <SDL2/SDL.h>
#include "shapes.hpp"
using namespace CPPN;
using CPPN::Graphics::Color;

namespace CPPN {
    namespace ShapeDesigner {
        SDL_Texture* ConvertShapeToTexture(Shape* shape) {
            switch (shape->shape) {
                case (ShapeTypes::RECTANGLE):
                    return GenerateRect(shape);
                    break;
                case (ShapeTypes::OVAL):
                    return GenerateOval(shape);
                    break;
                case (ShapeTypes::CIRCLE):
                    return GenerateCircle(shape);
                    break;
                case (ShapeTypes::POLYGON):
                    return GeneratePolygon(shape);
                    break;
            }
            return nullptr;
        }

    }
}
