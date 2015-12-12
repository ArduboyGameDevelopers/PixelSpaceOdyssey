#ifndef LEVELCHARACTER_H
#define LEVELCHARACTER_H

#include <QtGlobal>
#include <QImage>

enum CharacterType {
    CharacterTypePlayer,
    CharacterTypeBear,
    CharacterTypeDog,
    CharacterTypeSpiderSmall,
    CharacterTypeSpiderLarge,
    CharacterTypeCount
};

enum CharacterDir { CharacterDirLeft = -1, CharacterDirRight = 1 };

class CharacterInfo
{
public:
    CharacterInfo() :
        _type(CharacterTypeCount),
        _x(0),
        _y(0),
        _direction(CharacterDirLeft)
    {
    }
    
    CharacterInfo(CharacterType type, int x, int y, CharacterDir direction) :
        _type(type),
        _x(x),
        _y(y),
        _direction(direction)
    {
    }
    
    CharacterInfo(const CharacterInfo &other) :
        _type(other.type()),
        _x(other.x()),
        _y(other.y()),
        _direction(other.direction())
    {
    }
    
public:
    static const QImage getImage(CharacterType type);
    static const QString getName(CharacterType type);
    static CharacterType typeFromName(const QString &name);

public:
    inline CharacterType type() const { return _type; }
    inline int x() const { return _x; }
    inline void setX(int x) { _x = x; }

    inline int y() const { return _y; }
    inline void setY(int y) { _y = y; }

    inline void setPos(int x, int y) { setX(x); setY(y); }

    inline CharacterDir direction() const { return _direction; }
    inline void setDirection(CharacterDir direction) { _direction = direction; }
    
    inline bool isValid() const { return _type != CharacterTypeCount; }
    
    const QImage image() const;
    const QString name() const;

private:
    CharacterType _type;
    int _x;
    int _y;
    CharacterDir _direction;
};

#endif // LEVELCHARACTER_H
