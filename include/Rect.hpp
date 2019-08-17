#pragma once

#include "dxdemo.hpp"

namespace dxdemo
{

template<typename T>
class RectT
{
public:

    T x {0};
    T y {0};
    T width {0};
    T height {0};

    RectT() = default;
    RectT(const RectT& rect) = default;
    RectT(RectT&& rect) = default;
    RectT(T _x, T _y, T _width, T _height) : x(_x), y(_y), width(_width), height(_height)
    {}

};//class RectT

using RectI = RectT<int>;
using RectF = RectT<float>;
using RectD = RectT<double>;
using Rect = RectF;

}//namespace vgui
