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

	ID3D11SamplerState* m_pTextureSampler = NULL;
	ID3D11ShaderResourceView* m_pTextureView;

	ConstantBuffer constantBufferData;
	XMMATRIX world;

	int vertexTypeCount = 0;
	int indexCount = 0;

	const char* fileName = "Textures/Space.dds";

	UINT stride = sizeof(VertexType);
	UINT offset = 0;

public:
	HRESULT Init(ID3D11Device* pd3dDevice, ID3D11DeviceContext* immediateContext);

	VertexType* GetVertices();
	WORD* GetIndices();

	void DynamicAllocationVertices(const int size);
	void DynamicAllocationIndices(const int size);

	HRESULT CreateVertexBuffer();
	HRESULT CreateIndexBuffer();
	void Render(CameraClass* cameraClass, GraphicClass* graphicClass);

	~SkyMapClass();
};

