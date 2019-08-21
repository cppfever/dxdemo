#pragma once

#include "dxdemo.hpp"


namespace dxdemo::impl
{

class Win32Window
{
public:

    friend class dxdemo::EventLoop;

    dxdemo::Handle Handle()
    {
        return m_handle;
    }

    void Close()
    {
        ::PostQuitMessage(0);
    }

    int X()
    {
        RECT rect;
        ::GetWindowRect(m_handle, &rect);
        return rect.left;
    }

    int Y()
    {
        RECT rect;
        ::GetWindowRect(m_handle, &rect);
        return rect.top;
    }

    int Width()
    {
        RECT rect;
        ::GetWindowRect(m_handle, &rect);
        return rect.right - rect.left;
    }

    int Height()
    {
        RECT rect;
        ::GetWindowRect(m_handle, &rect);
        return rect.bottom - rect.top;
    }

    int ViewportWidth()
    {
        RECT rect;
        ::GetClientRect(m_handle, &rect);
        return rect.right - rect.left;
    }

    int ViewportHeight()
    {
        RECT rect;
        ::GetClientRect(m_handle, &rect);
        return rect.bottom - rect.top;
    }

    void SetRect(int x, int y, int width, int height)
    {
        ::MoveWindow(m_handle, x, y, width, height, true);
    }

    bool Visible()
    {
        return static_cast<bool>(::IsWindowVisible(m_handle));
    }

    void SetVisible(bool show)
    {
        if(show)
            ::ShowWindow(m_handle, SW_SHOW);
        else
            ::ShowWindow(m_handle, SW_HIDE);
    }

protected:

    virtual void OnIdle(){}
    virtual void OnPaint(){}
    virtual void OnResize(int width, int height){}
    virtual void OnLostFocus(){}
    virtual void OnGainedFocus(){}
    virtual void OnTextEnter(){}
    virtual void OnKeyPress(){}
    virtual void OnKeyRelease(){}
    virtual void OnMouseWheelMove(){}
    virtual void OnMouseWheelScroll(){}
    virtual void OnMouseButtonPress(){}
    virtual void OnMouseButtonRelease(){}
    virtual void OnMouseMove(){}
    virtual void OnMouseEnter(){}
    virtual void OnMouseLeft(){}
    virtual void OnJoystickButtonPress(){}
    virtual void OnJoystickButtonRelease(){}
    virtual void OnJoystickMove(){}
    virtual void OnJoystickConnect(){}
    virtual void OnJoystickDisconnect(){}
    virtual void OnTouchBegin(){}
    virtual void OnTouchMove(){}
    virtual void OnTouchEnd(){}
    virtual void OnSensorChange(){}
    virtual void OnClose(bool& close){}

    virtual bool ProcessEvent(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam, LRESULT& result)
    {
        if(handle == m_handle)
        {
            if(msg == WM_PAINT)
            {
                PAINTSTRUCT ps;
                HDC hdc = ::BeginPaint(handle, &ps);
                OnPaint();
                ::EndPaint(handle, &ps);
                result = 0;
                return true;
            }
            else if(msg == WM_ERASEBKGND)
            {
                result = -1;
                return true;
            }
            else if(msg == WM_SIZE)
            {

                OnResize(LOWORD(lparam), HIWORD(lparam));
                result = 0;
                return true;
            }
            else if(msg == WM_CLOSE)
            {
                bool close = true;
                OnClose(close);

                if(close)
                    Destroy();

                result = 0;
                return true;
            }
        }

        OnIdle();
        return false;
    }

    static LRESULT CALLBACK StaticProcessEvent(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        if(msg == WM_CREATE)
        {
            LONG_PTR win_ptr = LONG_PTR(LPCREATESTRUCT(lparam)->lpCreateParams);

            if(win_ptr)
                SetWindowLongPtr(handle, GWLP_USERDATA, win_ptr);
            else
                throw std::runtime_error("NULL Win32Window pointer in CreateWin32Window function.");

            return 0;
        }
        else if(msg == WM_DESTROY)
        {
            PostQuitMessage(0);
            return 0;
        }

        Win32Window* window = (Win32Window*)::GetWindowLongPtr(handle, GWLP_USERDATA);
        LRESULT result = 0;

        if(window)
        {
            if(window->ProcessEvent(handle, msg, lparam, wparam, result))
                return result;
        }

        //Do not call DefWindowProc from Win32Window::ProcessEvent,  this will cause an OS error.
        return ::DefWindowProc(handle, msg, wparam, lparam);
    }

    static int ProcessEvents()
    {
        MSG msg;
        BOOL ret = FALSE;

        while((ret = GetMessage( &msg, NULL, 0, 0 )) != 0)
        {
            if(ret == -1)
                throw std::runtime_error("Win32 event loop error.");
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        return msg.wParam;
    }

    virtual void Create(const char* title, int x, int y, int width, int height)
    {
        if(m_handle)
            throw std::runtime_error("Wind32 window handle is not NULL. Maybe window created ?.");

        std::wstring classname = L"dxdemo::iWin32Window";
        classname += std::to_wstring(reinterpret_cast<int64_t>(this));
        //std::wcout << classname << std::endl;

        HINSTANCE hinstance = (HINSTANCE)::GetModuleHandle(NULL);

        WNDCLASSEX wndclassex = {0};
        wndclassex.cbSize = sizeof(WNDCLASSEX);
        wndclassex.lpfnWndProc = Win32Window::StaticProcessEvent;
        wndclassex.style = CS_DBLCLKS |CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
        wndclassex.hInstance = hinstance;
        wndclassex.lpszClassName = classname.c_str();
        wndclassex.hCursor = LoadCursor(NULL, IDC_ARROW);

        if(!::RegisterClassEx(&wndclassex))
            throw std::runtime_error("Can't register Win32 window class.");

        //::DwmEnableComposition(DWM_EC_DISABLECOMPOSITION);

        std::wstring wtitle = dxdemo::Utf8ToWstring(title);
        m_handle = ::CreateWindowEx(0, classname.c_str(), wtitle.c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE |WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                                    x, y, width, height, NULL, NULL, hinstance, this);

        if(!m_handle)
            throw std::runtime_error("Can't create Win32 window.");
    }

    virtual void Destroy()
    {
        if(m_handle)
        {
            ::DestroyWindow(m_handle);
            m_handle = NULL;
        }
    }

private:

    dxdemo::Handle m_handle {NULL};
};


}//namespace dxdemo::impl

