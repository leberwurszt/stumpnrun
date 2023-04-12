// main.cpp
//
// StumpNRun

#include "main.h"

int main(int argc, char *argv[])
{
    Engine* engine;
    uint32_t flags = 0x00000000;
    

    // search arguments
    for(int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if(arg == "-e" || arg == "--edit")
            flags |= FLAG_EDITOR;
    }


    // initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1)
    {
        std::cerr << SDL_GetError() << std::endl;
        return -1;
    }

    // set texture filtering to linear
    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0" ))
        std::cerr << "Warning: Linear texture filtering not enabled!" << std::endl;

    // initialize SDL_mixer for sounds
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return -1;
    }

    // initialize window
    SDL_Window* window = SDL_CreateWindow("Stump & Run", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_OPENGL);
    if (!window)
    {
        std::cerr << SDL_GetError() << std::endl;
        return -1;
    }

    // set window to fullscreen
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    // initialize engine

    engine = new Engine(window, flags);

    engine->Run();

    delete engine;
    SDL_DestroyWindow(window);

    std::cout << "...and the stumpgirl said \"lol\"" << std::endl;

    return 0;
}