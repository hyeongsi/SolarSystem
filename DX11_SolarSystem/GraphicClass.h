#pragma once
#include <D3D11.h>
#include <D3DX11.h>
#include <xnamath.h>

class GraphicClass
{
private:
    HWND* g_hWnd;

    UINT m_width;
    UINT m_height;

    D3D_DRIVER_TYPE driverTypes[3];
    UINT numDriverTypes;
    D3D_FEATURE_LEVEL featureLevels[3];
    UINT numFeatureLevels;

    D3D_DRIVER_TYPE m_driverType;
    D3D_FEATURE_LEVEL m_featureLevel;

    IDXGISwapChain* m_pSwapChain;
    ID3D11Device* m_pd3dDevice;
    ID3D11DeviceContext* m_pImmediateContext;

    ID3D11RenderTargetView* m_pRenderTargetView;
    ID3D11Texture2D* m_pDepthStencil;
    ID3D11DepthStencilView* m_pDepthStencilView;

    ID3D11VertexShader* m_pVertexShader = NULL;
    ID3D11PixelShader* m_pPixelShader = NULL;
    ID3D11InputLayout* m_pVertexLayout = NULL;

public:
	GraphicClass(HWND* hwnd);
    void Shutdown();

    UINT GetWidth();
    UINT GetHeight();

    ID3D11Device* GetDevice();
    ID3D11DeviceContext* GetImmediateContext();

    HRESULT InitGraphicClass();
    void Update();
    void Render();

    void SetIAVertexBuffer(ID3D11Buffer* vertexBuffer, UINT stride, UINT offset);
    void SetIAIndexBuffer(ID3D11Buffer* indexBuffer);
};

