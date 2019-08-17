#pragma once

#include "dxdemo.hpp"


namespace dxdemo
{

class Window : public impl::Win32Window
{
public:

    Window()
    {}

    Window(const char* title, int x, int y, int width, int height)
    {
        Create(title, x, y, width, height);
    }

    virtual ~Window()
    {
        Destroy();
    }

    void SetPaintTimer(uint32_t period)
    {
        std::thread([=]()
        {
            while(true)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(period));
                OnPaint();
                m_thick++;
            }
        }).detach();
    }

    int GetThick()
    {
        return m_thick;
    }

protected:

    void Create(const char* title, int x, int y, int width, int height) override
    {
        AddToEventLoop(this);
        impl::Win32Window::Create(title, x, y, width, height);
    }

    void Destroy()
    {
       impl::Win32Window::Destroy();
        RemoveFromEventLoop(this);
    }

private:    

    static void AddToEventLoop(Window* window)
    {
        auto i = std::find(s_windows.begin(), s_windows.end(), window);

        if( i== s_windows.end())
            s_windows.push_back(window);
    }

    static void RemoveFromEventLoop(Window* window)
    {
        auto i = std::find(s_windows.begin(), s_windows.end(), window);

        if( i!= s_windows.end())
            s_windows.erase(i);
    }

    friend class dxdemo::EventLoop;
    static inline std::vector<impl::Win32Window*> s_windows;
    int m_thick = 0;

};//class Window

}//namespace dxdemo

