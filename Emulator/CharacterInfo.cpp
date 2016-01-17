#include "CharacterInfo.h"

#include <QDebug>

#include "ch_hero_animations.h"
#include "ch_dog_animations.h"
#include "ch_bear_animations.h"
#include "ch_spider_large_animations.h"
#include "ch_spider_small_animations.h"

typedef struct _CharacterTypeParams {
    QString name;
    QString filename;
    QSize size;
    QPoint center;
    QRect bounds;
    QImage image;
    QImage imageFlipped;
    
} CharacterTypeParams;

static CharacterTypeParams lookup[] = {
    {
        "Player",
        ":/characters_full/ch_player_8x8_full.png",
        QSize(8, 8),
        QPoint(CH_HERO_ANIMATION_CENTER_X, CH_HERO_ANIMATION_CENTER_Y),
        QRect(CH_HERO_ANIMATION_BOUNDS_X, CH_HERO_ANIMATION_BOUNDS_Y, CH_HERO_ANIMATION_BOUNDS_WIDTH, CH_HERO_ANIMATION_BOUNDS_HEIGHT),
        QImage(),
        QImage()
    },
    {
        "Bear",
        ":/characters_full/ch_bear_32x24_full.png",
        QSize(32, 24),
        QPoint(CH_BEAR_ANIMATION_CENTER_X, CH_BEAR_ANIMATION_CENTER_Y),
        QRect(CH_BEAR_ANIMATION_BOUNDS_X, CH_BEAR_ANIMATION_BOUNDS_Y, CH_BEAR_ANIMATION_BOUNDS_WIDTH, CH_BEAR_ANIMATION_BOUNDS_HEIGHT),
        QImage(),
        QImage()
    },
    {
        "Dog",
        ":/characters_full/ch_dog_16x10_full.png",
        QSize(16, 10),
        QPoint(CH_DOG_ANIMATION_CENTER_X, CH_DOG_ANIMATION_CENTER_Y),
        QRect(CH_DOG_ANIMATION_BOUNDS_X, CH_DOG_ANIMATION_BOUNDS_Y, CH_DOG_ANIMATION_BOUNDS_WIDTH, CH_DOG_ANIMATION_BOUNDS_HEIGHT),
        QImage(),
        QImage()
    },
    {
        "Spider Small",
        ":/characters_full/ch_spider_16x8_full.png",
        QSize(16, 8),
        QPoint(CH_SPIDER_SMALL_ANIMATION_CENTER_X, CH_SPIDER_SMALL_ANIMATION_CENTER_Y),
        QRect(CH_SPIDER_SMALL_ANIMATION_BOUNDS_X, CH_SPIDER_SMALL_ANIMATION_BOUNDS_Y, CH_SPIDER_SMALL_ANIMATION_BOUNDS_WIDTH, CH_SPIDER_SMALL_ANIMATION_BOUNDS_HEIGHT),
        QImage(),
        QImage()
    },
    {
        "Spider Large",
        ":/characters_full/ch_spider_24x16_full.png",
        QSize(24, 16),
        QPoint(CH_SPIDER_LARGE_ANIMATION_CENTER_X, CH_SPIDER_LARGE_ANIMATION_CENTER_Y),
        QRect(CH_SPIDER_LARGE_ANIMATION_BOUNDS_X, CH_SPIDER_LARGE_ANIMATION_BOUNDS_Y, CH_SPIDER_LARGE_ANIMATION_BOUNDS_WIDTH, CH_SPIDER_LARGE_ANIMATION_BOUNDS_HEIGHT),
        QImage(),
        QImage()
    },
};
static const int lookupCount = sizeof(lookup) / sizeof(CharacterTypeParams);

int CharacterInfo::_nextId = 0;

const QImage CharacterInfo::getImage(CharacterType type, bool flipped)
{
    Q_ASSERT(type >= 0 && type < lookupCount);
    
    CharacterTypeParams &params = lookup[type];
    if (params.image.isNull())
    {
        params.image = QImage(params.filename);
        params.imageFlipped = params.image.mirrored(true, false);
    }
    
    return flipped? params.imageFlipped : params.image;
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
    return getImage(_type, _direction == DIR_RIGHT);
}

const QString CharacterInfo::name() const
{
    return getName(_type);
}
