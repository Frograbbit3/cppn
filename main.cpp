#include <iostream>
#include "cppn.h"
using namespace CPPN;



int main() {
    Core::Init(800,600,"men");

    //create core functions
    

    Core::AssignMacro("tick", [](){
        Graphics::Color red =Graphics::Color((int)rand()%255,(int)rand()%255,(int)rand()%255,(int)rand()%255);
        Graphics::Color blue =Graphics::Color((int)rand()%255,(int)rand()%255,(int)rand()%255,(int)rand()%255);
        Graphics::Rectangle* testRect = new Graphics::Rectangle((int)rand()%800, (int)rand()%600, 50, 50, red);
        Graphics::Rectangle* testRect2 = new Graphics::Rectangle((int)rand()%800, (int)rand()%600, 100, 50, blue);
        testRect2->draggable = true;
        testRect->draggable = true;
    });
    

    Core::Run();
}

