#pragma once

#include "dxdemo.hpp"


using namespace Microsoft::WRL;

namespace dxdemo
{

struct D3DVERTEX
{
    FLOAT x, y, z;
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
        params.SwapEffect = D3DSWAPEFFECT_DISCARD;
        params.BackBufferFormat = D3DFMT_X8R8G8B8;
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
        RECT rect;
        ::GetClientRect(Handle(), &rect);

        //IF CLIENT AREA WIDTH OR HEIGHT EQUAL TO ZERO THAN IDirect3dDevice9::Reset
        //WILL CAUSE RUNTIME ERROR
        if(rect.right == 0 || rect.bottom == 0)
            return;

        D3DPRESENT_PARAMETERS params = {0};
        params.Windowed = TRUE;
        params.SwapEffect = D3DSWAPEFFECT_DISCARD;
        params.BackBufferFormat = D3DFMT_X8R8G8B8;
        params.BackBufferCount = 1;
        params.hDeviceWindow = Handle();
        params.BackBufferWidth = width;
        params.BackBufferHeight = height;

        if(FAILED(m_device->Reset(&params)))
            throw std::runtime_error("Direct3D9: can't reset device.");
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
        m_d3dmutex.lock();

        int width = ViewportWidth();
        int height = ViewportHeight();

        m_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 127), 1.0f, 0);

        if(m_device->BeginScene() == D3D_OK)
        {
            static float angle{0};
            angle += 0.1f;

            D3DXMATRIX rotation;
            D3DXMatrixRotationZ(&rotation, angle);
            if(FAILED(m_device->SetTransform(D3DTS_WORLD, &rotation)))
               throw std::runtime_error("Direct3D9: can't create vertex buffer.");

            m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
            m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
            m_device->SetRenderState(D3DRS_AMBIENT,RGB(255,255,255));
            m_device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

            D3DVERTEX vertices[3];

            vertices[0].x = -1.0;
            vertices[0].y = -1.0;
            vertices[0].z = 0.0;
            vertices[0].color = D3DCOLOR_XRGB(255, 0, 0);

            vertices[1].x = 0.0;
            vertices[1].y = 1.0;
            vertices[1].z = 0.0;
            vertices[1].color = D3DCOLOR_XRGB(0, 255, 0);

            vertices[2].x =  1.0;
            vertices[2].y = -1.0;
            vertices[2].z = 0.0;
            vertices[2].color = D3DCOLOR_XRGB(0, 0, 255);

            ComPtr<IDirect3DVertexBuffer9> vertex_buffer;
            void *video_memory = nullptr;

            if(FAILED(m_device->CreateVertexBuffer(3*sizeof(D3DVERTEX), 0,
                                                   D3DFVF_XYZ|D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &vertex_buffer, NULL)))
                throw std::runtime_error("Direct3D9: can't create vertex buffer.");

            if(FAILED(vertex_buffer->Lock(0, 3*sizeof(D3DVERTEX), &video_memory, 0)))
                throw std::runtime_error("Direct3D9: can't lock vertex object.");

            memcpy(video_memory, vertices, 3*sizeof(D3DVERTEX));

            if(FAILED(vertex_buffer->Unlock()))
                throw std::runtime_error("Direct3D9: can't unlock vertex object.");

            if(FAILED(m_device->SetStreamSource(0, vertex_buffer.Get(), 0, sizeof(D3DVERTEX))))
                throw std::runtime_error("Direct3D9: can't set stream source.");

            if(FAILED(m_device->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE)))
                throw std::runtime_error("Direct3D9: can't set format.");

            if(FAILED(m_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1)))
                throw std::runtime_error("Direct3D9: can't draw primitives.");

            if(FAILED(m_device->EndScene()))
                throw std::runtime_error("Direct3D9: can't end scene.");

            if(FAILED(m_device->Present(NULL,NULL, NULL, NULL)))
                throw std::runtime_error("Direct3D9: can't present.");

            vertex_buffer.Reset();
        }

        m_d3dmutex.unlock();
    }

    void OnResize(int width, int height) override
    {
        m_d3dmutex.lock();
        ResetDevice(0, 0);
        m_d3dmutex.unlock();
    }

private:    

    ComPtr<IDirect3DDevice9> m_device;
    ComPtr<IDirect3DSurface9> m_surface;
    std::mutex m_d3dmutex;
    Point m_a{0.5, 0.1};
    Point m_b{0.9, 0.9};
    Point m_c{0.1, 0.9};
    static inline ComPtr<IDirect3D9> s_d3d = ::Direct3DCreate9(D3D_SDK_VERSION);
};//class DX9Window

}//namespace dxdemo

