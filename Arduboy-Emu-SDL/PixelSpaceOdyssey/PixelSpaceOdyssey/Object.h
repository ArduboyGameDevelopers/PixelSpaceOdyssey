//
//  Object.h
//  PixelSpaceOdyssey
//
//  Created by Alex Lementuev on 12/1/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef Object_h
#define Object_h

#define OBJECT_DEF(Type) \
public: \
    virtual Type* retain() { incReferenceCount(); return this; }

class Object
{
private:
    int _referenceCount;
    
public:
    Object();
    virtual ~Object();
    
public:
    virtual void release() { decReferenceCount(); }
    
protected:
    void incReferenceCount();
    void decReferenceCount();
};

#endif /* Object_h */
