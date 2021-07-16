#pragma once
#include "vertexNbufferStructResource.h"

class CameraClass;
class GraphicClass;
class SkyMapClass
{
private:
	ID3D11Device* m_pd3dDevice = NULL;
	ID3D11DeviceContext* m_pImmediateContext = NULL;

	VertexType* m_vertices = NULL;
	ID3D11Buffer* m_pVertexBuffer = NULL;
	WORD* m_indices = NULL;
	ID3D11Buffer* m_pIndexBuffer = NULL;

	ID3D11VertexShader* m_pVertexShader = NULL;
	ID3D11PixelShader* m_pPixelShader = NULL;

	ConstantBuffer constantBufferData;
	XMMATRIX world;

	// »Ø∞Ê∏ ¿ª ¿ß«— ∏Æº“Ω∫ ∫‰
	ID3D11Texture2D* g_pEnvMap = NULL;
	ID3D11RenderTargetView* g_pEnvMapRTV = NULL;
	ID3D11ShaderResourceView* g_pEnvMapSRV = NULL;

	ID3D11Texture2D* g_pEnvMapDepth;
	ID3D11DepthStencilView* g_pEnvMapDSV;

	D3D11_VIEWPORT g_viewPortEnvMap;

	const float scale = 1.0f;

	int vertexTypeCount = 0;
	int indexCount = 0;

	const char* fileName = "Textures/Space.dds";

	UINT stride = sizeof(VertexType);
	UINT offset = 0;

public:
	HRESULT Init(ID3D11Device* pd3dDevice, ID3D11DeviceContext* immediateContext);

	VertexType* GetVertices();
	WORD* GetIndices();

	HRESULT CreateVertexBuffer();
	HRESULT CreateIndexBuffer();
	void Render(CameraClass* cameraClass, GraphicClass* graphicClass);

	void DynamicAllocationVertices(const int size);
	void DynamicAllocationIndices(const int size);

	~SkyMapClass();
};

