#include "cppn.h"
using namespace CPPN;
int main() {
    Core::Init(800, 800, "platformer");

    Graphics::RectangleProperties floorprop;
    floorprop.setFill(Color(128,255,128,255));
    Graphics::Rectangle floor(0,700,800,100, floorprop);

    Graphics::OvalProperties playerprop;
    playerprop.setFill(Color(255,128,128,255))
    .setSize(50,50);
    Graphics::Oval player(400, 200, playerprop);

    int yV = 0;
    float vX = 0;

    Core::AssignMacro(Enums::Event::ON_KEY_HOLD, [&vX, &yV]() {
        for (auto &key: Input::keysPressed) {
         //   std::cout << key <<std::endl;
            if (Input::keysPressed.find("W") != Input::keysPressed.end()) {
                yV = -15;
            }
            if (Input::keysPressed.find("A") != Input::keysPressed.end()) {
                vX = -15;
            }
            if (Input::keysPressed.find("D") != Input::keysPressed.end()) {
                vX = 15;
            }
        }
    });

    Core::AssignMacro(Enums::Event::ON_TICK, [&player,&floor, &yV, &vX]() {
        if (!player.isCollidingShape(floor)) {
            yV += 1;
            player.y += yV;
        }else{
            if (yV >= 0) {
                player.y -= yV;
                yV = 0;
            }else{
                player.y+=yV;
            }
        }
        player.x += std::floor(vX);
        vX *= 0.8;
    });

    Core::Run();

}