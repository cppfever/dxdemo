#pragma once

#include "dxdemo.hpp"

namespace dxdemo
{

template<typename T>
class WindowBase;

template<typename T>
class EventLoopT
{
public:

    int Run()
    {
        while(WindowBase<T>::s_windows.size())
        {
            for(auto window : WindowBase<T>::s_windows)
                window->ProcessEvents();

            for(auto window : WindowBase<T>::s_windows)
                window->OnIdle();
        }

        return 0;
    }

};//class EventLoopT

} // namespace vgui
