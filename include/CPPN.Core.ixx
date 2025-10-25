export module CPPN.Core;
import CPPN.Graphics.Settings; // import the parent graphics module instead of the missing submodule


/*
        Main startup function for creating the CPPN window and setting up events. This *must be called* to start the project.
        @param EngineSettings The settings for creating the core game engine. This also includes the WindowSettings.
*/
export void InitEngine
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL failed to init!" << std::endl;
    }
    CPPN::GraphicsPrivate::InitRenderer(EngineSettings.WINDOW_SETTINGS);
}
module :private;
