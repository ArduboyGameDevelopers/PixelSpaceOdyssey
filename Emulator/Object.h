#ifndef OBJECT_H
#define OBJECT_H

#define RETAIN(REF) ((REF) ? (REF)->retain() : NULL)
#define RELEASE(REF) if (REF) { (REF)->release(); }

#define OBJECT_DEF(Type) \
public: \
    virtual Type* retain() { incReferenceCount(); return this; }

class Object
{
public:
    Object();
    virtual ~Object();

public:
    inline void release() { decReferenceCount(); }

protected:
    void incReferenceCount();
    void decReferenceCount();

private:
    int _referenceCount;
};

#endif // OBJECT_H
