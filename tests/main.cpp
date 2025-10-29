#include <any>
#include <iostream>
#include <string>
#include "cppn.h"
using namespace CPPN;
#include <sys/resource.h>
#include <any>




int main() {
    //Core stuff
    Core::Init(800,800, "Testing Window");


    //filesystem
    FileSystem::Init("Testing Company", "Testing Game");
    std::cout << "[Save Path]:" << FileSystem::SAVE_PATH << std::endl;
    std::cout << "[Resources Path]:" << FileSystem::RESOURCE_PATH << std::endl;
    //Shape properties
    Graphics::RectangleProperties testRect;
    testRect
    .setCornerRadius(25)
    .setFill(Graphics::Color(255,128,255,128))
    .setSize(128,128)
    .setStroke(Graphics::Color(25,25,182,255))
    .setStrokeWidth(15);

    Graphics::Rectangle mainRect (50, 50, 50, 50, testRect);
    mainRect.draggable = true;

    Graphics::OvalProperties testOval;
    testOval
    .setFill(Graphics::Color(64,218,47, 255))
    .setStroke(Graphics::Color(85,27,14, 255))
    .setStrokeWidth(8)
    .setSize(50, 100);

    Graphics::Oval mainOval(150, 150, testOval);
    mainOval.draggable=true;

    //Image
    Graphics::Image f("file.png", 200, 50, 100, 100);
    f.draggable = true;


    //Events
    Core::AssignMacro(Enums::Event::ON_TICK, [&mainRect, &mainOval, &f]() {
        mainRect.rotation+=1.0f;
        mainOval.rotation+=1.0f;
        f.rotation+=1.0f;
    });

    Core::AssignMacro(Enums::Event::ON_KEY_PRESS, []() {
        std::cout << "[Key press event, keys held]:(";
        for (auto &m : Input::keysPressed) { 
            std::cout << m << ",";
        }
        std::cout << ")"<<std::endl;
    });

    Core::AssignMacro(Enums::Event::ON_KEY_RELEASE, []() {
        std::cout << "[Key release event, keys held]:(";
        for (auto &m : Input::keysPressed) { 
            std::cout << m << ",";
        }
        std::cout << ")"<<std::endl;
    });

    Core::AssignMacro(Enums::Event::ON_KEY_HOLD, []() {
        std::cout << "[Key hold event, keys held]:(";
        for (auto &m : Input::keysPressed) { 
            std::cout << m << ",";
        }
        std::cout << ")"<<std::endl;
    });

    //Main loop
    Core::Run();
}
