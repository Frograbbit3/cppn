#include <iostream>
#include "include/cppn.h"
#include "include/graphics/graphics_shapes.hpp"
#include "include/graphics/graphics_svg.hpp"
#include "include/graphics/graphics_utils.hpp"
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
    prop2.setCornerRadius(5)
    .setStrokeWidth(15)
    .setStroke(Color(255,0,255,255));
    Graphics::Rectangle rect(50,50,150, 150,prop2);

    Graphics::OvalProperties prop1;
    prop1.setFill(Color(128,65,255,255))
    .setStroke(Color(195,255,15,255))
    .setStrokeWidth(15)
    .setSize(75, 150);
    Graphics::Oval oval1(250,50,prop1);

    rect.draggable=true;
    oval1.draggable=true;


    //create core functions
    
    

    Core::Run();
}

