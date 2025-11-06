#define NO_AUTO_CACHE
#include "cppn.h"


using namespace CPPN;

int main() {
    Core::Init(400, 400, "example");
    FileSystem::Init("example", "example");
    Audio::Init();

    Shape path;
    path.fillColor =  {0,0,0,255};
    Vector2 base {0,0};
    path.points = {base};
    path.size = {400, 400};
    path.shape = ShapeTypes::POLYGON;

    Core::AssignMacro(Event::ON_MOUSE_RELEASE, [&path](){
        Vector2 v({Input::mouseX,Input::mouseY});
        std::cout << fmt::format("({},{})", Input::mouseX, Input::mouseY) <<std::endl;
        path.points.emplace_back(v); // Default-construct Vector2
    });

    Core::AssignMacro(Event::ON_MOUSE_MOVE, [&path]() {
        path.points[path.points.size()-1].x = Input::mouseX;
        path.points[path.points.size()-1].y = Input::mouseY;
    });
    Core::AssignMacro(Event::ON_KEY_PRESS, [&path](){
        if (Input::keysPressed.find("Z") != Input::keysPressed.end()) {
            if (!path.points.empty()) {
                path.points.erase(path.points.end() - 1);
            }
        }
    });
    Core::Run();
}