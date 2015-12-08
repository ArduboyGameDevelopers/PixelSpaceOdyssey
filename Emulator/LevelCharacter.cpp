#include "LevelCharacter.h"

static const QString kCharacterImageLookup[] = {
    ":/characters/ch_player_8x8.png",
    ":/characters/ch_bear_32x24.png",
    ":/characters/ch_dog_16x10.png",
    ":/characters/ch_spider_16x8.png",
    ":/characters/ch_spider_24x16.png"
};

const QImage LevelCharacter::getImage(CharacterType type)
{
    const QString &name = kCharacterImageLookup[type];
    return QImage(name);
}

const QImage LevelCharacter::image() const
{
    return getImage(_type);
}