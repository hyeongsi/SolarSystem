#include "ObjectClass.h"

using namespace std;

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
	for (int i = 0; i < vertexSize; i++)
	{
		m_vertices[i].pos = vertexPosition[i];
	}
}

void ObjectClass::SetVertexNormal(XMFLOAT3* vertexNormal, int normalCount)
{
	for (int i = 0; i < normalCount; i++)
	{
		m_vertices[i].normal = vertexNormal[i];
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

void ObjectClass::Update(ID3D11DeviceContext* m_pImmediateContext,  float deltaTime)
{
	static float accumDeltaTime = 0.0f;
	accumDeltaTime += deltaTime;

	// �� �༺�� ���� : 109.25, 0.383, 0.950, 1, 0.532, 10.97, 9.14, 3.98, 3.87 
	mWorld[0] = XMMatrixScaling(10.0f, 10.0f, 10.0f) * XMMatrixRotationY(accumDeltaTime);
	mWorld[1] = XMMatrixScaling(0.3, 0.3f, 0.3f)  * XMMatrixTranslation(-20.0f, 0.0f, 0.0f) * XMMatrixRotationY(accumDeltaTime);
	mWorld[2] = XMMatrixScaling(0.9f, 0.9f, 0.9f)  * XMMatrixTranslation(-25.0f, 0.0f, 0.0f) * XMMatrixRotationY(accumDeltaTime);
	mWorld[3] = XMMatrixScaling(1.0f, 1.0f, 1.0f)  * XMMatrixTranslation(-30.0f, 0.0f, 0.0f) * XMMatrixRotationY(accumDeltaTime);
	mWorld[4] = XMMatrixScaling(0.5f, 0.5f, 0.5f) * XMMatrixTranslation(-33.0f, 0.0f, 0.0f) * XMMatrixRotationY(accumDeltaTime);
	mWorld[5] = XMMatrixScaling(5.0, 5.0, 5.0f)  * XMMatrixTranslation(-38.0f, 0.0f, 0.0f) * XMMatrixRotationY(accumDeltaTime);
	mWorld[6] = XMMatrixScaling(4.5, 4.5f, 4.5f)  * XMMatrixTranslation(-50.0f, 0.0f, 0.0f) * XMMatrixRotationY(accumDeltaTime);
	mWorld[7] = XMMatrixScaling(4.0f, 4.0f, 4.0f) * XMMatrixTranslation(-60.0f, 0.0f, 0.0f) * XMMatrixRotationY(accumDeltaTime);
	mWorld[8] = XMMatrixScaling(3.7f, 3.7f, 3.7f)  * XMMatrixTranslation(-70.0f, 0.0f, 0.0f) * XMMatrixRotationY(accumDeltaTime);
}

void ObjectClass::Render(ID3D11DeviceContext* m_pImmediateContext, CameraClass* cameraClass, int size)
{
	m_pImmediateContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (int i = 0; i < SOLAR_SYSTEM_SIZE; i++)	// �¾�,������ȭ����õ��
	{
		constantBufferData[i].mWorld = XMMatrixTranspose(mWorld[i]);
		constantBufferData[i].mView = XMMatrixTranspose(cameraClass->GetCoordinateConstantBuffer()->mView);
		constantBufferData[i].mProjection = XMMatrixTranspose(cameraClass->GetCoordinateConstantBuffer()->mProjection);
		constantBufferData[i].vOutputColor = XMFLOAT4(0, 0, 0, 1);

		m_pImmediateContext->UpdateSubresource(cameraClass->GetConstantBuffer(), 0, NULL, &constantBufferData[i], 0, 0);

		m_pImmediateContext->DrawIndexed(size, 0, 0);
	}
}

ObjectClass::ObjectClass()
{
	ConstantBuffer constantBuffer;
	for (int i = 0; i < SOLAR_SYSTEM_SIZE; i++)
	{
		mWorld.emplace_back(XMMatrixIdentity());
		constantBufferData.emplace_back(constantBuffer);
	}
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
