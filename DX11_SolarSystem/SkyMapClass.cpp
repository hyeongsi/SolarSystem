#include "SkyMapClass.h"
#include "CameraClass.h"
#include "GraphicClass.h"
#include <d3dcompiler.h>

HRESULT SkyMapClass::Init(ID3D11Device * pd3dDevice, ID3D11DeviceContext* immediateContext)
{
	HRESULT hr;

	m_pd3dDevice = pd3dDevice;
	m_pImmediateContext = immediateContext;

	D3D11_TEXTURE2D_DESC dstex;
	dstex.Width = 256;
	dstex.Height = 256;
	dstex.MipLevels = 9;
	dstex.ArraySize = 6;
	dstex.SampleDesc.Quality = 0;
	dstex.SampleDesc.Count = 1;
	dstex.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	dstex.Usage = D3D11_USAGE_DEFAULT;
	dstex.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;	// 렌더타겟 + 쉐이더용
	dstex.CPUAccessFlags = 0;
	dstex.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE;	// 밉맵 자동생성 + 큐브텍스쳐

	hr = m_pd3dDevice->CreateTexture2D(&dstex, NULL, &g_pEnvMap);

	// 6면의 렌더타겟 뷰 (2D 텍스쳐 배열 뷰)를 생성
	D3D11_RENDER_TARGET_VIEW_DESC descRT;
	descRT.Format = dstex.Format;
	descRT.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	descRT.Texture2DArray.ArraySize = 6;
	descRT.Texture2DArray.FirstArraySlice = 0;
	descRT.Texture2DArray.MipSlice = 0;

	hr = m_pd3dDevice->CreateRenderTargetView(g_pEnvMap, &descRT, &g_pEnvMapRTV);

	// 셰이더에서 환경맵에 액세스 할 수 있도록 SRV(shader resource view) 생성
	D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
	descSRV.Format = dstex.Format;
	descSRV.ViewDimension = D3D_SRV_DIMENSION_TEXTURECUBE; // 큐브 텍스쳐 사용
	descSRV.TextureCube.MipLevels = 9;
	descSRV.TextureCube.MostDetailedMip = 0;

	hr = m_pd3dDevice->CreateShaderResourceView(g_pEnvMap, &descSRV, &g_pEnvMapSRV);

	D3D11_TEXTURE2D_DESC dstex;
	dstex.Width = 256;
	dstex.Height = 256;
	dstex.Format = DXGI_FORMAT_R32_FLOAT;
	dstex.MipLevels = 1;
	dstex.ArraySize = 6;
	dstex.SampleDesc.Count = 1;
	dstex.SampleDesc.Quality = 0;
	dstex.Usage = D3D11_USAGE_DEFAULT;
	dstex.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dstex.CPUAccessFlags = 0;
	dstex.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	hr = m_pd3dDevice->CreateTexture2D(&dstex, NULL, &g_pEnvMapDepth);

	// 큐브 텍스처의 깊이/스텐실 뷰 생성
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = dstex.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	descDSV.Texture2DArray.FirstArraySlice = 0;
	descDSV.Texture2DArray.ArraySize = 6;
	descDSV.Texture2DArray.MipSlice = 0;

	hr = m_pd3dDevice->CreateDepthStencilView(g_pEnvMapDepth, &descDSV, &g_pEnvMapDSV);

	g_viewPortEnvMap.TopLeftX = 0.0f;
	g_viewPortEnvMap.TopLeftY = 0.0f;
	g_viewPortEnvMap.Width = dstex.Width;
	g_viewPortEnvMap.Height = dstex.Height;
	g_viewPortEnvMap.MinDepth = 0.0f;
	g_viewPortEnvMap.MaxDepth = 1.0f;

	ID3DBlob* pVSBlob = NULL;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	wchar_t szfileName[20] = L"temp.fx";
	hr = D3DX11CompileFromFileW(szfileName, NULL, NULL, "SKYMAP_VS", "vs_4_0", dwShaderFlags, 0, NULL, &pVSBlob, NULL, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			"fx error, (SKYMAP_VS)", "Error", MB_OK);
		return hr;
	}

	hr = m_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &m_pVertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}

	ID3DBlob* pPSBlob = NULL;
	hr = D3DX11CompileFromFileW(szfileName, NULL, NULL, "SKYMAP_PS", "ps_4_0", dwShaderFlags, 0, NULL, &pPSBlob, NULL, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			"fx error, (SKYMAP_PS)", "Error", MB_OK);
		return hr;
	}

	hr = m_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
		return hr;

	/*D3D11_RASTERIZER_DESC cmdesc;

	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_BACK;
	cmdesc.FrontCounterClockwise = true;
	hr = m_pd3dDevice->CreateRasterizerState(&cmdesc, &m_pCCWcullMode);

	cmdesc.FrontCounterClockwise = false;

	hr = m_pd3dDevice->CreateRasterizerState(&cmdesc, &m_pCWcullMode);

	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	m_pd3dDevice->CreateDepthStencilState(&dssDesc, &m_pDSLessEqual);*/

	return hr;
}

VertexType* SkyMapClass::GetVertices()
{
	return m_vertices;
}

WORD* SkyMapClass::GetIndices()
{
	return m_indices;
}

HRESULT SkyMapClass::CreateVertexBuffer()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VertexType);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = m_vertices;
	hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);

	return hr;
}

HRESULT SkyMapClass::CreateIndexBuffer()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * indexCount;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = m_indices;
	hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pIndexBuffer);
	if (FAILED(hr))
		return hr;

	return hr;
}

void SkyMapClass::DynamicAllocationVertices(const int size)
{
	delete[] m_vertices;
	m_vertices = nullptr;

	if (size <= 0)
		return;

	m_vertices = new VertexType[size];
	vertexTypeCount = size;
}

void SkyMapClass::DynamicAllocationIndices(const int size)
{
	delete[] m_indices;
	m_indices = nullptr;

	if (size <= 0)
		return;

	m_indices = new WORD[size];
	indexCount = size;
}

void SkyMapClass::Render(CameraClass* cameraClass, GraphicClass* graphicClass)
{
	//world = XMMatrixScaling(500.0f, 500.0f, 500.0f) * XMMatrixTranslation(-100.0f, 0.0f, 0.0f);	// 임시로 지정

	//m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	//m_pImmediateContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	//m_pImmediateContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//constantBufferData.mWorld = XMMatrixTranspose(world);
	//constantBufferData.mView = XMMatrixTranspose(cameraClass->GetCoordinateConstantBuffer()->mView);
	//constantBufferData.mProjection = XMMatrixTranspose(cameraClass->GetCoordinateConstantBuffer()->mProjection);

	//graphicClass->SetVertexShader();
	//m_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);

	//m_pImmediateContext->UpdateSubresource(cameraClass->GetConstantBuffer(), 0, NULL, &constantBufferData, 0, 0);
	//m_pImmediateContext->PSSetShaderResources(0, 1, &m_pShaderResourceView);

	//m_pImmediateContext->OMSetDepthStencilState(m_pDSLessEqual, 0);
	//m_pImmediateContext->RSSetState(RSCullNone);

	//m_pImmediateContext->DrawIndexed(indexCount, 0, 0);
}

SkyMapClass::~SkyMapClass()
{
	if (m_indices)
	{
		delete[] m_indices;
		m_indices = nullptr;
	}

	if (m_vertices)
	{
		delete[] m_vertices;
		m_vertices = nullptr;
	}

	if (m_pIndexBuffer)		m_pIndexBuffer->Release();
	if (m_pVertexBuffer)	m_pVertexBuffer->Release();
}
