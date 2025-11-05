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
    background->points = {{20, 400}, {40, 600}, {20,200}};

    Shape* label = new Shape();
    label->fillColor = {255,255,255,255};
    label->value = value;
    label->size = {500, 500};
    label->shape = ShapeTypes::LABEL;
    label->value = value;  
    background->AddChild(label);
    background->draggable = true;
    

    background->cache();
    // You may want to store 'label' somewhere or attach it to 'background' if your framework supports it.
    return background;
}
int main() {
    
    Core::Init(800, 800, "test");

    FileSystem::Init("example", "example");
    FileSystem::IniParser example("example.ini");
    FileSystem::SaveData save;
    Shape* m = CreateButton(50, 50, "Hello world");
    
    Core::AssignMacro(Event::ON_READY, [&example, &save, &m]() {
        example.load();
        if (!example.exists("Rect1", "PositionX")) {
            std::string name;
            std::cout << "What's your name? ";
            std::cin >> name;
            example.set("General", "Name", name);
            std::cout << "loaded defaults" << std::endl;
            example.set("Rect1", "PositionX", 0);
            example.set("Rect1", "PositionY", 0);
            example.set("Rect1", "Rotation", 0.0);
            example.save();
        }
    
        m->position.x = example.get<int>("Rect1", "PositionX");
        m->position.y = example.get<int>("Rect1", "PositionY");
        m->transforms.rotation=example.get<double>("Rect1", "Rotation");
        m->children[0]->value=example.get<std::string>("General", "Name");
        m->cache();
        //example.save();
    });
    
    Core::AssignMacro(Event::ON_MOUSE_CLICK, [&m, &example]() {
        example.set("Rect1", "PositionX", m->position.x);
        example.set("Rect1", "PositionY", m->position.y);
        example.set("Rect1", "Rotation", m->transforms.rotation);
        example.save();
        std::cout << "saved!" << std::endl;
    });

    Core::AssignMacro(Event::ON_KEY_HOLD, [&m]() {
        if (Input::keysPressed.find("D") != Input::keysPressed.end()) {
            m->transforms.rotation+=5;
            m->cache();
        }
    });



    
    Core::Run();
}