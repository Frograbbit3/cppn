#include <iostream>
#include "cppn.h"
using namespace CPPN;


void onTick() {
    std::cout << "hello, world!" << std::endl;
}

int main() {
    Core::Init(800,600,"men");

    //create core functions
    std::function<void()> tick = onTick;
    Core::AssignMacro("tick", tick);

    //run
    Core::Run();
}