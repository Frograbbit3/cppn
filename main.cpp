#include <any>
#include <iostream>
#include "include/core/input_core.hpp"
#include "include/core/macros.hpp"
#include "include/cppn.h"
#include "include/filesystem/filesystem_core.hpp"
#include "include/graphics/graphics_images.hpp"
#include "include/graphics/graphics_shapes.hpp"
#include "include/graphics/graphics_svg.hpp"
#include "include/graphics/graphics_utils.hpp"
using namespace CPPN;
#include <sys/resource.h>
#include <any>




int main() {
    Core::Init(800,600,"men");
    FileSystem::Init("Example Company", "Example Name");
    std::cout << FileSystem::SAVE_PATH << std::endl;
    FileSystem::SaveData save;
    Graphics::RectangleProperties r;
    r.setFill(Color(255,0,0)).setCornerRadius(15);
    Graphics::Rectangle rect (150, 250, 50, 250, r);
    Graphics::Rectangle rect2 (350, 250, 50, 250, r);
    if (FileSystem::FileExists(FileSystem::AbsoluteSavesPath("file.txt"))) {
        save=FileSystem::OpenSaveFile(FileSystem::AbsoluteSavesPath("file.txt"));
    }else{
        save.set("records", "highest", 0);
    }
    Core::AssignMacro("mousedown", [&rect2, &rect, &save]() {
        if (rect.isColliding(CPPN::Input::mouseX,CPPN::Input::mouseY)) {
            save.set("records", 
            "highest", 
            std::any_cast<int>(save.get("records", "highest")) + 1
        );
            std::cout << std::any_cast<int>(save.get("records", "highest")) <<std::endl;
        }else if (rect2.isColliding(CPPN::Input::mouseX, CPPN::Input::mouseY)) {
            FileSystem::WriteSaveFile("file.txt", save);
            std::cout <<"saved" <<std::endl;
        }
    });
    

    Core::Run();
}

