#pragma once

#include "dxdemo.hpp"


namespace dxdemo
{

class Window : public WindowBase
{
public:

    Window(const char* title, int x, int y, int width, int height)
    {
        Create(title, x, y, width, height);
    }

    virtual ~Window()
    {
        Destroy();
    }

protected:

    void Create(const char* title, int x, int y, int width, int height) override
    {
        WindowBase::Create(title, x, y, width, height);
    }

    void Destroy() override
    {
        WindowBase::Destroy();
    }

    void OnDraw() override
    {

    }

    void OnResize(int width, int height) override
    {

    } 

};//class Window

}//namespace dxdemo

