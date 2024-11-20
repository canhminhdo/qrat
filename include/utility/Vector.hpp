//
// Created by CanhDo on 2024/11/08.
//

#ifndef QRAT_VECTOR_HPP
#define QRAT_VECTOR_HPP

#include <algorithm>
#include <iostream>

template<typename Object>
class Vector {
public:
    // prevent implicit type conversions, otherwise, e.g., Vector v = 10 is valid
    explicit Vector(int initSize = 0) : theSize{initSize}, theCapacity{initSize + SPARE_CAPACITY} {
        objects = new Object[theCapacity];
    }

    // copy constructor
    Vector(const Vector &rhs) : theSize{rhs.theSize}, theCapacity{rhs.theCapacity}, objects{nullptr} {
        objects = new Object[theCapacity];
        for (int i = 0; i < theSize; i++)
            objects[i] = rhs.objects[i];
    }

    // copy assignment
    Vector &operator=(const Vector &rhs) {
        Vector copy = rhs; // calling to the copy constructor
        std::swap(*this, copy);
        return *this;
    }

    // de-constructor
    ~Vector() {
        delete[] objects;
    }

    // move constructor
    Vector(Vector &&rhs) : theSize{rhs.theSize}, theCapacity{rhs.theCapacity}, objects{rhs.objects} {
        rhs.objects = nullptr;
        rhs.theSize = 0;
        rhs.theCapacity = 0;
    }

    // move assignment
    Vector &operator=(Vector &&rhs) {
        std::swap(theSize, rhs.theSize);
        std::swap(theCapacity, rhs.theCapacity);
        std::swap(objects, rhs.objects);
        return *this;
    }

    void resize(int newSize) {
        if (newSize > theCapacity)
            reserve(newSize * 2);
        theSize = newSize;
    }

    void reserve(int newCapacity) {
        if (newCapacity < theSize)
            return;
        Object *newArray = new Object[newCapacity];
        for (int i = 0; i < theSize; i++)
            newArray[i] = std::move(objects[i]);
        theCapacity = newCapacity;
        std::swap(objects, newArray);
        delete[] newArray;
    }

    // return by reference
    Object &operator[](int index) {
        return objects[index];
    }

    // return by const reference
    const Object &operator[](int index) const {
        return objects[index];
    }

    bool empty() const {
        return size() == 0;
    }

    int size() const {
        return theSize;
    }

    int capacity() const {
        return theCapacity;
    }

    // for lvalue object
    void push_back(const Object &x) {
        if (theSize == theCapacity)
            reserve(2 * theCapacity + 1);
        objects[theSize++] = x;
    }

    // for rvalue object
    void push_back(Object &&x) {
        if (theSize == theCapacity)
            reserve(2 * theCapacity + 1);
        objects[theSize++] = std::move(x);
        // still need to use std::move(x) to make it an rvalue since x is actually a lvalue with a rvalue reference type.
    }

    void pop_back() {
        --theSize;
    }

    const Object &back() const {
        return objects[theSize - 1];
    }

    typedef Object *iterator;
    typedef const Object *const_iterator;

    iterator begin() {
        return &objects[0];
    }

    const_iterator begin() const {
        return &objects[0];
    }

    iterator end() {
        return &objects[size()];
    }

    const_iterator end() const {
        return &objects[size()];
    }

    void dump() const {
        for (int i = 0; i < theSize; i++)
            std::cout << objects[i] << std::endl;
    }

    static const int SPARE_CAPACITY = 16;

private:
    int theSize;
    int theCapacity;
    Object *objects;
};


#endif//QRAT_VECTOR_HPP
