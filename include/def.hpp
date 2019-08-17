#pragma once

#include "dxdemo.hpp"


namespace dxdemo
{
//https://eb2.co/blog/2014/04/c--14-and-sdl2-managing-resources/
template<typename Creator, typename Destructor, typename... Arguments>
auto make_resource(Creator c, Destructor d, Arguments&&... args);

using Handle = HWND;

std::wstring Utf8ToWstring(const char* str)
{
    size_t length = std::strlen(str);

    if(length != 0)
    {
        int num_chars = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str, length, NULL, 0);
        std::wstring wstrTo;
        if(num_chars)
        {
            wstrTo.resize(num_chars);
            if(::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str, length, &wstrTo[0], num_chars))
            {
                return wstrTo;
            }
        }
    }

    return std::wstring();
}


/**
  If return type of 'operator' std::is_integral,
  and argement type of 'operator' std::is_floating_point,
  then std::round is the result.
  Otherwise used 'operator' without rounding.
*/
#define ROUND_OPERATOR(Name, op) \
    template<typename R, typename A> \
    R Name(R arg1, A arg2) \
{ if constexpr(std::is_integral<R>::value == true && \
    std::is_floating_point<A>::value == true) \
    return static_cast<R>(std::round(arg1 op arg2)); \
    else return static_cast<R>(arg1 op arg2); }


ROUND_OPERATOR(RoundAdd, +)
ROUND_OPERATOR(RoundSub, -)
ROUND_OPERATOR(RoundMult, *)
ROUND_OPERATOR(RoundDiv, /)

}//namespace vgui

