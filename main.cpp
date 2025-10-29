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




int main() {
    Core::Init(800,600,"men");
    FileSystem::Init("Example Company", "Example Name");
    std::cout << FileSystem::SAVE_PATH << std::endl;
    FileSystem::SaveData save;

    

    Core::Run();
}

