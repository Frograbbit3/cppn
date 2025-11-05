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
    Audio::Sound test("test.mp3");
    Audio::Sound test2("test2.mp3");
    test.play();
    test2.play();

    Core::AssignMacro(CPPN::Enums::Event::ON_TICK, []() {
        Audio::Tick();
    });
    Core::Run();
}