#include "cppn.h"


using namespace CPPN;
using namespace CPPN::ShapeDesigner;

Color red = {255,0,0,255};
Color blue = {0,0,255,255};  // Lighter blue for better visibility


Shape* CreateButton(int x, int y, std::string value) {
    Shape* background = new Shape();
    background->fillColor = red;
    background->position = {x,y};
    background->size = {300, 75};
    background->shape = ShapeTypes::RECTANGLE;

    Shape* label = new Shape();
    label->fillColor = {255,255,255,255};
    label->value = value;
    label->shape = ShapeTypes::LABEL;  
    background->AddChild(label);
    background->draggable = true;
    

    background->cache();
    // You may want to store 'label' somewhere or attach it to 'background' if your framework supports it.
    return background;
}
int main() {
    
    Core::Init(800, 800, "test");
    Shape* m = CreateButton(50, 50, "Hello world");
    Core::AssignMacro(Event::ON_TICK, [&m]() {
        m->transforms.rotation+=5;
        if (m->IsColliding(Input::mouseX, Input::mouseY)) {
            m->fillColor=blue;
        }else{
            m->fillColor=red;
        }
    });
    Core::Run();
}