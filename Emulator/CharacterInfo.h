#ifndef LEVELCHARACTER_H
#define LEVELCHARACTER_H

#include <QtGlobal>
#include <QImage>

#include "character.h"

enum CharacterType {
    CharacterTypePlayer,
    CharacterTypeBear,
    CharacterTypeDog,
    CharacterTypeSpiderSmall,
    CharacterTypeSpiderLarge,
    CharacterTypeCount
};

enum CharacterInitialState {
    CharacterInitialStateUndefined,
    CharacterInitialStateStat,   // stay still until it see the player
    CharacterInitialStateSleep,  // sleep until player wake is up
    CharacterInitialStatePatrol, // patroling the area
};

enum CharacterPatrolBehavior {
    CharacterPatrolBehaviorUndefined,
    CharacterPatrolBehaviorNone,        // stay in the last position
    CharacterPatrolBehaviorForever,     // once started partoling - never stops
    CharacterPatrolBehaviorReturnToBase // returns to initial position after time out
};

const QString CharacterInitialStateGetName(CharacterInitialState behavior);
const QString CharacterPatrolBehaviorGetName(CharacterPatrolBehavior behavior);

CharacterInitialState CharacterInitialStateFromName(const QString &behavior);
CharacterPatrolBehavior CharacterPatrolBehaviorFromName(const QString &behavior);

class CharacterInfo
{
public:
    CharacterInfo() :
        _id(0),
        _type(CharacterTypeCount),
        _x(0),
        _y(0),
        _direction(DIR_LEFT),
        _initialState(CharacterInitialStateUndefined),
        _patrolBehavior(CharacterPatrolBehaviorUndefined)
    {
    }
    
    CharacterInfo(CharacterType type, int x, int y, Direction direction) :
        _id(++_nextId),
        _type(type),
        _x(x),
        _y(y),
        _direction(direction),
        _initialState(CharacterInitialStateUndefined),
        _patrolBehavior(CharacterPatrolBehaviorUndefined)
    {
    }
    
    CharacterInfo(const CharacterInfo &other) :
        _id(other.id()),
        _type(other.type()),
        _x(other.x()),
        _y(other.y()),
        _direction(other.direction()),
        _initialState(other._initialState),
        _patrolBehavior(other._patrolBehavior)
    {
    }
    
public:
    static const QImage getImage(CharacterType type, bool flipped = false);
    static QSize getSize(CharacterType type);
    static QSize getBounds(CharacterType type);
    
    static const QString getName(CharacterType type);
    static CharacterType typeFromName(const QString &name);

public:
    inline CharacterType type() const { return _type; }
    inline int x() const { return _x; }
    inline void setX(int x) { _x = x; }

    inline int y() const { return _y; }
    inline void setY(int y) { _y = y; }
    
    inline int width() const { return getSize(_type).width(); }
    inline int height() const { return getSize(_type).height(); }

    inline void setPos(int x, int y) { setX(x); setY(y); }

    inline Direction direction() const { return _direction; }
    inline void setDirection(Direction direction) { _direction = direction; }
    
    inline int id() const { return _id; }
    inline bool isValid() const { return _type != CharacterTypeCount; }
    
    inline CharacterInitialState initialState() const { return _initialState; }
    inline void setInitialBehavior(CharacterInitialState behavior) { _initialState = behavior; }
    
    inline CharacterPatrolBehavior patrolBehavior() const { return _patrolBehavior; }
    inline void setPatrolBehavior(CharacterPatrolBehavior behavior) { _patrolBehavior = behavior; }
    
    const QImage image() const;
    const QString name() const;

private:
    static int _nextId;
    int _id;
    CharacterType _type;
    int _x;
    int _y;
    Direction _direction;
    CharacterInitialState _initialState;
    CharacterPatrolBehavior _patrolBehavior;
};

#endif // LEVELCHARACTER_H
