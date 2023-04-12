#include "engine.h"

Engine::Engine(SDL_Window* window, uint32_t flags)
{
    if(flags & FLAG_EDITOR)
        editor = true;
    

    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);

    SDL_GetWindowDisplayMode(window, &mode);
    SDL_GetWindowSize(window, &screenSizeX, &screenSizeY);
    screenRefresh = mode.refresh_rate;

    std::cout << "Screen size: " << screenSizeX << "x" << screenSizeY << std::endl
        << "Refresh rate: " << screenRefresh << std::endl;

    LoadTextures();
        

    for(int i = 0; i < 256; i++)
    {
        tile[i].x = TILE_SIZE_X * (i % (TILESET_WIDTH));
        tile[i].y = TILE_SIZE_Y * (i / (TILESET_WIDTH));

        tile[i].w = TILE_SIZE_X;
        tile[i].h = TILE_SIZE_Y;
    }

    

    level = new Level();
    level->Load("./level/test.bin");
    level->LayerSetBorderless(2, true);

    SDL_Surface* surfaceMask = IMG_Load("./tileset/wall_mask.png");
    level->SetMask(CreateCollisionMaskFromSurface(surfaceMask));
    SDL_FreeSurface(surfaceMask);
    
    player = new Player(level, &ticksLastFrame, &ticksCurrentFrame);
    player->SetPos((SDL_Point) { 128, 192 } );
    entities.push_back(player);

    for(int i = 0; i < 256; i++)
        spritePlayer[i]  = { (i % 4) * 128, (i / 4) * 128, 128, 128 };

    return;
}

Engine::~Engine()
{
    SDL_DestroyRenderer(render);

    SDL_DestroyTexture(textureTile);
    SDL_DestroyTexture(texturePlayer);

    delete level;

    return;
}

// main loop
int Engine::Run()
{
    // Check if render is loaded
    if (!render)
    {
        std::cerr << SDL_GetError() << std::endl;
        return -1;
    }

    ticksCurrentFrame = SDL_GetTicks64();
    while(!quit)
    {
        SDL_RenderClear(render);    // Clear Screen
        if(editor)
            ControlsEditor();                 // get events and user input
        else
        {
            ControlsGame();
            RunEntities();
            MoveCamera();
        }
        
        DrawLevel();
        SDL_RenderPresent(render);  // Confirm drawing on screen

        ticksLastFrame = ticksCurrentFrame;
        ticksCurrentFrame = SDL_GetTicks64();
        ticksSinceLastFrame = ticksCurrentFrame - ticksLastFrame;
        //std::cout << ticksSinceLastFrame << ", FPS:" << 1000. / ticksSinceLastFrame << ", " << ticksLastFrame << ", " << ticksCurrentFrame << std::endl;
    }

    return 0;
}

void Engine::RunEntities()
{
    for(Entity* entity : entities)
        entity->Do();

    return;
}

bool Engine::DrawLevel()
{
    int16_t x;
    int16_t y;

    imagePosition.w = tilesizeX;
    imagePosition.h = tilesizeY;

    // draw sky
    for(int16_t i = 0; i < VIEWSIZE_Y + 2; ++i)
        for(int16_t k = 0; k < VIEWSIZE_X + 2; ++k)
        {
            x = k + ((camera.x * .5) / TILE_SIZE_X);
            y = i + ((camera.y * .5) / TILE_SIZE_Y);

            imagePosition.x = k * TILE_SIZE_X - (int) (camera.x * .5) % TILE_SIZE_X;
            imagePosition.y = i * TILE_SIZE_Y - (int) (camera.y * .5) % TILE_SIZE_Y;
            SDL_RenderCopy(render, textureTile, &tile[level->GetTile(2, x, y)], &imagePosition);
    }

    //draw main layer
    for(int16_t i = -2; i < VIEWSIZE_Y + 2; ++i)
        for(int16_t k = -2; k < VIEWSIZE_X + 2; ++k)
        {
            x = k + camera.x / TILE_SIZE_X;
            y = i + camera.y / TILE_SIZE_Y;

            imagePosition.x = k * TILE_SIZE_X - camera.x % TILE_SIZE_X;
            imagePosition.y = i * TILE_SIZE_Y - camera.y % TILE_SIZE_X;
            SDL_RenderCopy(render, textureTile, &tile[level->GetTile(1, x, y)], &imagePosition);
        }

    // Draw player
    imagePosition.x = player->GetPosX() - camera.x;
    imagePosition.y = player->GetPosY() - camera.y;
    imagePosition.w = 128;
    imagePosition.h = 128;
    SDL_RenderCopy(render, texturePlayer, &spritePlayer[player->GetSprite()], &imagePosition);

    imagePosition.w = tilesizeX;
    imagePosition.h = tilesizeY;

    // draw foreground
    for(int16_t i = -2; i < VIEWSIZE_Y + 2; ++i)
        for(int16_t k = -2; k < VIEWSIZE_X + 2; ++k)
        {
            x = k + camera.x / TILE_SIZE_X;
            y = i + camera.y / TILE_SIZE_Y;

            imagePosition.x = k * TILE_SIZE_X - camera.x % TILE_SIZE_X;
            imagePosition.y = i * TILE_SIZE_Y - camera.y % TILE_SIZE_X;
            SDL_RenderCopy(render, textureTile, &tile[level->GetTile(0, x, y)], &imagePosition);
        }

    return true;
}

bool Engine::LoadTextures()
{
    SDL_Surface* surfaceTile = IMG_Load("./tileset/wall.png");
    SDL_Surface* surfacePlayer = IMG_Load("./tileset/character.png");
    
    textureTile = SDL_CreateTextureFromSurface(render, surfaceTile);
    texturePlayer = SDL_CreateTextureFromSurface(render, surfacePlayer);
    SDL_FreeSurface(surfaceTile);
    SDL_FreeSurface(surfacePlayer);

    return true;
}

// Create a collision mask from a surface
uint8_t** Engine::CreateCollisionMaskFromSurface(SDL_Surface* surface)
{
    Uint32* pixels = (Uint32 *) surface->pixels;
	uint8_t** mask = new uint8_t*[256];

    // clear mask
	for(uint16_t i = 0; i < 256; ++i)
    {
		mask[i] = new uint8_t[(TILE_SIZE_X * TILE_SIZE_Y) / 8];
        for(uint16_t k = 0; k < (TILE_SIZE_X * TILE_SIZE_Y) / 8; ++k)
            mask[i][k] = 0x00;
    }
    
    // set mask values depending on pixel colour (obstacles are black)
    // I have no idea how this code works. I'm just a comment
    for (int i = 0; i < ((surface->w / TILE_SIZE_X) * (surface->h / TILE_SIZE_Y)); ++i)
        for (int k = 0; k < TILE_SIZE_X * TILE_SIZE_Y; ++k)
            mask[i][k / 8] |= pixels[(k % TILE_SIZE_X + (k / TILE_SIZE_X) * TILE_SIZE_X * 4) + TILE_SIZE_X * (i % 4) + (TILE_SIZE_X * TILE_SIZE_Y) * (i / 4) * 4] & (RMASK | GMASK | BMASK) ? 0x00 : 0x80 >> (k % 8);

    return mask;
}

void Engine::Engine::ControlsGame()
{
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT: // event window quit
            quit = true;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    quit = true;
                    break;

                case SDLK_UP:
                    --camera.y;
                    break;

                case SDLK_DOWN:
                    ++camera.y;
                    break;

                case SDLK_LEFT:
                    --camera.x;
                    break;
                    
                case SDLK_RIGHT:
                    ++camera.x;
                    break;

                case SDLK_w:
                    player->Jump();
                    break;

                case SDLK_a:
                    player->SetMovementLeft();
                    break;
                    
                case SDLK_d:
                    player->SetMovementRight();
                    break;
            }
            break;

        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
                case SDLK_a:
                    player->StopMovement();
                    break;
                    
                case SDLK_d:
                    player->StopMovement();
                    break;
            }
            break;
        }
    }
}

void Engine::ControlsEditor()
{
    SDL_GetMouseState(&mousePos.x, &mousePos.y);
    mouseTileX = (camera.x + mousePos.x) / TILE_SIZE_X;
    mouseTileY = (camera.y + mousePos.y) / TILE_SIZE_Y;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT: // event window quit
            quit = true;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    quit = true;
                    break;

                case SDLK_UP:
                    camera.y -= TILE_SIZE_Y;
                    break;

                case SDLK_DOWN:
                    camera.y += TILE_SIZE_Y;
                    break;

                case SDLK_LEFT:
                    camera.x -= TILE_SIZE_X;
                    break;
                    
                case SDLK_RIGHT:
                    camera.x += TILE_SIZE_X;
                    break;

                case SDLK_s:
                    level->Save("./level/test.bin");
                    break;

                case SDLK_l:
                    level->Load("./level/test.bin");
                    break;

                case SDLK_1:
                    editor_layer = 0;
                    printf("Layer 0\n");
                    break;

                case SDLK_2:
                    editor_layer = 1;
                    printf("Layer 1\n");
                    break;

                case SDLK_3:
                    editor_layer = 2;
                    printf("Layer 2\n");
                    break;

                case SDLK_r:
                    int x;
                    int y;
                    std::cout << "New X size: ";
                    std::cin >> x;
                    std::cout << "New Y size: ";
                    std::cin >> y;
                    level->Resize(editor_layer, x, y);
            }
            break;

        case SDL_MOUSEWHEEL:
            if (event.wheel.y > 0)
                editor_tile++;
            else if (event.wheel.y < 0)
                editor_tile--;
            break;
            

        case SDL_MOUSEBUTTONDOWN:
            break;
        }
    }

    if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
        level->SetTile(editor_layer, mouseTileX, mouseTileY, editor_tile);
    }
    if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
    {
        level->SetTile(editor_layer, mouseTileX, mouseTileY, 0);
    }
}

void Engine::MoveCamera()
{
    camera.x = player->GetPosX() - screenSizeX / 2;
    camera.y = player->GetPosY() - screenSizeY / 2;
    if(camera.x < 0)
        camera.x = 0;
    if(camera.y < 0)
        camera.y = 0;
    if(camera.x > (level->GetSizeX() * TILE_SIZE_X + screenSizeX))
        camera.x = (level->GetSizeX() * TILE_SIZE_X + screenSizeX);
    if(camera.y > (level->GetSizeY() * TILE_SIZE_Y + screenSizeY))
        camera.y = (level->GetSizeY() * TILE_SIZE_Y + screenSizeY);
        
}