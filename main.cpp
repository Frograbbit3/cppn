#include <iostream>
#include "cppn.h"
using namespace CPPN;
#include <sys/resource.h>


// Core::Init and object constructions moved into main()

void printMemoryUsage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    std::cout << "Memory usage: " << usage.ru_maxrss << " KB" << std::endl;
}


int main() {
    Core::Init(800,600,"men");
    Graphics::RectangleProperties prop2;
    prop2.fill = Graphics::Color(255,0,0,255);
    prop2.strokeWidth = 25;
    Graphics::Rectangle mainPlayer(20, 20, prop2);
    Graphics::OvalProperties prop;
    prop.width = 50;
    prop.height = 25;
    Graphics::Oval player2(40, 40, prop);
    Graphics::Image exampleImage("/home/nateb/Desktop/cppn/assets/file.png",0,0,64,64);
    exampleImage.draggable = true;
    mainPlayer.draggable = true;
    player2.draggable = true;

    //create core functions
    
    Core::AssignMacro("tick", [&exampleImage,&mainPlayer, &player2]() {
        exampleImage.rotation += 1.0f;
        mainPlayer.rotation += 1.0f;
       // player2.rotation -= 2.5f;
    });
    

    Core::Run();
}

