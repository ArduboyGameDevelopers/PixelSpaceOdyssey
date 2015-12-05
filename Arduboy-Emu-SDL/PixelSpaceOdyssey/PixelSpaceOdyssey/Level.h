//
//  Level.h
//  PixelSpaceOdyssey
//
//  Created by Alex Lementuev on 12/2/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef Level_h
#define Level_h

#include "Object.h"

#include "tilemap.h"

class Level : public Object
{
OBJECT_DEF(Level)
    
private:
    uint8_t  _tileSetId;
    uint8_t* _indices;
    uint8_t  _rows;
    uint8_t  _cols;
    uint16_t _playerX;
    uint16_t _playerY;

protected:
    Level();
    
public:
    Level(uint8_t* indices, uint8_t rows, uint8_t cols);
    virtual ~Level();
    
public:
    static Level* readFromFile(const char* filename);
    void writeToFile(const char* filename);
    
public:
    inline uint8_t* indices() const { return _indices; }
    
    inline uint8_t tileSetId() const { return _tileSetId; }
    inline void setTileSetId(uint8_t tileSetId) { _tileSetId = tileSetId; }
    
    inline uint8_t rows() const { return _rows; }
    inline void setRows(uint8_t rows) { _rows = rows; }
    
    inline uint8_t cols() const { return _cols; }
    inline void setCols(uint8_t cols) { _cols = cols; }
    
    inline uint16_t playerX() const { return _playerX; }
    inline void setPlayerX(uint16_t playerX) { _playerX = playerX; }
    inline uint16_t playerY() const { return _playerY; }
    inline void setPlayerY(uint16_t playerY) { _playerY = playerY; }
    
protected:
    inline void setIndices(uint8_t* indices) { _indices = indices; }
};

#endif /* Level_h */
