#include "cppn.h"


using namespace CPPN;
using namespace CPPN::ShapeDesigner;

Color red = {255,0,0,255};
Color blue = {0,0,255,255};  // Lighter blue for better visibility
int main() {
    
    Core::Init(800, 800, "test");

    Shape shape;
    shape.fillColor= Color(128,128,255,255);
    shape.position.x = 50;
    shape.position.y = 50;
    shape.size.width = 150;
    shape.size.height = 100;
    shape.shape = ShapeTypes::RECTANGLE;
    shape.points = {
        Vector2{0,0},
        Vector2{25,0},
        Vector2{63,25},
        Vector2{12,52}
    };
    shape.draggable = true;

    
    Shape newshape;
    newshape.fillColor = Color(255, 255, 255, 255);  // White text
    newshape.shape = ShapeTypes::LABEL;
    newshape.value = "hello, world!";  // Set value BEFORE cache()
    newshape.position.x = 200;
    newshape.position.y = 200;
    newshape.draggable = true;
    
    // Cache after all properties are set
    shape.cache();
    newshape.cache();


    shape.OnClick = [](int offX, int offY) {
        std::cout << "click" << std::endl;
    };
    shape.OnRelease = [](int offX, int offY) {
        std::cout << "declick" << std::endl;
    };
    Core::AssignMacro(CPPN::Enums::Event::ON_TICK, [&shape, &newshape]() {
        if (shape.IsCollidingShape(&newshape)) {
            shape.fillColor=red;
        }else{
            shape.fillColor=blue;
        }
        shape.position.x=Input::mouseX-shape.size.width/2;
        shape.position.y=Input::mouseY-shape.size.height/2;
        shape.cache(); //required in 99.9% of cases except rotation
    });
    Core::Run();
}