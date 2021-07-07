#include "ObjectClass.h"

VertexType* ObjectClass::GetVertices()
{
	return m_vertices;
}

ID3D11Buffer* ObjectClass::GetVertexBuffer()
{
	return m_pVertexBuffer;
}

UINT ObjectClass::GetStride()
{
	return stride;
}

UINT ObjectClass::GetOffset()
{
	return offset;
}

//void ObjectClass::SetVertexPosition(XMFLOAT3* vertexPosition)
//{
//	int arrSize = sizeof(m_vertices) / sizeof(m_vertices[0]);
//
//	for (int i = 0; i < arrSize; i++)
//	{
//		m_vertices[i].pos = vertexPosition[i];
//	}
//}

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
	/*VertexType vertices[] =
	{
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
	};*/


	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VertexType) * (sizeof(m_vertices) / sizeof(m_vertices[0]));
	//bd.ByteWidth = sizeof(VertexType) * 8;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = m_vertices;
	//InitData.pSysMem = vertices;
	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);

	return hr;
}

ObjectClass::~ObjectClass()
{
	delete[] m_vertices;
	m_vertices = nullptr;

	if (m_pVertexBuffer)	m_pVertexBuffer->Release();
}
