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

    //saving
    FileSystem::SaveData saved;
    if (FileSystem::FileExists(FileSystem::AbsoluteSavesPath("file.txt"))) {
        std::cout << "found file" << std::endl;
        saved = FileSystem::OpenSaveFile(FileSystem::AbsoluteSavesPath("file.txt"));
        std::cout << "loaded file" << std::endl;
    }else{
        std::cout << "starting from scratch" << std::endl;
        saved.set("main", "rotation",static_cast<double>(0.0f));
        saved.set("main", "x",static_cast<int>(0));
        saved.set("main", "y",static_cast<int>(0));
        std::cout << "finished init" << std::endl;
    }

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
    Graphics::Image f("file.png", saved.get<int>("main", "x"), saved.get<int>("main", "y"), 100, 100);
    f.draggable = true;
    std::cout << "rotation is " << saved.get<double>("main", "rotation") << std::endl;
    f.rotation = saved.get<double>("main", "rotation");

    //Events
    Core::AssignMacro(Event::ON_TICK, [&mainRect, &mainOval, &f]() {
        mainRect.rotation+=1.0f;
        mainOval.rotation+=1.0f;
      //  f.rotation+=1.0f;
    });

    Core::AssignMacro(Event::ON_MOUSE_DRAG, [&saved, &f](){
        saved.set("main", "x", f.x);
        saved.set("main", "y", f.y);
    });
    Core::AssignMacro(Event::ON_KEY_PRESS, [&saved]() {
        std::cout << "[Key press event, keys held]:(";
        for (auto &m : Input::keysPressed) { 
            std::cout << m << ",";
            if (m == "A"){
                std::cout << std::endl << "saved" << std::endl;
                FileSystem::WriteSaveFile("file.txt", saved);
            }
        }
        std::cout << ")"<<std::endl;
    });

    Core::AssignMacro(Event::ON_KEY_RELEASE, []() {
        std::cout << "[Key release event, keys held]:(";
        for (auto &m : Input::keysPressed) { 
            std::cout << m << ",";
        }
        std::cout << ")"<<std::endl;
    });

    Core::AssignMacro(Event::ON_KEY_HOLD, [&f, &saved]() {
        std::cout << "[Key hold event, keys held]:(";
        for (auto &m : Input::keysPressed) { 
            std::cout << m << ",";
            if (m == "Right") {
                f.rotation+=2.5f;
            }
            if (m == "Left") {
                f.rotation-=2.5f;
            }
            saved.set("main", "rotation", f.rotation);
        }
        std::cout << ")"<<std::endl;
    });

    //Main loop
    Core::Run();
}
