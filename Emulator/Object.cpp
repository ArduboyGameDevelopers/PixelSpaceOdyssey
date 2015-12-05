#include "Object.h"

#include <QtGlobal>

Object::Object() :
    _referenceCount(1)
{
}

Object::~Object()
{
    Q_ASSERT(_referenceCount == 0);
}

void Object::incReferenceCount()
{
    ++_referenceCount;
}

void Object::decReferenceCount()
{
    Q_ASSERT(_referenceCount > 0);

    --_referenceCount;
    if (_referenceCount == 0)
    {
        delete this;
    }
}
