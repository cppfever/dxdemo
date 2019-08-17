#pragma once

#include "dxdemo.hpp"


namespace dxdemo
{

template<typename T>
class EventLoopT;

template<typename T>
class WindowBase : public impl::Win32Window<T>
{
protected:

    void Create(const char* title, int x, int y, int width, int height) override
    {
        impl::Win32Window<T>::Create(title, x, y, width, height);
        AddToEventLoop(this);
    }

    void Destroy()
    {
       impl::Win32Window<T>::Destroy();
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

    friend class dxdemo::EventLoopT<T>;
    static inline std::vector<dxdemo::WindowBase<T>*> s_windows;

};//class WindowBase

}//namespace vgui

