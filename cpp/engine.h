#pragma once

#include <iostream>
#include <cstdint>
#include <cstdio>
#include <list>


// SDL libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "level.h"
#include "entity.h"
#include "player.h"

#define VIEWSIZE_X screenSizeX / tilesizeX
#define VIEWSIZE_Y screenSizeY / tilesizeY

#define FLAG_EDITOR 0x00000008

#define RMASK 0x000000ff
#define GMASK 0x0000ff00
#define BMASK 0x00ff0000
#define AMASK 0xff000000

class Engine
{
    protected:
        bool quit = false;
        bool editor = false;

        // SDL
        SDL_Renderer* render = NULL;
        SDL_Event event;
        SDL_DisplayMode mode;
        int screenRefresh;
        int screenSizeX, screenSizeY;

        SDL_Rect imagePosition;

        SDL_Rect tile[256];                        // rectangles for particular textures of the tileset
        SDL_Rect spritePlayer[256];

        // mouse
        SDL_Point mousePos;
        uint16_t mouseTileX = 0;
        uint16_t mouseTileY = 0;
        uint32_t mouseScrollTime = 1;
        uint32_t mouseScrollLast = 0;

        // editor
        uint8_t editor_layer = 1;
        uint8_t editor_tile = 1;

        // Textures
        SDL_Texture* textureTile = NULL;
        SDL_Texture* texturePlayer = NULL;

        SDL_Point camera = { 0, 0 };

        uint16_t tilesizeX = TILE_SIZE_X;
        uint16_t tilesizeY = TILE_SIZE_Y;

        Uint64 ticksLastFrame = 0;
        Uint64 ticksCurrentFrame = 0;
        Uint64 ticksSinceLastFrame = 0;

        Level* level = NULL;
        Player* player = NULL;

        std::list<Entity*> entities;

        void Controls();
        void ControlsGame();
        void ControlsEditor();

    public:
        Engine(SDL_Window* window, uint32_t flags);
        ~Engine();

        int Run();
        void RunEntities();
        bool LoadTextures();
        bool DrawLevel();

        uint8_t** CreateCollisionMaskFromSurface(SDL_Surface* surface);

        void MoveCamera();


};
