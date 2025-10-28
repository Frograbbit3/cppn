#include <iostream>
#include "cppn.h"
using namespace CPPN;
#include <sys/resource.h>


// Core::Init and object constructions moved into main()
Graphics::Oval mainPlayer(20, 20, 50, 50);

void printMemoryUsage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    std::cout << "Memory usage: " << usage.ru_maxrss << " KB" << std::endl;
}


int main() {
    Core::Init(800,600,"men");
    Graphics::Image exampleImage("/home/nateb/Desktop/cppn/bin/file.png",0,0,0,0);
    exampleImage.draggable = true;
    mainPlayer.draggable = true;

    //create core functions
    

    

    Core::Run();
}

