#pragma once

#include "dxdemo.hpp"

namespace dxdemo
{

template<typename T>
struct SizeT
{
    T width {0};
    T height {0};

    SizeT() = default;
    SizeT(const SizeT& point) = default;
    SizeT(SizeT&& point) = default;
    SizeT(T _x, T _y) : width(_x), height(_y) {}

    operator bool() const { return width && height; }

//    T X() const { return x; }
//    T Y() const { return y; }
//    SizeT&  X(T _x) { x = _x; return *this; }
//    SizeT&  Y(T _y) { y = _y; return *this; }

    SizeT&  Set(T _x, T _y) { width = _x; height = _y; return *this; }

    SizeT& operator = (const SizeT& point) = default;
    SizeT& operator = (SizeT&& point) = default;

    SizeT& operator += (const SizeT& point) { width += point.width; height += point.height; return *this; }
    SizeT& operator -= (const SizeT& point) { width -= point.width; height -= point.height; return *this; }

    template<typename U>
    SizeT& operator *= (U factor)
    {
        width = RoundMult<T, U>(width, factor);
        height = RoundMult<T, U>(height, factor);
        return *this;
    }

    template<typename U>
    SizeT& operator /= (U divisor)
    {
        width = RoundDiv<T, U>(width, divisor);
        height = RoundDiv<T, U>(height, divisor);
        return *this;
    }

    template<typename U>
    friend std::ostream& operator << (std::ostream& os, const SizeT<U>& p);

    template<typename U>
    friend bool operator == (const SizeT<U>& p1, const SizeT<U>& p2);

    template<typename U>
    friend bool operator != (const SizeT<U>& p1, const SizeT<U>& p2);

    template<typename U>
    friend SizeT<U> operator + (const SizeT<U>& p1, const SizeT<U>& p2);

    template<typename U>
    friend SizeT<U> operator - (const SizeT<U>& p1, const SizeT<U>& p2);

    template<typename U, typename F>
    friend SizeT<U> operator * (const SizeT<U>& p, F factor);

    template<typename U, typename F>
    friend SizeT<U> operator * (F factor, const SizeT<U>& p);

    template<typename U, typename D>
    friend SizeT<U> operator / (const SizeT<U>& p, D divisor);

};//class SizeT

template<typename T>
inline std::ostream& operator << (std::ostream& os, const SizeT<T>& p)
{
    return os << "{x = " << p.width << ", y = " << p.height << "}";
}

template<typename T>
inline bool operator == (const SizeT<T>& p1, const SizeT<T>& p2)
{
    return p1.width == p2.width && p1.height == p2.height;
}

template<typename T>
inline bool operator != (const SizeT<T>& p1, const SizeT<T>& p2)
{
    return p1.width != p2.width || p1.height != p2.height;
}

template<typename T>
inline SizeT<T> operator + (const SizeT<T>& p1, const SizeT<T>& p2)
{
    return { p1.width + p2.width , p1.height + p2.height };
}

template<typename T>
inline SizeT<T> operator - (const SizeT<T>& p1, const SizeT<T>& p2)
{
    return { p1.width - p2.width , p1.height - p2.height };
}

template<typename T, typename F>
inline SizeT<T> operator * (const SizeT<T>& p, F factor)
{
    return { RoundMult<T, F>(p.width, factor), RoundMult<T, F>(p.height, factor) };
}

template<typename T, typename F>
inline SizeT<T> operator * (F factor, const SizeT<T>& p)
{
    return { RoundMult<T, F>(p.width, factor), RoundMult<T, F>(p.height, factor) };
}

template<typename T, typename D>
inline SizeT<T> operator / (const SizeT<T>& p, D divisor)
{
    return { RoundDiv<T, D>(p.width, divisor), RoundDiv<T, D>(p.height, divisor) };
}

using SizeI = SizeT<int>;
using SizeF = SizeT<float>;
using SizeD = SizeT<double>;
using Size = SizeF;
}//namespace vgui
