#pragma once

#include "dxdemo.hpp"


namespace dxdemo
{
class EventLoop;

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

}//namespace dxdemo

