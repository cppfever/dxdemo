#pragma once

#include "dxdemo.hpp"


namespace dxdemo
{

class WindowBase : public impl::Win32Window
{
protected:

    void Create(const char* title, int x, int y, int width, int height) override
    {
        impl::Win32Window::Create(title, x, y, width, height);
        AddToEventLoop(this);
    }

    void Destroy()
    {
       impl::Win32Window::Destroy();
        RemoveFromEventLoop(this);
    }

private:    

    static void AddToEventLoop(WindowBase* window)
    {
        auto i = std::find(s_windows.begin(), s_windows.end(), window);

        if( i== s_windows.end())
            s_windows.push_back(window);
    }

    static void RemoveFromEventLoop(WindowBase* window)
    {
        auto i = std::find(s_windows.begin(), s_windows.end(), window);

        if( i!= s_windows.end())
            s_windows.erase(i);
    }

    friend class dxdemo::EventLoop;
    static inline std::vector<impl::Win32Window*> s_windows;

};//class WindowBase

}//namespace dxdemo

