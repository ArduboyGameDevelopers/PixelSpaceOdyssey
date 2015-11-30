//
//  List.h
//  PixelSpaceOdyssey
//
//  Created by Alex Lementuev on 11/29/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef List_h
#define List_h

#include <stdio.h>
#include <assert.h>

template <class T>
class List
{
private:
    static const int kDefaultCapacity = 16;
    
private:
    T*  _data;
    int _size;
    int _capacity;
    
public:
    List(int capacity = kDefaultCapacity);
    ~List();
    
public:
    void add(const T& element);
    void remove(const T& element);
    int indexOf(const T& element) const;
    
    inline T get(int index) const
    {
        assert(index >= 0 && index < _size);
        return _data[index];
    }
    
public:
    inline int size() const { return _size; }
    
private:
    void expand(int capacity);
};

template <class T>
List<T>::List(int capacity) :
    _data(NULL),
    _size(0),
    _capacity(capacity)
{
    if (capacity > 0)
    {
        _data = (T*) malloc(capacity * sizeof(T));
    }
}

template <class T>
List<T>::~List()
{
    free(_data);
}

template <class T>
void List<T>::add(const T& element)
{
    if (_size == _capacity)
    {
        expand(_capacity > 0 ? 2 * _capacity : kDefaultCapacity);
    }
    _data[_size++] = element;
}

template <class T>
void List<T>::remove(const T& element)
{
    int index = indexOf(element);
    if (index != -1)
    {
        for (int i = index + 1; i < _size; ++i)
        {
            _data[i - 1] = _data[i];
        }
        --_size;
    }
}

template <class T>
int List<T>::indexOf(const T& element) const
{
    for (int i = 0; i < _size; ++i)
    {
        if (_data[i] == element)
        {
            return i;
        }
    }
    return -1;
}

template <class T>
void List<T>::expand(int capacity)
{
    assert(capacity > 0);
    T* data = (T*) malloc(capacity * sizeof(T));
    
    // we can't use memcpy because it makes some pointers invalid
    for (int i = 0; i < _size; ++i)
    {
        data[i] = _data[i];
    }
    
    free(_data);
    
    _data = data;
    _capacity = capacity;
}

#endif /* List_h */
