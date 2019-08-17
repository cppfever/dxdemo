#pragma once

#include "dxdemo.hpp"


namespace dxdemo
{

class DX9Window : public WindowBase<DX9Window>
{
public:

    DX9Window(const char* title, int x, int y, int width, int height)
    {
        ::CoInitialize(NULL);
        Create(title, x, y, width, height);
    }

    virtual ~DX9Window()
    {
        Destroy();
        ::CoUninitialize();
    }

protected:

    void Create(const char* title, int x, int y, int width, int height) override
    {
        WindowBase::Create(title, x, y, width, height);

        RECT src_rect;
        //::GetClientRect(src_window, &src_rect);

        m_d3d = ::Direct3DCreate9(D3D_SDK_VERSION);
        if(!m_d3d)
            throw ExceptionInfo << "Direct3D9. Can't create Direct3D object.";

        D3DDISPLAYMODE mode;
        if(m_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode) < 0)
            throw ExceptionInfo << "Direct3D9. Can't retrieve the current display mode of the adapter.";

        HWND hwnd = ::FindWindow(NULL, L"Heroes of Might & Magic III - HD Edition");
        RECT rect;
        ::GetClientRect(hwnd, &rect);

        D3DPRESENT_PARAMETERS param {0};

        param.Windowed = TRUE;
        param.BackBufferCount = 1;
        param.BackBufferHeight = mode.Height;
        param.BackBufferWidth = mode.Width;
        param.SwapEffect = D3DSWAPEFFECT_DISCARD;
        param.hDeviceWindow = Handle();//NULL;//::GetDesktopWindow();

        // create device & capture surface
        if(m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, NULL, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &param, &m_device) < 0)
            throw ExceptionInfo << "Direct3D9. Can't create device.";
    }


    void CopyWindowContent(HWND src_window)
    {
        m_device->BeginScene();
        Microsoft::WRL::ComPtr<IDirect3DSurface9> src_surface;

//        if(m_device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &src_surface) < 0)
        if(m_device->GetRenderTarget(0, &src_surface) < 0)
            throw ExceptionInfo << "Direct3D9. Can't create offscreen surface.";

        D3DSURFACE_DESC sd;
        if(src_surface->GetDesc(&sd) < 0)
            throw ExceptionInfo << "Direct3D9. Can't create offscreen surface.";

        if(m_device->CreateOffscreenPlainSurface(sd.Width, sd.Height, sd.Format, D3DPOOL_SYSTEMMEM, &m_surface, nullptr) < 0)
            throw ExceptionInfo << "Direct3D9. Can't get source surface.";

//        if(m_device->ColorFill(src_surface.Get(), NULL, D3DCOLOR(D3DCOLOR_ARGB(0,255,0,0))) <0)
//            throw ExceptionInfo << "Direct3D9. Can't fill color.";

        m_device->EndScene();

        if(m_device->Present(NULL, NULL, Handle(), NULL) < 0)
            throw ExceptionInfo << "Direct3D9. Can't copy frontbuffer to backbuffer.";

        if(m_device->GetRenderTargetData(src_surface.Get(), m_surface.Get()) < 0)
            throw ExceptionInfo << "Direct3D9. Can't create offscreen surface.";

        D3DXSaveSurfaceToFile(L"1.bmp", D3DXIFF_BMP, m_surface.Get(), NULL, NULL);
    }

    void Destroy()
    {
        Release();
        WindowBase::Destroy();

    }

    void Release()
    {
        if(m_surface) m_surface->Release();
        if(m_device) m_device->Release();
        if(m_d3d) m_d3d->Release();
    }

    void OnDraw() override
    {
        dxdemo::SizeI size = Size();
        CopyWindowContent(NULL);

    }

    void OnResize(int width, int height) override
    {

    }

private:    

    Microsoft::WRL::ComPtr<IDirect3D9> m_d3d;
    Microsoft::WRL::ComPtr<IDirect3DDevice9> m_device;
    Microsoft::WRL::ComPtr<IDirect3DSurface9> m_surface;

};//class DX9Window

using EventLoop = dxdemo::EventLoopT<dxdemo::DX9Window>;

}//namespace dxdemo

