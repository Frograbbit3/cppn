#include "core/core.hpp"
#include "shapedesigner/core.hpp"



using namespace CPPN;
using namespace CPPN::ShapeDesigner;
int main() {
    
    Core::Init(800, 800, "test");

    Shape shape;
    shape.fillColor= Color(128,128,255,255);
    shape.position.x = 50;
    shape.position.y = 50;
    shape.size.width = 100;
    shape.size.height = 150;
    shape.shape = ShapeTypes::OVAL;
    shape.points = {
        Vector2{0,0},
        Vector2{25,0},
        Vector2{63,25},
        Vector2{12,52}
    };

    //boilerplate for testing
    SDL_Texture* text = ConvertShapeToTexture(&shape);
    SDL_Rect rct{shape.position.x, shape.position.y, shape.size.width, shape.size.height};
    bool running = true;
    SDL_Event e;       
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;
            if (e.type == SDL_MOUSEMOTION) {
                std::cout << "(" << e.motion.x << "," << e.motion.y << ")"<<std::endl;
            }
        }

        SDL_SetRenderDrawColor(CPPN::Graphics::renderer, 20, 20, 20, 255);
        SDL_RenderClear(CPPN::Graphics::renderer);
        SDL_RenderCopyEx(CPPN::Graphics::renderer, text, nullptr, &rct, shape.transforms.rotation, NULL, SDL_FLIP_NONE);
        shape.transforms.rotation+=5;
        // draw something

        SDL_RenderPresent(CPPN::Graphics::renderer);
    }

    //Core::Run();
}