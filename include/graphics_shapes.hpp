#pragma once
#include "graphics_general.hpp"
namespace CPPN
{
    namespace Graphics {
        class Rectangle : public BaseShape
        {
        private:
            SDL_Rect rect;

        public:
            int width;
            int height;
            Rectangle(int x, int y, int wid, int hei, CPPN::Graphics::Color color = {255, 255, 255, 255})
                : BaseShape(x, y, color), width(wid), height(hei)
            {
                rect.x = this->x;
                rect.y = this->y;
                rect.w = width;
                rect.h = height;
            }

            virtual void update() override
            {
                rect.x = static_cast<int>(x);
                rect.y = static_cast<int>(y);
                rect.w = width;
                rect.h = height;
            }
            void draw(SDL_Renderer *ren) const override
            {
                SDL_RenderFillRect(ren, &rect);
            }
            bool isColliding(int x, int y) const override {
                return (rect.x <= x && x <= rect.x + rect.w) && (rect.y <= y && y <= rect.y + rect.h);
            }
        };
        class Oval : public BaseShape
        {
        public:
            int width;
            int height;

            Oval(int x, int y, int wid, int hei, CPPN::Graphics::Color color = {255, 255, 255, 255})
                : BaseShape(x, y, color), width(wid), height(hei) {}

            virtual void update() override {
                // No need for a rect; use width and height directly
            }

            void draw(SDL_Renderer *ren) const override {
                int x_c = x; // Center x
                int y_c = y; // Center y
                int a = width / 2; // Horizontal radius
                int b = height / 2; // Vertical radius

                for (int dy = -b; dy <= b; ++dy) {
                    int dx = static_cast<int>(a * sqrt(1.0 - (dy * dy) / static_cast<double>(b * b)));
                    SDL_RenderDrawLine(ren, x_c - dx, y_c + dy, x_c + dx, y_c + dy);
                }
            }

            bool isColliding(int px, int py) const override {
                int x_c = x; // Center x
                int y_c = y; // Center y
                int a = width / 2; // Horizontal radius
                int b = height / 2; // Vertical radius

                // Check if the point satisfies the ellipse equation
                double dx = static_cast<double>(px - x_c) / a;
                double dy = static_cast<double>(py - y_c) / b;
                return (dx * dx + dy * dy) <= 1.0;
            }
        };
 
    }
} // namespace CPPN
