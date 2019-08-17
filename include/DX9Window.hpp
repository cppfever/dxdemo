#pragma once

#include "dxdemo.hpp"


using namespace Microsoft::WRL;

namespace dxdemo
{

struct D3DVERTEX
{
   float x, y, z, rhw;
   DWORD color;
};

class DX9Window : public Window
{
public:

    DX9Window(const char* title, int x, int y, int width, int height) :
        Window()
    {        
        if(FAILED(::CoInitialize(NULL)))
            throw std::runtime_error("COM initializing error.");

        Create(title, x, y, width, height);
    }

    virtual ~DX9Window()
    {
        ::CoUninitialize();
    }

protected:

    void Create(const char* title, int x, int y, int width, int height) override
    {
        Window::Create(title, x, y, width, height);

        if(!s_d3d)
            throw std::runtime_error("Direct3D9: Initialization error.");

        D3DPRESENT_PARAMETERS params = {0};
        params.Windowed = TRUE;
        params.SwapEffect = D3DSWAPEFFECT_FLIP;
        params.BackBufferFormat = D3DFMT_A8R8G8B8;
        params.BackBufferWidth = 640;
        params.BackBufferHeight = 480;
        params.BackBufferCount = 1;
        params.hDeviceWindow = Handle();

        if(FAILED(s_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
                                      Handle(), D3DCREATE_HARDWARE_VERTEXPROCESSING, &params, &m_device)))
                throw std::runtime_error("Direct3D9: can't create device.");
    }


    void CopyWindowContent(HWND src_window)
    {
        m_device->BeginScene();
        Microsoft::WRL::ComPtr<IDirect3DSurface9> src_surface;

//        if(m_device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &src_surface) < 0)
        if(m_device->GetRenderTarget(0, &src_surface) < 0)
            throw std::runtime_error("Direct3D9. Can't create offscreen surface.");

        D3DSURFACE_DESC sd;
        if(src_surface->GetDesc(&sd) < 0)
            throw std::runtime_error("Direct3D9. Can't create offscreen surface.");

        if(m_device->CreateOffscreenPlainSurface(sd.Width, sd.Height, sd.Format, D3DPOOL_SYSTEMMEM, &m_surface, nullptr) < 0)
            throw std::runtime_error("Direct3D9. Can't get source surface.");

//        if(m_device->ColorFill(src_surface.Get(), NULL, D3DCOLOR(D3DCOLOR_ARGB(0,255,0,0))) <0)
//            throw std::runtime_error("Direct3D9. Can't fill color.");

        m_device->EndScene();

        if(m_device->Present(NULL, NULL, Handle(), NULL) < 0)
            throw std::runtime_error("Direct3D9. Can't copy frontbuffer to backbuffer.");

        if(m_device->GetRenderTargetData(src_surface.Get(), m_surface.Get()) < 0)
            throw std::runtime_error("Direct3D9. Can't create offscreen surface.");

        D3DXSaveSurfaceToFile(L"1.bmp", D3DXIFF_BMP, m_surface.Get(), NULL, NULL);
    }

    void Destroy()
    {
        Reset();
        Window::Destroy();

    }

    void Reset()
    {
        if(m_surface) m_surface.Reset();
        if(m_device) m_device.Reset();
    }

    void OnPaint() override
    {
        m_paintmutex.lock();

        int width = Width();
        int height = Height();

        m_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 127), 1.0f, 0);

        if(m_device->BeginScene() == D3D_OK)
        {
            D3DVERTEX vertices[3];

            vertices[0].x = 50 + GetThick();
            vertices[0].y = 50;
            vertices[0].z = 0.5;
            vertices[0].rhw = 1.0f;
            vertices[0].color = 0x00ff00;

            vertices[1].x = 250 + GetThick();
            vertices[1].y = 50;
            vertices[1].z = 0.5;
            vertices[1].rhw = 1.0f;
            vertices[1].color = 0x0000ff;

            vertices[2].x = 50 + GetThick();
            vertices[2].y = 250;
            vertices[2].z = 0.5;
            vertices[2].rhw = 1.0f;
            vertices[2].color = 0xff0000;

            ComPtr<IDirect3DVertexBuffer9> vertex_buffer;
            void *video_memory = nullptr;

            if(FAILED(m_device->CreateVertexBuffer(3*sizeof(D3DVERTEX), 0,
                      D3DFVF_XYZRHW|D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &vertex_buffer, NULL)))
               throw std::runtime_error("Direct3D9: can't create vertex buffer.");

            if(FAILED(vertex_buffer->Lock(0, 3*sizeof(D3DVERTEX), &video_memory, 0)))
               throw std::runtime_error("Direct3D9: can't lock vertex object.");

            memcpy(video_memory, vertices, 3*sizeof(D3DVERTEX));
            if(FAILED(vertex_buffer->Unlock()))
                    throw std::runtime_error("Direct3D9: can't unlock vertex object.");

            if(FAILED(m_device->SetStreamSource(0, vertex_buffer.Get(), 0, sizeof(D3DVERTEX))))
               throw std::runtime_error("Direct3D9: can't set stream source.");

            if(FAILED(m_device->SetFVF(D3DFVF_XYZRHW|D3DFVF_DIFFUSE)))
               throw std::runtime_error("Direct3D9: can't set format.");

            if(FAILED(m_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1)))
               throw std::runtime_error("Direct3D9: can't draw primitives.");

            if(FAILED(m_device->EndScene()))
               throw std::runtime_error("Direct3D9: can't end scene.");

            if(FAILED(m_device->Present(NULL,NULL, NULL, NULL)))
               throw std::runtime_error("Direct3D9: can't present.");

            vertex_buffer.Reset();
        }

        m_paintmutex.unlock();
    }

    void OnResize(int width, int height) override
    {

    }

private:    

    ComPtr<IDirect3DDevice9> m_device;
    ComPtr<IDirect3DSurface9> m_surface;
    std::mutex m_paintmutex;
    static inline ComPtr<IDirect3D9> s_d3d = ::Direct3DCreate9(D3D_SDK_VERSION);
};//class DX9Window

}//namespace dxdemo

