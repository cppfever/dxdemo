#pragma once

#include "dxdemo.hpp"


namespace dxdemo::impl
{

template<typename T>
class Win32Window
{
public:

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
        return Position().x;
    }

    int Y()
    {
        return Position().y;
    }

    dxdemo::PointI Position()
    {
        RECT rect;
        ::GetWindowRect(m_handle, &rect);
        return dxdemo::PointI(rect.left, rect.top);
    }

    int Width()
    {
        return Size().width;
    }

    int Height()
    {
        return Size().height;
    }

    dxdemo::SizeI Size()
    {
        RECT rect;
        ::GetWindowRect(m_handle, &rect);
        return dxdemo::SizeI(rect.right - rect.left, rect.bottom - rect.top);
    }

    dxdemo::RectI Rect()
    {
        RECT rect;
        ::GetWindowRect(m_handle, &rect);
        return dxdemo::RectI(rect.left, rect.top, rect.right, rect.bottom);
    }

    bool Visible()
    {
        return static_cast<bool>(::IsWindowVisible(m_handle));
    }

    T& SetX(int x)
    {
        dxdemo::RectI rect = Rect();
        return SetRect(x, rect.y, rect.width, rect.height);
    }

    T& SetY(int y)
    {
        dxdemo::RectI rect = Rect();
        return SetRect(rect.x, y, rect.width, rect.height);
    }

    T& SetPosition(int x, int y)
    {
        dxdemo::SizeI size = Size();
        return SetRect(x, y, size.width, size.height);
    }

    T& SetPosition(Point position)
    {
        dxdemo::SizeI size = Size();
        return SetRect(position.x, position.y, size.width, size.height);
    }

    T& SetWidth(int width)
    {
        dxdemo::RectI rect = Rect();
        return SetRect(rect.x, rect.y, width, rect.height);
    }

    T& SetHeight(int height)
    {
        dxdemo::RectI rect = Rect();
        return SetRect(rect.x, rect.y, rect.width, height);
    }

    T& SetSize(int width, int height)
    {
        dxdemo::PointI pos = Position();
        return SetRect(pos.x, pos.y, width, height);
    }

    T& SetSize(dxdemo::SizeI size)
    {
        dxdemo::PointI pos = Position();
        return SetRect(pos.x, pos.y, size.width, size.height);
    }

    T& SetRect(dxdemo::RectI rect)
    {
        return SetRect(rect.x, rect.y, rect.width, rect.height);
    }

    T& SetRect(int x, int y, int width, int height)
    {
        ::MoveWindow(m_handle, x, y, width, height, true);
        return CastThis();
    }

    T& SetVisible(bool show)
    {
        if(show)
            ::ShowWindow(m_handle, SW_SHOW);
        else
            ::ShowWindow(m_handle, SW_HIDE);

        return CastThis<T>();
    }

protected:

    template<typename U>
    U& CastThis()
    {
        return dynamic_cast<U&>(*this);
    }

    virtual void OnIdle(){}
    virtual void OnDraw(){}
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
                OnDraw();
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
                bool close = false;
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
                throw ExceptionInfo << "NULL Win32Window pointer in CreateWin32Window function.";

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
                throw ExceptionInfo << "Win32 event loop error.";
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
            throw ExceptionInfo << "Wind32 window handle is not NULL. Maybe window created ?.";

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
            throw ExceptionInfo << "Can't register Win32 window class.";

        //::DwmEnableComposition(DWM_EC_DISABLECOMPOSITION);

        std::wstring wtitle = dxdemo::Utf8ToWstring(title);
        m_handle = ::CreateWindowEx(0, classname.c_str(), wtitle.c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE |WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                                    x, y, width, height, NULL, NULL, hinstance, this);

        if(!m_handle)
            throw ExceptionInfo << "Can't create Win32 window.";
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

