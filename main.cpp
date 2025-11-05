#include "core/core.hpp"
#include "core/macros.hpp"
#include "cppn.h"
#include "audio/audio_core.hpp"
#include "enums/enums.hpp"
#include "filesystem/filesystem_core.hpp"
#include "shapedesigner/defines.hpp"


using namespace CPPN;

int main() {
    Core::Init(8000, 8000, "example");
    FileSystem::Init("example", "example");
    Audio::Init();
    Audio::Sound jump("jump.mp3");
    Graphics::background = {135, 206, 235,255};
    int vY = 0;
    Shape bird;
    bird.fillColor = {255,255,0,255};
    bird.size = {50, 50};
    bird.position = {100, 400};
    bird.shape = ShapeTypes::OVAL;
    bird.cache();
    Core::AssignMacro(Event::ON_KEY_PRESS, [&jump, &vY]() {
        jump.play();
        vY = -17;
    });

    Core::AssignMacro(Event::ON_TICK,[&bird, &vY]() {
        bird.position.y+=vY;
        vY += 1;
    });

    Core::Run();
}