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
    background->shape = ShapeTypes::OVAL;
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
    /*
    Shape* m = CreateButton(50, 50, "Hello world");
    Core::AssignMacro(Event::ON_TICK, [&m]() {
        m->transforms.rotation+=1;
        if (m->IsColliding(Input::mouseX, Input::mouseY)) {
            m->fillColor=blue;
        }else{
            m->fillColor=red;
        }
        m->cache();
        if (m->children[0]->IsColliding(Input::mouseX, Input::mouseY)) {
            m->children[0]->fillColor=red;
        }else{
            m->children[0]->fillColor=blue;
            m->children[0]->cache();
        }
    });
    */
    FileSystem::Init("example", "example");
    FileSystem::IniParser example("example.ini");
    FileSystem::SaveData save;
    Core::AssignMacro(Event::ON_READY, [&example, &save]() {
        example.set("General", "LogFile", "/fake/new/location");
        save.set("General", "LogFile", "/fake/new/location");
        std::cout << "The log file is now located at " << example.get<std::string>("General", "LogFile") << std::endl;
        example.save();
        FileSystem::WriteSaveFile("save.txt", save);
    });
    
    Core::Run();
}