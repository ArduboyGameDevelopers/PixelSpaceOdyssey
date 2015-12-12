#include "CharacterInfo.h"

#include <QDebug>

typedef struct _CharacterTypeParams {
    QString name;
    QString filename;
    QSize size;
} CharacterTypeParams;

static const CharacterTypeParams lookup[] = {
    { "Player",       ":/characters_full/ch_player_8x8_full.png",     QSize(8, 8) },
    { "Bear",         ":/characters_full/ch_bear_32x24_full.png",     QSize(32, 24) },
    { "Dog",          ":/characters_full/ch_dog_16x10_full.png",      QSize(16, 10) },
    { "Spider Small", ":/characters_full/ch_spider_16x8_full.png",    QSize(16, 8) },
    { "Spider Large", ":/characters_full/ch_spider_24x16_full.png",   QSize(24, 16) },
};
static const int lookupCount = sizeof(lookup) / sizeof(CharacterTypeParams);

const QImage CharacterInfo::getImage(CharacterType type)
{
    Q_ASSERT(type >= 0 && type < lookupCount);
    return QImage(lookup[type].filename);
}

QSize CharacterInfo::getSize(CharacterType type)
{
    Q_ASSERT(type >= 0 && type < lookupCount);
    return lookup[type].size;
}

const QString CharacterInfo::getName(CharacterType type)
{
    Q_ASSERT(type >= 0 && type < lookupCount);
    return lookup[type].name;
}

CharacterType CharacterInfo::typeFromName(const QString &name)
{
    if (name == "Player") return CharacterTypePlayer;
    if (name == "Bear") return CharacterTypeBear;
    if (name == "Dog") return CharacterTypeDog;
    if (name == "Spider Small") return CharacterTypeSpiderSmall;
    if (name == "Spider Large") return CharacterTypeSpiderLarge;
    
    return CharacterTypeCount;
}

const QImage CharacterInfo::image() const
{
    return getImage(_type);
}

const QString CharacterInfo::name() const
{
    return getName(_type);
}
