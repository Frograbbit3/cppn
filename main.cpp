#include <iostream>
#include "include/core/macros.hpp"
#include "include/cppn.h"
#include "include/filesystem/filesystem_core.hpp"
#include "include/graphics/graphics_images.hpp"
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
    FileSystem::Init("Example Company", "Example Name");


    Graphics::RectangleProperties rectp;
    rectp
    .setFill(Color(255,0,0,255))
    .setSize(50, 50);
    Graphics::Rectangle r(50, 50, 50, 50, rectp);
    r.draggable = true;

    Graphics::Image m("file.png", 200, 200, 50, 50);
    m.draggable=true;

    Core::Run();
}

