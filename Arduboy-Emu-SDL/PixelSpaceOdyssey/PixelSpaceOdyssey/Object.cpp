//
//  Object.c
//  PixelSpaceOdyssey
//
//  Created by Alex Lementuev on 12/1/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#include <assert.h>

#include "Object.h"

Object::Object() :
    _referenceCount(1)
{
}

Object::~Object()
{
    assert(_referenceCount == 0);
}

void Object::incReferenceCount()
{
    ++_referenceCount;
}

void Object::decReferenceCount()
{
    assert(_referenceCount > 0);
    
    --_referenceCount;
    if (_referenceCount == 0)
    {
        delete this;
    }
}