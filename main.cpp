#include "core/core.hpp"
#include "shapedesigner/core.hpp"



using namespace CPPN;
using namespace CPPN::ShapeDesigner;
int main() {
    
    Core::Init(400, 400, "test");

    Shape shape;
    shape.fillColor= Color(128,128,255,255);
    shape.position.x = 50;
    shape.position.y = 25;
    shape.size.width = 25;
    shape.size.height = 50;
    shape.shape = ShapeTypes::CIRCLE;

    //boilerplate for testing
    SDL_Texture* text = ConvertShapeToTexture(&shape);
    SDL_Rect rct{shape.position.x, shape.position.y, shape.size.width, shape.size.height};
    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;
        }

        SDL_SetRenderDrawColor(CPPN::Graphics::renderer, 20, 20, 20, 255);
        SDL_RenderClear(CPPN::Graphics::renderer);
        SDL_RenderCopy(CPPN::Graphics::renderer, text, nullptr, &rct);

        // draw something

        SDL_RenderPresent(CPPN::Graphics::renderer);
    }

    //Core::Run();
}