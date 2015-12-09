#include "LevelCharacter.h"

static const QString kCharacterImageLookup[] = {
    ":/characters/ch_player_8x8.png",
    ":/characters/ch_bear_32x24.png",
    ":/characters/ch_dog_16x10.png",
    ":/characters/ch_spider_16x8.png",
    ":/characters/ch_spider_24x16.png"
};

static const QString kCharacterNameLookup[] = {
    "Player",
    "Bear",
    "Dog",
    "Spider Small",
    "Spider Large"
};

const QImage LevelCharacter::getImage(CharacterType type)
{
    const QString &name = kCharacterImageLookup[type];
    return QImage(name);
}

const QString LevelCharacter::getName(CharacterType type)
{
    return kCharacterNameLookup[type];
}

CharacterType LevelCharacter::typeFromName(const QString &name)
{
    if (name == "Player") return CharacterTypePlayer;
    if (name == "Bear") return CharacterTypeBear;
    if (name == "Dog") return CharacterTypeDog;
    if (name == "Spider Small") return CharacterTypeSpiderSmall;
    if (name == "Spider Large") return CharacterTypeSpiderLarge;
    
    return CharacterTypeCount;
}

const QImage LevelCharacter::image() const
{
    return getImage(_type);
}

const QString LevelCharacter::name() const
{
    return getName(_type);
}