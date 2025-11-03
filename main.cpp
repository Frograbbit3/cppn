#include "cppn.h"


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
    shape.draggable = true;

    
    Core::AssignMacro(CPPN::Enums::Event::ON_TICK, [&shape]() {
        shape.transforms.rotation+=5;
        shape.cache(); //required in 99.9% of cases except rotation
    });
    Core::Run();
}