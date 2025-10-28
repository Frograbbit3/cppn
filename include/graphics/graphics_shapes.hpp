#pragma once
#include "graphics_general.hpp"
#include "graphics_core.hpp"
#include "graphics_svg.hpp"
#include "graphics_utils.hpp"
namespace CPPN
{
    namespace Graphics
    {
        class Rectangle : public BaseShape
        {
        private:
            SDL_Rect rect;
            SDL_Texture *texture;
            CPPN::Graphics::RectangleProperties properties;

        public:
            Rectangle(int x, int y, int width, int height, CPPN::Graphics::RectangleProperties settings)
                : BaseShape(x, y, settings.fill), properties(settings)
            {
                properties.width = width;
                properties.height = height;
                std::cout << properties.generateSVG() <<std::endl;
                texture = CPPN::Graphics::LoadSVG(properties.generateSVG(), CPPN::Graphics::renderer);
            }

            virtual void update() override
            {
                rect.x = x;
                rect.y = y;
                rect.w = properties.width;
                rect.h = properties.height;
                //texture = CPPN::Graphics::LoadSVG(properties.generateSVG(), CPPN::Graphics::renderer);
            }
            void draw(SDL_Renderer *ren) const override
            {
                SDL_RenderCopyEx(ren, texture, nullptr, &rect, rotation, nullptr, SDL_FLIP_NONE);
            }
            bool isColliding(int px, int py) const override
            {
                return (px >= x && px <= x + properties.width) &&
                    (py >= y && py <= y + properties.height);
            }
        };
        class Oval : public BaseShape
        {
        private:
            SDL_Texture *texture;
            SDL_Rect rect;

        public:
            CPPN::Graphics::OvalProperties properties;
            Oval(int x, int y, CPPN::Graphics::OvalProperties settings)
                : BaseShape(x, y, properties.fill), properties(settings)
            {
               // texture = CPPN::Graphics::LoadSVG(properties.generateSVG(), CPPN::Graphics::renderer);
                update();

                texture = CPPN::Graphics::LoadSVG(properties.generateSVG(), CPPN::Graphics::renderer);
            }

            virtual void update() override
            {
                rect.x = x;
                rect.y = y;
                rect.h = properties.height;
                rect.w = properties.width;
            }

            void draw(SDL_Renderer *ren) const override
            {
                SDL_RenderCopyEx(ren, texture, nullptr, &rect, rotation, nullptr, SDL_FLIP_NONE);
            }

            bool isColliding(int px, int py) const override {
                int cx = rect.x + rect.w / 2;
                int cy = rect.y + rect.h / 2;
                int a = rect.w / 2;
                int b = rect.h / 2;
                double dx = (px - cx) / double(a);
                double dy = (py - cy) / double(b);
                return dx*dx + dy*dy <= 1.0;
            }
        };

    }
} // namespace CPPN
