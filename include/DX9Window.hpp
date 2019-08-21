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

struct Point
{
    float x;
    float y;
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

        CreateDevice(0, 0);
    }

    void CreateDevice(int width, int height)
    {
        D3DPRESENT_PARAMETERS params = {0};
        params.Windowed = TRUE;
        params.SwapEffect = D3DSWAPEFFECT_FLIP;
        params.BackBufferFormat = D3DFMT_A8R8G8B8;
        params.BackBufferCount = 1;
        params.hDeviceWindow = Handle();
        params.BackBufferWidth = width;
        params.BackBufferHeight = height;

        if(FAILED(s_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
                                      Handle(), D3DCREATE_HARDWARE_VERTEXPROCESSING, &params, &m_device)))
            throw std::runtime_error("Direct3D9: can't create device.");
    }

    void ResetDevice(int width, int height)
    {
        HRESULT result = m_device->TestCooperativeLevel();

        if(result == D3DERR_DEVICELOST)
            return;
        else
            if(result == D3DERR_DEVICENOTRESET)
                ResetDevice(width, height);
            else
                if(result == D3DERR_DRIVERINTERNALERROR)
                    throw std::runtime_error("Direct3D9: device driver iternal error.");


        D3DPRESENT_PARAMETERS params = {0};
        params.Windowed = TRUE;
        params.SwapEffect = D3DSWAPEFFECT_FLIP;
        params.BackBufferFormat = D3DFMT_A8R8G8B8;
        params.BackBufferCount = 1;
        params.hDeviceWindow = Handle();
        params.BackBufferWidth = width;
        params.BackBufferHeight = height;

        if(FAILED(m_device->Reset(&params)))
            throw std::runtime_error("Direct3D9: can't reset device.");
    }

    bool TryResetDevice(int width, int height)
    {
        throw std::runtime_error("UNREACHABLE POINT.");
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

        int width = ViewportWidth();
        int height = ViewportHeight();

        m_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 127), 1.0f, 0);

        if(m_device->BeginScene() == D3D_OK)
        {
            D3DVERTEX vertices[3];

            vertices[0].x = (float)width * m_a.x;
            vertices[0].y = (float)height * m_a.y;
            vertices[0].z = 0.5;
            vertices[0].rhw = 1.0f;
            vertices[0].color = 0x00ff00;

            vertices[1].x = (float)width * m_b.x;
            vertices[1].y = (float)height * m_b.y;
            vertices[1].z = 0.5;
            vertices[1].rhw = 1.0f;
            vertices[1].color = 0x0000ff;

            vertices[2].x = (float)width * m_c.x;
            vertices[2].y = (float)height * m_c.y;
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



        //        D3DXMATRIX matProjection;						// Создаем проекционную матрицу
        //        D3DXMatrixPerspectiveFovLH(&matProjection, 45.0f, (float)width/(float)height, 0.1f, 100.0f );

        //        // Устанавливаем проекционную матрицу
        //        if(m_device)
        //            m_device->SetTransform( D3DTS_PROJECTION, &matProjection );
        ResetDevice(0, 0);
    }

private:    

    ComPtr<IDirect3DDevice9> m_device;
    ComPtr<IDirect3DSurface9> m_surface;
    std::mutex m_paintmutex;
    Point m_a{0.5, 0.1};
    Point m_b{0.9, 0.9};
    Point m_c{0.1, 0.9};
    static inline ComPtr<IDirect3D9> s_d3d = ::Direct3DCreate9(D3D_SDK_VERSION);
};//class DX9Window

}//namespace dxdemo

