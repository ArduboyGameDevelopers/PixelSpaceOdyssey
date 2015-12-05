//
//  Level.c
//  PixelSpaceOdyssey
//
//  Created by Alex Lementuev on 12/2/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#include <SDL2/SDL.h>
#include <string.h>

#include "Level.h"

#define READ_UINT16 SDL_ReadBE16
#define WRITE_UINT16 SDL_WriteBE16

#define READ_UINT8 SDL_ReadU8
#define WRITE_UINT8 SDL_WriteU8

static const uint8_t kVersion = 1;

Level::Level() :
    _tileSetId(0),
    _indices(NULL),
    _rows(0),
    _cols(0),
    _playerX(0),
    _playerY(0)
{
}

Level::Level(uint8_t* indices, uint8_t rows, uint8_t cols) :
    _tileSetId(0),
    _rows(rows),
    _cols(cols),
    _playerX(0),
    _playerY(0)
{
    size_t size = rows * cols * sizeof(uint8_t);
    _indices = (uint8_t *) malloc(size);
    memcpy(_indices, indices, size);
}

Level::~Level()
{
    if (_indices)
    {
        free(_indices);
    }
}

Level* Level::readFromFile(const char* filename)
{
    SDL_RWops* file = SDL_RWFromFile(filename, "r");
    if (!file)
    {
        return NULL;
    }
    
    // version
    uint8_t version = READ_UINT8(file);
    
    // read tileset
    int tileSetId = READ_UINT8(file);
    
    // player pos
    uint16_t playerX = READ_UINT16(file);
    uint16_t playerY = READ_UINT16(file);
    
    // read indices
    int rows = READ_UINT16(file);
    int cols = READ_UINT16(file);
    int indexCount = rows * cols;
    uint8_t* indices = (uint8_t *) malloc(indexCount);
    for (int i = 0; i < indexCount; ++i)
    {
        indices[i] = READ_UINT8(file);
    }
    
    // read enemies
    uint8_t enemyCount = READ_UINT8(file);
    
    SDL_RWclose(file);
    
    Level* level = new Level();
    level->setTileSetId(tileSetId);
    level->setIndices(indices);
    level->setRows(rows);
    level->setCols(cols);
    level->setPlayerX(playerX);
    level->setPlayerY(playerY);
    return level;
}

void Level::writeToFile(const char* filename)
{
    SDL_RWops* file = SDL_RWFromFile(filename, "w");
    
    // version
    WRITE_UINT8(file, kVersion);
    
    // tileset
    WRITE_UINT8(file, 0);
    
    // player pos
    WRITE_UINT16(file, _playerX);
    WRITE_UINT16(file, _playerY);
    
    // indices
    WRITE_UINT16(file, _rows);
    WRITE_UINT16(file, _cols);
    int indexCount = _rows * _cols;
    for (int i = 0; i < indexCount; ++i)
    {
        WRITE_UINT8(file, _indices[i]);
    }
    
    // enemies
    WRITE_UINT8(file, 0);
    
    SDL_RWclose(file);
}