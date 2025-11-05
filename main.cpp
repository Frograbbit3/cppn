#include "core/core.hpp"
#include "core/macros.hpp"
#include "cppn.h"
#include "audio/audio_core.hpp"
#include "enums/enums.hpp"
#include "filesystem/filesystem_core.hpp"


using namespace CPPN;

int main() {
    Core::Init(800, 800, "example");
    FileSystem::Init("example", "example");
    Audio::Init();
    Audio::Sound jump("jump.mp3");
    Core::AssignMacro(Event::ON_KEY_PRESS, [&jump]() {
        jump.play();
    });

    Core::Run();
}