#include "GraphicClass.h"
#include "ObjLoader.h"
#include "CameraClass.h"
#include <d3dcompiler.h>

GraphicClass::GraphicClass(HWND* hwnd)
{
	g_hWnd = hwnd;

	RECT rc;
	GetClientRect(*hwnd, &rc);

	m_width = rc.right - rc.left;
	m_height = rc.bottom - rc.top;

	numDriverTypes = ARRAYSIZE(driverTypes);
	driverTypes[0] = D3D_DRIVER_TYPE_HARDWARE;
	driverTypes[1] = D3D_DRIVER_TYPE_WARP;
	driverTypes[2] = D3D_DRIVER_TYPE_REFERENCE;
	
	numFeatureLevels = ARRAYSIZE(featureLevels);
	featureLevels[0] = D3D_FEATURE_LEVEL_11_0;
	featureLevels[1] = D3D_FEATURE_LEVEL_10_1;
	featureLevels[2] = D3D_FEATURE_LEVEL_10_0;

	m_driverType = D3D_DRIVER_TYPE_NULL;
	m_featureLevel = D3D_FEATURE_LEVEL_11_0;

	m_pSwapChain = nullptr;
	m_pd3dDevice = nullptr;
	m_pImmediateContext = nullptr;

	m_pRenderTargetView = nullptr;
	m_pDepthStencil = nullptr;
	m_pDepthStencilView = nullptr;

	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
	m_pVertexLayout = nullptr;
	m_pVertexBuffer = nullptr;
}

void GraphicClass::Shutdown()
{
	if (m_pIndexBuffer)			m_pIndexBuffer->Release();
	if (m_pVertexBuffer)		m_pVertexBuffer->Release();
	if (m_pVertexLayout)		m_pVertexLayout->Release();
	if (m_pVertexShader)		m_pVertexShader->Release();
	if (m_pPixelShader)			m_pPixelShader->Release();
	if (m_pDepthStencil)		m_pDepthStencil->Release();
	if (m_pDepthStencilView)	m_pDepthStencilView->Release();
	if (m_pRenderTargetView)	m_pRenderTargetView->Release();
	if (m_pSwapChain)			m_pSwapChain->Release();
	if (m_pImmediateContext)	m_pImmediateContext->Release();
	if (m_pImmediateContext)	m_pImmediateContext->Release();
}

UINT GraphicClass::GetWidth()
{
	return m_width;
}

UINT GraphicClass::GetHeight()
{
	return m_height;
}

ID3D11Device* GraphicClass::GetDevice()
{
	return m_pd3dDevice;
}

ID3D11DeviceContext* GraphicClass::GetImmediateContext()
{
	return m_pImmediateContext;
}

HRESULT GraphicClass::InitGraphicClass()
{
	HRESULT hr = S_OK;

	UINT createDeviceFlags = 0;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = m_width;
	swapChainDesc.BufferDesc.Height = m_height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = *g_hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		m_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, m_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &swapChainDesc, &m_pSwapChain, &m_pd3dDevice, &m_featureLevel, &m_pImmediateContext);
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;

	ID3D11Texture2D* pBackBuffer = NULL;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return hr;

	hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = m_width;
	descDepth.Height = m_height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = m_pd3dDevice->CreateTexture2D(&descDepth, NULL, &m_pDepthStencil);
	if (FAILED(hr))
		return hr;

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = m_pd3dDevice->CreateDepthStencilView(m_pDepthStencil, &descDSV, &m_pDepthStencilView);
	if (FAILED(hr))
		return hr;

	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	D3D11_VIEWPORT viewPort;
	viewPort.Width = (FLOAT)m_width;
	viewPort.Height = (FLOAT)m_height;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	m_pImmediateContext->RSSetViewports(1, &viewPort);

	ID3DBlob* pVSBlob = NULL;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	wchar_t szfileName[20] = L"temp.fx";
	hr = D3DX11CompileFromFileW(szfileName, NULL, NULL, "VS", "vs_4_0", dwShaderFlags, 0, NULL, &pVSBlob, NULL, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			"fx not exists, (VS)", "Error", MB_OK);
		return hr;
	}

	hr = m_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &m_pVertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	hr = m_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &m_pVertexLayout);
	pVSBlob->Release();
	if (FAILED(hr))
		return hr;

	m_pImmediateContext->IASetInputLayout(m_pVertexLayout);

	ID3DBlob* pPSBlob = NULL;
	hr = D3DX11CompileFromFileW(szfileName, NULL, NULL, "PS", "ps_4_0", dwShaderFlags, 0, NULL, &pPSBlob, NULL, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			"fx not exists, (PS)", "Error", MB_OK);
		return hr;
	}

	hr = m_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
		return hr;

	m_pImmediateContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);

	return hr;
}

void GraphicClass::Update()
{
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);
	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	m_pImmediateContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);
}

void GraphicClass::Render()
{
	m_pImmediateContext->Draw(3, 0);
	//m_pImmediateContext->DrawIndexed(36, 0, 0);

	m_pSwapChain->Present(0, 0);
}

void GraphicClass::SetIAVertexBuffer(ID3D11Buffer* vertexBuffer, UINT stride, UINT offset)
{
	m_pImmediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
}

void GraphicClass::SetIAIndexBuffer()
{
	HRESULT hr = S_OK;

	WORD indices[] =
	{
		3,1,0,
		2,1,3,

		0,5,4,
		1,5,0,

		3,4,7,
		0,4,3,

		1,6,5,
		2,6,1,

		2,7,6,
		3,7,2,

		6,4,5,
		7,4,6,
	};
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 36;        // 36 vertices needed for 12 triangles in a triangle list
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indices;
	hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pIndexBuffer);
	if (FAILED(hr))
		return;

	// Set index buffer
	m_pImmediateContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// Set primitive topology
	//g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
}
