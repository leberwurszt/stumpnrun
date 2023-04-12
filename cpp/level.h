#pragma once

#include <iostream>
#include <cstdint>
#include <string>
#include <fstream>

// SDL libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#define LAYERS 3
#define LAYER_MAIN 1


// Header definitions
#define LEVEL_HEADER_LENGTH 0x0400

#define LEVEL_LAYER0_SIZE_X 0x0000
#define LEVEL_LAYER0_SIZE_Y 0x0004
#define LEVEL_LAYER1_SIZE_X 0x0008
#define LEVEL_LAYER1_SIZE_Y 0x000C
#define LEVEL_LAYER2_SIZE_X 0x0010
#define LEVEL_LAYER2_SIZE_Y 0x0014
#define LEVEL_TITLE_STRING_BEGIN 0x0020
#define LEVEL_TITLE_STRING_LENGTH 0x0040
#define LEVEL_TILESET_STRING_BEGIN 0x0060
#define LEVEL_TILESET_STRING_LENGTH 0x0040

#define DEFAULT_LAYER0_SIZE_X 0
#define DEFAULT_LAYER0_SIZE_Y 0
#define DEFAULT_LAYER1_SIZE_X 16
#define DEFAULT_LAYER1_SIZE_Y 16
#define DEFAULT_LAYER2_SIZE_X 0
#define DEFAULT_LAYER2_SIZE_Y 0

#define DEFAULT_LAYER_SIZE_X 64
#define DEFAULT_LAYER_SIZE_Y 64

// size of tileset
#define TILESET_WIDTH 4
#define TILESET_HEIGHT 64

// tile size
#define TILE_SIZE_X 64
#define TILE_SIZE_Y 64

struct layer
{
    uint8_t** tile = NULL;
    bool borderless = false;
    uint32_t sizeX = DEFAULT_LAYER_SIZE_X;
    uint32_t sizeY = DEFAULT_LAYER_SIZE_Y;
};

class Level
{
    protected:
        layer* layers = NULL;

        uint8_t** mask = NULL;

        std::string title =	""; 
        std::string tileset = "";


    public:
        Level();
        ~Level();

        // get methods
		std::string GetTitle();
		std::string GetTileset();

        uint8_t GetTile(uint8_t layer, uint16_t x, uint16_t y);
        bool GetMaskStateFromCoord(uint32_t x, uint32_t y);
        uint8_t** GetMask();
        uint32_t GetSizeX();
        uint32_t GetSizeY();

		// set methods
		void SetTitle(std::string title);
		void SetTileset(std::string tileset);
		void SetTile(uint8_t layer, uint16_t x, uint16_t y, uint8_t val);
        void SetMask(uint8_t** mask);

		// other methods
		void New();
        void CreateLayer(uint8_t layer, uint16_t x, uint16_t y);
		void Resize(uint8_t layer, uint16_t x, uint16_t y);
		void ClearLayer(uint8_t layer);
        void DeleteLayer(uint8_t layer);
        bool Load(std::string filename);
        bool Save(std::string filename);

        void DeleteMask();

        void CreateDefaultCollisionMask();

        bool LayerIsBorderless(uint8_t layer);
        void LayerSetBorderless(uint8_t layer, bool borderless);
};