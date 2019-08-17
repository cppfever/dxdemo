#pragma once

#include "dxdemo.hpp"

namespace dxdemo
{

class WindowBase;

class EventLoop
{
public:

    int Run()
    {
        while(WindowBase::s_windows.size())
        {
            for(auto window : WindowBase::s_windows)
                window->ProcessEvents();

            for(auto window : WindowBase::s_windows)
                window->OnIdle();
        }

        return 777;
    }

};//class EventLoop

} // namespace dxdemo
