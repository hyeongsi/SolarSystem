#pragma once
#include <D3D11.h>
#include <D3DX11.h>
#include <DirectXMath.h>
#include <vector>
#include <string>
#include "vertexNbufferStructResource.h"

using namespace std;

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
    ID3D11PixelShader* m_pSolidPixelShader = NULL;
    ID3D11InputLayout* m_pVertexLayout = NULL;

    vector<string> m_texturePath;
    vector<ID3D11ShaderResourceView*> m_pSolarSystemTextureRV;
    ID3D11SamplerState* m_pSamplerState = NULL;

    HRESULT LoadTexture(const char* filePath);

public:
	GraphicClass(HWND* hwnd);
    void Shutdown();

    UINT GetWidth();
    UINT GetHeight();

    ID3D11Device* GetDevice();
    ID3D11DeviceContext* GetImmediateContext();
    vector<ID3D11ShaderResourceView*> GetShaderResourceViewVector();

    HRESULT InitGraphicClass();

    void Update();
    void Render();

    void SetIAVertexBuffer(ID3D11Buffer* vertexBuffer, UINT stride, UINT offset);
    void SetIAIndexBuffer(ID3D11Buffer* indexBuffer);
    void SetPixelShader(PixelShaderNumber pixelShaderNumver);
};

