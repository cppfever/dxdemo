#pragma once

#include "dxdemo.hpp"

namespace dxdemo
{

class Window;

class EventLoop
{
public:

    int Run()
    {
        while(Window::s_windows.size())
        {
            for(auto window : Window::s_windows)
                window->ProcessEvents();

            for(auto window : Window::s_windows)
                window->OnIdle();
        }

        return 777;
    }

};//class EventLoop

} // namespace dxdemo
