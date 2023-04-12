#include "level.h"

Level::Level()
{
	// create layer arrays depending on predefined layer count
	layers = new layer[LAYERS];

	New();
    return;
}

Level::~Level()
{
	delete[] layers;

	DeleteMask();
    
    return;
}

std::string Level::GetTitle()
{
    return title;
}

std::string Level::GetTileset()
{
    return tileset;
}

uint8_t Level::GetTile(uint8_t layer, uint16_t x, uint16_t y)
{
	if(x < layers[layer].sizeX && y < layers[layer].sizeY)
		return layers[layer].tile[x][y];
	else if(layers[layer].borderless && layers[layer].sizeX > 0 && layers[layer].sizeY > 0)
		return layers[layer].tile[x % layers[layer].sizeX][y % layers[layer].sizeY];
	else
		return 0;
}

void Level::SetTitle(std::string title)
{
    this->title = title;
}

void Level::SetTileset(std::string tileset)
{
    this->tileset = tileset;
}

void Level::SetTile(uint8_t layer, uint16_t x, uint16_t y, uint8_t val)
{
	if(x < layers[layer].sizeX && y < layers[layer].sizeY && x >= 0 && y >= 0)
		layers[layer].tile[x][y] = val;
    return;
}

void Level::CreateLayer(uint8_t layer, uint16_t x, uint16_t y)
{
	layers[layer].sizeX = x;
	layers[layer].sizeY = y;

	layers[layer].tile = new uint8_t*[layers[layer].sizeX];

	for(uint16_t i = 0; i < layers[layer].sizeX; ++i)
		layers[layer].tile[i] = new uint8_t[layers[layer].sizeY];
    
	ClearLayer(layer);
}

void Level::New()
{
	for(uint8_t i = 0; i < LAYERS; ++i)
	{
		DeleteLayer(i);
		CreateLayer(i, DEFAULT_LAYER_SIZE_X, DEFAULT_LAYER_SIZE_Y);
	}
}

void Level::Resize(uint8_t layer, uint16_t x, uint16_t y)
{
	uint8_t sizeXOld = layers[layer].sizeX;
	uint8_t sizeYOld = layers[layer].sizeY;

	uint8_t** tileOld = layers[layer].tile;

	CreateLayer(layer, x, y);

	for(uint16_t i = 0; i < layers[layer].sizeX && i < sizeXOld; i++)
		for(uint16_t k = 0; k < layers[layer].sizeY && k < sizeYOld; k++)
			layers[layer].tile[i][k] = tileOld[i][k];
        
	for(uint16_t i = 0; i < sizeXOld; i++)
		delete[] tileOld[i];

	delete[] tileOld;
}

void Level::ClearLayer(uint8_t layer)
{
	for(uint16_t i = 0; i < layers[layer].sizeX; i++)
		for(uint16_t k = 0; k < layers[layer].sizeY; k++)
			layers[layer].tile[i][k] = 0;

}

void Level::DeleteLayer(uint8_t layer)
{
	if(layers[layer].tile)
	{
		for(uint16_t i = 0; i < layers[layer].sizeX; i++)
				delete[] layers[layer].tile[i];

		delete[] layers[layer].tile;
	}
	layers[layer].sizeX = 0;
	layers[layer].sizeX = 0;
	layers[layer].borderless = false;
}

bool Level::Load(std::string filename)
{
	bool success = true;

	uint32_t x0, x1, x2;
	uint32_t y0, y1, y2;

	std::ifstream file;
	file.open (filename, std::ios::in | std::ios::binary);
	
	if(!file)
		{
			std::cerr << "File " << filename << " does not exist!" << std::endl;
			return false;
		}

	try
	{
		// Read header
		uint8_t* header = new uint8_t[LEVEL_HEADER_LENGTH];
		file.read((char*) header, LEVEL_HEADER_LENGTH);

		// Read Level size from header
		x0 = (header[LEVEL_LAYER0_SIZE_X + 3]) | (header[LEVEL_LAYER0_SIZE_X + 2] << 8) | (header[LEVEL_LAYER0_SIZE_X + 1] << 16) | (header[LEVEL_LAYER0_SIZE_X] << 24);
		y0 = (header[LEVEL_LAYER0_SIZE_Y + 3]) | (header[LEVEL_LAYER0_SIZE_Y + 2] << 8) | (header[LEVEL_LAYER0_SIZE_Y + 1] << 16) | (header[LEVEL_LAYER0_SIZE_Y] << 24);
		
		x1 = (header[LEVEL_LAYER1_SIZE_X + 3]) | (header[LEVEL_LAYER1_SIZE_X + 2] << 8) | (header[LEVEL_LAYER1_SIZE_X + 1] << 16) | (header[LEVEL_LAYER1_SIZE_X] << 24);
		y1 = (header[LEVEL_LAYER1_SIZE_Y + 3]) | (header[LEVEL_LAYER1_SIZE_Y + 2] << 8) | (header[LEVEL_LAYER1_SIZE_Y + 1] << 16) | (header[LEVEL_LAYER1_SIZE_Y] << 24);
		
		x2 = (header[LEVEL_LAYER2_SIZE_X + 3]) | (header[LEVEL_LAYER2_SIZE_X + 2] << 8) | (header[LEVEL_LAYER2_SIZE_X + 1] << 16) | (header[LEVEL_LAYER2_SIZE_X] << 24);
		y2 = (header[LEVEL_LAYER2_SIZE_Y + 3]) | (header[LEVEL_LAYER2_SIZE_Y + 2] << 8) | (header[LEVEL_LAYER2_SIZE_Y + 1] << 16) | (header[LEVEL_LAYER2_SIZE_Y] << 24);
		
		New();
		CreateLayer(0, x0, y0);
		CreateLayer(1, x1, y1);
		CreateLayer(2, x2, y2);
		printf("0: %i x %i\n1: %i x %i\n2: %i x %i\n",x0,y1,x1,y1,x2,y2);

		// Read level title
		for(uint16_t i = 0; i < LEVEL_TITLE_STRING_LENGTH && header[LEVEL_TITLE_STRING_BEGIN + i] != '\0'; i++)
			title += header[LEVEL_TITLE_STRING_BEGIN + i];

		// Read level tileset name
		for(uint16_t i = 0; i < LEVEL_TILESET_STRING_LENGTH && header[LEVEL_TILESET_STRING_BEGIN + i] != '\0'; i++)
			tileset += header[LEVEL_TILESET_STRING_BEGIN + i];

		delete[] header;

		// Read tiles
		for(uint8_t i = 0; i < LAYERS; ++i)
			for(uint16_t k = 0; k < layers[i].sizeX; ++k)
				for(uint16_t l = 0; l < layers[i].sizeY; ++l)
					file.read((char*) &layers[i].tile[k][l], 1);


	}
	catch(const std::exception& e)
	{
		std::cerr << "Error loading level: " << e.what() << '\n';
	}

	if(file.is_open())
		file.close();

	if(success)
		std::cout << "Level loaded successfully." << std::endl;

	return success;
}

bool Level::Save(std::string filename)
{
	bool success = true;
	std::ofstream file;
	file.open(filename, std::ios::binary | std::ios::out);

	try
	{
		// header char array
		char* header = new char[LEVEL_HEADER_LENGTH];

		// clear header array
		for(uint32_t i = 0; i < LEVEL_HEADER_LENGTH; ++i)
			header[i] = 0x00;

		// Convert Level Size Layer 0 X
		header[LEVEL_LAYER0_SIZE_X + 3] = (layers[0].sizeX & 0x000000ff) >>  0;
		header[LEVEL_LAYER0_SIZE_X + 2] = (layers[0].sizeX & 0x0000ff00) >>  8;
		header[LEVEL_LAYER0_SIZE_X + 1] = (layers[0].sizeX & 0x00ff0000) >> 16;
		header[LEVEL_LAYER0_SIZE_X    ] = (layers[0].sizeX & 0xff000000) >> 24;

		// Convert Level Size Layer 0 Y
		header[LEVEL_LAYER0_SIZE_Y + 3] = (layers[0].sizeY & 0x000000ff) >>  0;
		header[LEVEL_LAYER0_SIZE_Y + 2] = (layers[0].sizeY & 0x0000ff00) >>  8;
		header[LEVEL_LAYER0_SIZE_Y + 1] = (layers[0].sizeY & 0x00ff0000) >> 16;
		header[LEVEL_LAYER0_SIZE_Y    ] = (layers[0].sizeY & 0xff000000) >> 24;

		// Convert Level Size Layer 1 X
		header[LEVEL_LAYER1_SIZE_X + 3] = (layers[1].sizeX & 0x000000ff) >>  0;
		header[LEVEL_LAYER1_SIZE_X + 2] = (layers[1].sizeX & 0x0000ff00) >>  8;
		header[LEVEL_LAYER1_SIZE_X + 1] = (layers[1].sizeX & 0x00ff0000) >> 16;
		header[LEVEL_LAYER1_SIZE_X    ] = (layers[1].sizeX & 0xff000000) >> 24;

		// Convert Level Size Layer 1 Y
		header[LEVEL_LAYER1_SIZE_Y + 3] = (layers[1].sizeY & 0x000000ff) >>  0;
		header[LEVEL_LAYER1_SIZE_Y + 2] = (layers[1].sizeY & 0x0000ff00) >>  8;
		header[LEVEL_LAYER1_SIZE_Y + 1] = (layers[1].sizeY & 0x00ff0000) >> 16;
		header[LEVEL_LAYER1_SIZE_Y    ] = (layers[1].sizeY & 0xff000000) >> 24;

		// Convert Level Size Layer 2 X
		header[LEVEL_LAYER2_SIZE_X + 3] = (layers[2].sizeX & 0x000000ff) >>  0;
		header[LEVEL_LAYER2_SIZE_X + 2] = (layers[2].sizeX & 0x0000ff00) >>  8;
		header[LEVEL_LAYER2_SIZE_X + 1] = (layers[2].sizeX & 0x00ff0000) >> 16;
		header[LEVEL_LAYER2_SIZE_X    ] = (layers[2].sizeX & 0xff000000) >> 24;

		// Convert Level Size Layer 2 Y
		header[LEVEL_LAYER2_SIZE_Y + 3] = (layers[2].sizeY & 0x000000ff) >>  0;
		header[LEVEL_LAYER2_SIZE_Y + 2] = (layers[2].sizeY & 0x0000ff00) >>  8;
		header[LEVEL_LAYER2_SIZE_Y + 1] = (layers[2].sizeY & 0x00ff0000) >> 16;
		header[LEVEL_LAYER2_SIZE_Y    ] = (layers[2].sizeY & 0xff000000) >> 24;


		// Convert level title
		for(uint16_t i = 0; i < LEVEL_TITLE_STRING_LENGTH; ++i)
			if(i < title.length())
				header[LEVEL_TITLE_STRING_BEGIN + i] = title.at(i);
			else
				header[LEVEL_TITLE_STRING_BEGIN + i] = '\0';

		// Convert level tileset name
		for(uint16_t i = 0; i < LEVEL_TILESET_STRING_LENGTH; i++)
			if(i < tileset.length())
				header[LEVEL_TILESET_STRING_BEGIN + i] = tileset.at(i);
			else
				header[LEVEL_TILESET_STRING_BEGIN + i] = '\0';

		// Write file header
		file.write(header, LEVEL_HEADER_LENGTH);
		delete[] header;

		// Write tiles
		for(uint8_t i = 0; i < LAYERS; i++)
			for(uint16_t k = 0; k < layers[i].sizeX; k++)
				for(uint16_t l = 0; l < layers[i].sizeY; ++l)
					file.write((char*) &layers[i].tile[k][l], 1);

	}

	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		success = false;
	}

	if(file.is_open())
		file.close();

	if(success)
		std::cout << "Level saved." << std::endl;

	return success;
}

bool Level::GetMaskStateFromCoord(uint32_t x, uint32_t y)
{
	uint8_t tile = GetTile(LAYER_MAIN, x / TILE_SIZE_X, y / TILE_SIZE_Y);
	uint16_t tilePosX = x % TILE_SIZE_X;
	uint16_t tilePosY = y % TILE_SIZE_Y;

	return (mask[tile][(tilePosX + tilePosY * TILE_SIZE_Y) / 8] << (tilePosX + tilePosY * TILE_SIZE_Y) % 8 & 0x80);
}

void Level::SetMask(uint8_t** mask)
{
	this->mask = mask;
}

uint8_t** Level::GetMask()
{
	return mask;
}

void Level::DeleteMask()
{
	if(mask != NULL)
		for(uint16_t i = 0; i < 256; i++)
			delete[] mask[i];
		delete[] mask;

	mask = NULL;
}

void Level::CreateDefaultCollisionMask()
{
	mask = new uint8_t*[256];

	for(uint16_t i = 0; i < 256; i++)
		mask[i] = new uint8_t[(TILE_SIZE_X * TILE_SIZE_Y) / 8];

	for(uint16_t i = 0; i < (TILE_SIZE_X * TILE_SIZE_Y) / 8; i++)
		mask[0][i] = 0;

	for(uint16_t i = 1; i < 256; i++)
		for(uint16_t k = 0; k < (TILE_SIZE_X * TILE_SIZE_Y) / 8; k++)
			mask[i][k] = 255;
}

bool Level::LayerIsBorderless(uint8_t layer)
{
	return layers[layer].borderless;
}

void Level::LayerSetBorderless(uint8_t layer, bool borderless)
{
	layers[layer].borderless = borderless;
}

uint32_t Level::GetSizeX()
{
	return layers[LAYER_MAIN].sizeX;
}

uint32_t Level::GetSizeY()
{
	return layers[LAYER_MAIN].sizeY;
}