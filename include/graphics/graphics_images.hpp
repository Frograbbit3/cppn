#pragma once
#include "SDL2/SDL_image.h"
#include "graphics_general.hpp"
#include "../filesystem/filesystem_core.hpp"
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

            /*
                Creates an image which is rendered to the screen. This does not requires a properties struct.

                @param path The path to the resource data. Will become absolute if an absolute path is not provided.
                @param x The x position to place the upper left corner.
                @param y The y position to place the upper left corner.
                @param width An optional parameter that resizes the image to fit within that width.
                @param height An optional parameter that resizes the image to fit within that height.
            */
            Image(const std::string &path, int x, int y, int wid = 0, int hei = 0)
                : BaseShape(x, y), texture(nullptr), width(wid), height(hei)
            {
                texture = CPPN::Graphics::LoadImage(FileSystem::AbsoluteResourcePath(path));
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


            /*
                @deprecated Not required for images.
            */
            virtual void update() override {}
            /*
                Draws the image to the screen. Not recommended to manually call.
                @param ren Use CPPN::Graphics::renderer.
            */
            void draw(SDL_Renderer *ren) const override
            {
                if (!texture) return;
                SDL_Rect dst{ x, y, width, height };
                SDL_RenderCopyEx(ren, texture, nullptr, &dst, rotation, nullptr, SDL_FLIP_NONE);
            }


            /*
                Checks if the image collides with a point.
                
                @note This does AABB collision as pixel perfect is unsupported right now.
            */
            bool isColliding(int px, int py) const override
            {
                return (px >= x && px <= x + width) && (py >= y && py <= y + height);
            }
        };

    }
}