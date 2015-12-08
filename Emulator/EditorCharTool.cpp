#include "EditorCharTool.h"
#include "CharacterButton.h"

#include <DisplayWidget.h>

EditorCharTool::EditorCharTool(DisplayWidget *displayWidget, CharacterType characterType) :
    EditorTool(displayWidget),
    _characterType(characterType),
    _characterImage(LevelCharacter::getImage(characterType))
{
}

void EditorCharTool::onPaint(QPainter *painter)
{
    QRect rect(tileX(), tileY(), GRID_CELL_WIDTH_PX, GRID_CELL_HEIGHT_PX);
    painter->drawRect(rect);
    
    int drawX = tileX() + (GRID_CELL_WIDTH_PX - _characterImage.width()) / 2 ;
    int drawY = tileY() + GRID_CELL_HEIGHT_PX - _characterImage.height();
    
    painter->setOpacity(0.5);
    painter->drawImage(drawX, drawY, _characterImage);
    painter->setOpacity(1.0);
}

void EditorCharTool::onMousePressed(int, int)
{
    CharacterButton::deselectAll();
    
    Level *level = Level::current();
    
    int cx = (tileX() + (GRID_CELL_WIDTH_PX - _characterImage.width()) / 2) / PIXEL_WIDTH ;
    int cy = (tileY() + GRID_CELL_HEIGHT_PX - _characterImage.height()) / PIXEL_HEIGHT;
    if (_characterType == CharacterTypePlayer)
    {
        level->setPlayerPos(cx, cy);
        level->setPlayerDir(CharacterDirRight);
    }
    else
    {
        level->addCharacter(_characterType, cx, cy);
    }
}

void EditorCharTool::onMouseMoved(int x, int y)
{
    int oldTileX = tileX();
    int oldTileY = tileY();

    EditorTool::onMouseMoved(x, y);
    
    if (oldTileX != tileX() || oldTileY != tileY())
    {
        displayWidget()->repaint();
    }
}
