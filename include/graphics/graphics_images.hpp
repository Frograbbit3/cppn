#pragma once
#include "SDL2/SDL_image.h"
#include "graphics_general.hpp"
#include "graphics_core.hpp"
#include <string>
#include <iostream>

namespace CPPN
{
    namespace Graphics
    {
        class Image : public BaseShape
        {
        private:
            SDL_Texture *texture;

        public:
            int width;
            int height;

            Image(const std::string &path, int x, int y, int wid = 0, int hei = 0)
                : BaseShape(x, y), texture(nullptr), width(wid), height(hei)
            {
                texture = CPPN::Graphics::LoadImage(path);
                if (!texture) {
                    std::cerr << "Image: failed to load texture for '" << path << "'\n";
                    return;
                }

                if (width == 0 || height == 0) {
                    int tw = 0, th = 0;
                    if (SDL_QueryTexture(texture, nullptr, nullptr, &tw, &th) == 0) {
                        if (width == 0) width = tw;
                        if (height == 0) height = th;
                    }
                }
            }

            ~Image() override {
                if (texture) SDL_DestroyTexture(texture);
            }

            virtual void update() override {}

            void draw(SDL_Renderer *ren) const override
            {
                if (!texture) return;
                SDL_Rect dst{ x, y, width, height };
                SDL_RenderCopy(ren, texture, nullptr, &dst);
            }

            bool isColliding(int px, int py) const override
            {
                return (px >= x && px <= x + width) && (py >= y && py <= y + height);
            }
        };

    }
}