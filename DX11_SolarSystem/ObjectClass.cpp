#include "ObjectClass.h"
#include "CameraClass.h"

VertexType* ObjectClass::GetVertices()
{
	return m_vertices;
}

ID3D11Buffer* ObjectClass::GetVertexBuffer()
{
	return m_pVertexBuffer;
}

ID3D11Buffer* ObjectClass::GetIndexBuffer()
{
	return m_pIndexBuffer;
}

void ObjectClass::SetVertexCount(int count)
{
	vertexCount = count;
}

int ObjectClass::GetIndexcount()
{
	return indexCount;
}

UINT ObjectClass::GetStride()
{
	return stride;
}

UINT ObjectClass::GetOffset()
{
	return offset;
}

void ObjectClass::SetVertexPosition(XMFLOAT3* vertexPosition, int vertexSize)
{
	XMFLOAT3 test;
	for (int i = 0; i < vertexSize; i++)
	{
		m_vertices[i].pos = vertexPosition[i];
	}
}

void ObjectClass::SetIndexPosition(FaceType* indexPosition, int indexSize)
{
	delete[] m_indices;
	m_indices = nullptr;

	m_indices = new WORD[indexSize * 3];

	int indexCount = 0;
	for (int i = 0; i < indexSize; i++)
	{
		m_indices[indexCount] = indexPosition[i].vIndex1;
		indexCount++;
		m_indices[indexCount] = indexPosition[i].vIndex2;
		indexCount++;
		m_indices[indexCount] = indexPosition[i].vIndex3;
		indexCount++;
	}
}

void ObjectClass::DynamicAllocationVertices(const int size)
{
	delete[] m_vertices;
	m_vertices = nullptr;

	if (size <= 0)
		return;

	m_vertices = new VertexType[size];
}

HRESULT ObjectClass::CreateVertexBuffer(ID3D11Device* pd3dDevice)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VertexType) * vertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = m_vertices;
	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);

	return hr;
}

HRESULT ObjectClass::CreateIndexBuffer(ID3D11Device* pd3dDevice, int byteWidth)
{
	HRESULT hr = S_OK;

	indexCount = byteWidth * 3;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * indexCount;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = m_indices;
	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &m_pIndexBuffer);
	if (FAILED(hr))
		return hr;

	return hr;
}

void ObjectClass::Update(ID3D11DeviceContext* m_pImmediateContext, CameraClass* cameraClass, float deltaTime)
{
	mWorld = XMMatrixRotationY(deltaTime * 20);

	ConstantBuffer constantBufferData;
	constantBufferData.mWorld = XMMatrixTranspose(mWorld);
	constantBufferData.mView = XMMatrixTranspose(cameraClass->GetCoordinateConstantBuffer()->mView);
	constantBufferData.mProjection = XMMatrixTranspose(cameraClass->GetCoordinateConstantBuffer()->mProjection);

	m_pImmediateContext->UpdateSubresource(cameraClass->GetConstantBuffer(), 0, NULL, &constantBufferData, 0, 0);
}

void ObjectClass::Render(ID3D11DeviceContext* m_pImmediateContext, int size)
{
	m_pImmediateContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pImmediateContext->DrawIndexed(size, 0,0);
}

ObjectClass::ObjectClass()
{
	mWorld = XMMatrixIdentity();
}

ObjectClass::~ObjectClass()
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

	if (m_pIndexBuffer)	m_pIndexBuffer->Release();
	if (m_pVertexBuffer)	m_pVertexBuffer->Release();
}
