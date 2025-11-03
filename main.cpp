#include "cppn.h"


using namespace CPPN;
using namespace CPPN::ShapeDesigner;

Color red = {255,0,0,255};
Color blue = {0,0,255,255};  // Lighter blue for better visibility


Shape* CreateButton(int x, int y, std::string value) {
    Shape* background = new Shape();
    background->fillColor = red;
    background->position.x = x;
    background->position.y = y;
    background->size.width = 300;
    background->size.height = 75;
    background->shape = ShapeTypes::RECTANGLE;

    Shape* label = new Shape();
    label->fillColor = {255,255,255,255};
    label->position.x = background->position.x + background->size.width/4;
    label->position.y = background->position.y + background->size.height/4;
    label->value = value;
    label->shape = ShapeTypes::LABEL;

    background->cache();
    label->cache();
    // You may want to store 'label' somewhere or attach it to 'background' if your framework supports it.
    return background;
}
int main() {
    
    Core::Init(800, 800, "test");
    Shape* m = CreateButton(50, 50, "Hello world");
    m->OnClick = [&m](int x, int y) {
        m->fillColor=blue;
    };
    m->OnRelease = [&m](int x, int y) {
        m->fillColor=red;
    };
    Core::Run();
}