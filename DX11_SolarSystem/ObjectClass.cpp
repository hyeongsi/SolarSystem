#include "ObjectClass.h"

using namespace std;

VertexType* ObjectClass::GetVertices()
{
	return m_vertices;
}

WORD* ObjectClass::GetIndices()
{
	return m_indices;
}

ID3D11Buffer* ObjectClass::GetVertexBuffer()
{
	return m_pVertexBuffer;
}

ID3D11Buffer* ObjectClass::GetIndexBuffer()
{
	return m_pIndexBuffer;
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

void ObjectClass::DynamicAllocationVertices(const int size)
{
	delete[] m_vertices;
	m_vertices = nullptr;

	if (size <= 0)
		return;

	m_vertices = new VertexType[size];
	vertexTypeCount = size;
}

void ObjectClass::DynamicAllocationIndices(const int size)
{
	delete[] m_indices;
	m_indices = nullptr;

	if (size <= 0)
		return;

	m_indices = new WORD[size];
	indexCount = size;
}

HRESULT ObjectClass::CreateVertexBuffer(ID3D11Device* pd3dDevice)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VertexType) * vertexTypeCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = m_vertices;
	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);

	return hr;
}

HRESULT ObjectClass::CreateIndexBuffer(ID3D11Device* pd3dDevice)
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
	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &m_pIndexBuffer);
	if (FAILED(hr))
		return hr;

	return hr;
}

void ObjectClass::Update(ID3D11DeviceContext* m_pImmediateContext,  float deltaTime)
{
	static float accumDeltaTime = 0.0f;
	accumDeltaTime += deltaTime;
	int accumDistance = 0;

	// 각 행성별 비율 : 109.25, 0.383, 0.950, 1, 0.532, 10.97, 9.14, 3.98, 3.87 
	// 태양과의 거리 : 0, 579.1, 1082, 1496, 2279, 7785, 14340, 28710, 44950
	// 공전속도 : 0, 48, 35 ,30, 24, 13, 9.6, 7, 23.5
	// 자전속도 : 1.9, 0.003, 0.0018, 0.4651, 0.2411, 12.6, 9.8, 2.59, 2.68
	// 자전기울기 : 7.25, 0.01, 2.64, 23.44, 25.19, 3.12, 26.73, 82.23, 28.33

	// 위의 데이터 나중에 txt 파일로 빼가지고, 로드해서 사용하도록 만들기
	// 크기 * 자전 * 거리 * 공전

	accumDistance += 20.0f;
	mWorld[0] = XMMatrixScaling(20.0f, 20.0f, 20.0f) * XMMatrixRotationX(7.25f * PI /180.0f) * XMMatrixRotationY(accumDeltaTime * 1.9f);
	accumDistance += 5.0f + 0.383f;
	mWorld[1] = XMMatrixScaling(0.383, 0.383f, 0.383f) * XMMatrixRotationX(0.01f * PI / 180.0f) * XMMatrixRotationY(accumDeltaTime * 0.003f) 
		* XMMatrixTranslation(-accumDistance, 0.0f, 0.0f) * XMMatrixRotationY(accumDeltaTime * 4.8f);
	accumDistance += 5.0f + 0.95f;
	mWorld[2] = XMMatrixScaling(0.95f, 0.95f, 0.95f) * XMMatrixRotationX(2.64f * PI / 180.0f) * XMMatrixRotationY(accumDeltaTime * 0.0018f) 
		* XMMatrixTranslation(-accumDistance, 0.0f, 0.0f) * XMMatrixRotationY(accumDeltaTime * 3.5f);
	accumDistance += 5.0f + 1.0f;
	mWorld[3] = XMMatrixScaling(1.0f, 1.0f, 1.0f) * XMMatrixRotationX(23.44f * PI / 180.0f) * XMMatrixRotationY(accumDeltaTime * 0.4651f) 
		* XMMatrixTranslation(-accumDistance, 0.0f, 0.0f) * XMMatrixRotationY(accumDeltaTime * 3.0f);
	accumDistance += 5.0f + 0.532f;
	mWorld[4] = XMMatrixScaling(0.532f, 0.532f, 0.532f) * XMMatrixRotationX(25.19f * PI / 180.0f) * XMMatrixRotationY(accumDeltaTime * 0.2411f) 
		* XMMatrixTranslation(-accumDistance, 0.0f, 0.0f) * XMMatrixRotationY(accumDeltaTime * 2.4f);
	accumDistance += 50.0f + 10.0f;
	mWorld[5] = XMMatrixScaling(10.97, 10.97, 10.97f) * XMMatrixRotationX(3.12f * PI / 180.0f) * XMMatrixRotationY(accumDeltaTime * 12.6f) 
		* XMMatrixTranslation(-accumDistance, 0.0f, 0.0f) * XMMatrixRotationY(accumDeltaTime * 1.3f);
	accumDistance += 50.0f + 9.14f;
	mWorld[6] = XMMatrixScaling(9.14, 9.14f, 9.14f) * XMMatrixRotationX(26.73f * PI / 180.0f) * XMMatrixRotationY(accumDeltaTime * 9.8f) 
		* XMMatrixTranslation(-accumDistance, 0.0f, 0.0f) * XMMatrixRotationY(accumDeltaTime * 0.96f);
	accumDistance += 50.0f + 3.98f;
	mWorld[7] = XMMatrixScaling(3.98f, 3.98f, 3.98f) * XMMatrixRotationX(82.23f * PI / 180.0f) * XMMatrixRotationY(accumDeltaTime * 2.59f) 
		* XMMatrixTranslation(-accumDistance, 0.0f, 0.0f) * XMMatrixRotationY(accumDeltaTime * 0.7f);
	accumDistance += 50.0f + 3.87f;
	mWorld[8] = XMMatrixScaling(3.87f, 3.87f, 3.87f) * XMMatrixRotationX(28.33f * PI / 180.0f) * XMMatrixRotationY(accumDeltaTime * 2.68f) 
		* XMMatrixTranslation(-accumDistance, 0.0f, 0.0f) * XMMatrixRotationY(accumDeltaTime * 2.35f);
}

void ObjectClass::Render(ID3D11DeviceContext* m_pImmediateContext, CameraClass* cameraClass, vector<ID3D11ShaderResourceView*> shaderResourceView)
{
	m_pImmediateContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (int i = 0; i < SOLAR_SYSTEM_SIZE; i++)	// 태양,수금지화목토천해
	{
		constantBufferData[i].mWorld = XMMatrixTranspose(mWorld[i]);
		constantBufferData[i].mView = XMMatrixTranspose(cameraClass->GetCoordinateConstantBuffer()->mView);
		constantBufferData[i].mProjection = XMMatrixTranspose(cameraClass->GetCoordinateConstantBuffer()->mProjection);

		m_pImmediateContext->UpdateSubresource(cameraClass->GetConstantBuffer(), 0, NULL, &constantBufferData[i], 0, 0);
		m_pImmediateContext->PSSetShaderResources(0, 1, &shaderResourceView[i]);
		m_pImmediateContext->DrawIndexed(indexCount, 0, 0);
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
