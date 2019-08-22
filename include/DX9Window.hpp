#pragma once

#include "dxdemo.hpp"


using namespace Microsoft::WRL;

namespace dxdemo
{

struct D3DVERTEX
{
    float x, y, z;
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
        params.BackBufferFormat = D3DFMT_A8R8G8B8;
        params.BackBufferCount = 1;
        params.hDeviceWindow = Handle();
        params.BackBufferWidth = width;
        params.BackBufferHeight = height;

        if(FAILED(s_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
                                      Handle(), D3DCREATE_SOFTWARE_VERTEXPROCESSING, &params, &m_device)))
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
        params.SwapEffect = D3DSWAPEFFECT_FLIP;
        params.BackBufferFormat = D3DFMT_A8R8G8B8;
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

    void SetupMatrix()
    {
        D3DXMATRIX MatrixWorld;
        D3DXMATRIX MatrixView;
        D3DXMATRIX MatrixProjection;
        UINT iTime=timeGetTime()%5000;
        FLOAT fAngle=iTime*(2.0f*D3DX_PI)/5000.0f;
        D3DXMatrixRotationX(&MatrixWorld, fAngle);
        if(FAILED(m_device->SetTransform(D3DTS_WORLD, &MatrixWorld)))
                throw std::runtime_error("Direct3D9: can't create vertex buffer.");

        D3DXVECTOR3 from(4.0f, 4.0f, 4.0f);
        D3DXVECTOR3 to(0.0f, 0.0f, 0.0f);
        D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
        D3DXMatrixLookAtLH(&MatrixView, &from, &to, &up);
        if(FAILED(m_device->SetTransform(D3DTS_VIEW, &MatrixView)))
           throw std::runtime_error("Direct3D9: can't create vertex buffer.");

        D3DXMatrixPerspectiveFovLH(&MatrixProjection, D3DX_PI/4, 1.0f, 1.0f, 100.0f);
        if(FAILED(m_device->SetTransform(D3DTS_PROJECTION, &MatrixProjection)))
           throw std::runtime_error("Direct3D9: can't create vertex buffer.");
    }

    void OnPaint() override
    {
        m_d3dmutex.lock();

        int width = ViewportWidth();
        int height = ViewportHeight();

        m_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 127), 1.0f, 0);

        if(m_device->BeginScene() == D3D_OK)
        {
            SetupMatrix();
            static float angle{0};
            angle += 0.1f;
            std::wstringstream str;
            str<< angle;
            ::SetWindowText(Handle(), str.str().c_str());

            D3DXMATRIX rotation;
            /*D3DXMatrixRotationX(&rotation, angle);
            if(FAILED(m_device->SetTransform(D3DTS_WORLD, &rotation)))
               throw std::runtime_error("Direct3D9: can't create vertex buffer.");

            D3DXMATRIX view;
            D3DXVECTOR3 from(4.0f, 4.0f, 4.0f);
            D3DXVECTOR3 to(0.0f, 0.0f, 0.0f);
            D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
            D3DXMatrixLookAtLH(&view, &from, &to, &up);
            if(FAILED(m_device->SetTransform(D3DTS_VIEW, &view)))
               throw std::runtime_error("Direct3D9: can't create vertex buffer.");

            D3DXMATRIX ortho;
            D3DXMatrixOrthoLH(&ortho, width, height,-1.0f, 1.0f);
            if(FAILED(m_device->SetTransform(D3DTS_VIEW, &view)))
               throw std::runtime_error("Direct3D9: can't create vertex buffer.");
*/

            D3DVERTEX vertices[3];

            vertices[0].x = 0.0;
            vertices[0].y = 0.0;
            vertices[0].z = 0.5;
            vertices[0].color = 0x00ff00;

            vertices[1].x = m_b.x;
            vertices[1].y = m_b.y;
            vertices[1].z = 0.5;
            vertices[1].color = 0x0000ff;

            vertices[2].x =  m_c.x;
            vertices[2].y = m_c.y;
            vertices[2].z = 0.5;
            vertices[2].color = 0xff0000;

            ComPtr<IDirect3DVertexBuffer9> vertex_buffer;
            void *video_memory = nullptr;

            if(FAILED(m_device->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE)))
                throw std::runtime_error("Direct3D9: can't set format.");

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

