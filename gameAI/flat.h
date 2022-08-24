#pragma once
#include "common.h"

const int FLAT_POINT = 3;

//µã½á¹¹
template <class T>
struct Point
{
    T _x;
    T _y;
    T _z;
    Point() : _x(0), _y(0), _z(0) {}
    Point(T x, T y, T z) :_x(x), _y(y), _z(z) {}
    Point(const Point<T>& point)
    {
        _x = point._x;
        _y = point._y;
        _z = point._z;
    }
    Point& operator+=(const T& n)
    {
        _x += n;
        _y += n;
        _z += n;
        return *this;
    }
};

template <class T>
struct Flat
{
    Point<T> point[FLAT_POINT];
    void operator+=(const T& offset)
    {
        for (int i = 0; i < FLAT_POINT; i++)
        {
            point[i] += offset;
        }
    }
};