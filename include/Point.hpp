#pragma once

#include "dxdemo.hpp"

namespace dxdemo
{

template<typename T>
struct PointT
{
    T x {0};
    T y {0};

    PointT() = default;
    PointT(const PointT& point) = default;
    PointT(PointT&& point) = default;
    PointT(T _x, T _y) : x(_x), y(_y) {}

    operator bool() const { return x && y; }

//    T X() const { return x; }
//    T Y() const { return y; }
//    PointT&  X(T _x) { x = _x; return *this; }
//    PointT&  Y(T _y) { y = _y; return *this; }

    PointT&  Set(T _x, T _y) { x = _x; y = _y; return *this; }

    PointT& operator = (const PointT& point) = default;
    PointT& operator = (PointT&& point) = default;

    PointT& operator += (const PointT& point) { x += point.x; y += point.y; return *this; }
    PointT& operator -= (const PointT& point) { x -= point.x; y -= point.y; return *this; }

    template<typename U>
    PointT& operator *= (U factor)
    {
        x = RoundMult<T, U>(x, factor);
        y = RoundMult<T, U>(y, factor);
        return *this;
    }

    template<typename U>
    PointT& operator /= (U divisor)
    {
        x = RoundDiv<T, U>(x, divisor);
        y = RoundDiv<T, U>(y, divisor);
        return *this;
    }

    template<typename U>
    friend std::ostream& operator << (std::ostream& os, const PointT<U>& p);

    template<typename U>
    friend bool operator == (const PointT<U>& p1, const PointT<U>& p2);

    template<typename U>
    friend bool operator != (const PointT<U>& p1, const PointT<U>& p2);

    template<typename U>
    friend PointT<U> operator + (const PointT<U>& p1, const PointT<U>& p2);

    template<typename U>
    friend PointT<U> operator - (const PointT<U>& p1, const PointT<U>& p2);

    template<typename U, typename F>
    friend PointT<U> operator * (const PointT<U>& p, F factor);

    template<typename U, typename F>
    friend PointT<U> operator * (F factor, const PointT<U>& p);

    template<typename U, typename D>
    friend PointT<U> operator / (const PointT<U>& p, D divisor);

};//class PointT

template<typename T>
inline std::ostream& operator << (std::ostream& os, const PointT<T>& p)
{
    return os << "{x = " << p.x << ", y = " << p.y << "}";
}

template<typename T>
inline bool operator == (const PointT<T>& p1, const PointT<T>& p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}

template<typename T>
inline bool operator != (const PointT<T>& p1, const PointT<T>& p2)
{
    return p1.x != p2.x || p1.y != p2.y;
}

template<typename T>
inline PointT<T> operator + (const PointT<T>& p1, const PointT<T>& p2)
{
    return { p1.x + p2.x , p1.y + p2.y };
}

template<typename T>
inline PointT<T> operator - (const PointT<T>& p1, const PointT<T>& p2)
{
    return { p1.x - p2.x , p1.y - p2.y };
}

template<typename T, typename F>
inline PointT<T> operator * (const PointT<T>& p, F factor)
{
    return { RoundMult<T, F>(p.x, factor), RoundMult<T, F>(p.y, factor) };
}

template<typename T, typename F>
inline PointT<T> operator * (F factor, const PointT<T>& p)
{
    return { RoundMult<T, F>(p.x, factor), RoundMult<T, F>(p.y, factor) };
}

template<typename T, typename D>
inline PointT<T> operator / (const PointT<T>& p, D divisor)
{
    return { RoundDiv<T, D>(p.x, divisor), RoundDiv<T, D>(p.y, divisor) };
}

using PointI = PointT<int>;
using PointF = PointT<float>;
using PointD = PointT<double>;
using Point = PointI;
}//namespace vgui
