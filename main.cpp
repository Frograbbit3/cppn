#include <iostream>
#include "cppn.h"
using namespace CPPN;



int main() {
    Core::Init(800,600,"men");

    //create core functions
    Graphics::Color red(255,0,0,255);
    Graphics::Rectangle testRect(20, 20, 50, 50, red);

    Core::AssignMacro("mousedrag", [&testRect]() {
        testRect.setPosition(CPPN::Input::mouseX, CPPN::Input::mouseY);
    });

    

    Core::Run();
}

