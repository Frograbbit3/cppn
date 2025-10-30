#include "cppn.h"
#include "shapedesigner/core.hpp"



using namespace CPPN;
using namespace CPPN::ShapeDesigner;
int main() {
    
    Core::Init(400, 400, "test");

    Shape shape;
    shape.fillColor= Color(128,128,255,255);


    //boilerplate for testing
    SDL_Texture* text = ConvertShapeToTexture(&shape);
    SDL_Rect rct{ 0, 0, 400, 400};
    SDL_RenderCopy(Graphics::renderer,text,&rct,nullptr);

    Core::Run();
}