#pragma once
#include <D3D11.h>
#include <D3DX11.h>
#include <DirectXMath.h>

const int SOLAR_SYSTEM_SIZE = 9;	// 태양,수금지화목토천해
const float PI = 3.14f;

using namespace DirectX;

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};

struct LightBufferType
{
	LightBufferType()
	{
		ZeroMemory(this, sizeof(LightBufferType));
	}

	XMFLOAT3 dir;
	float pad1;
	XMFLOAT3 pos;
	float range;
	XMFLOAT3 att;
	float pad2;
	XMFLOAT4 ambient;	// 주변
	XMFLOAT4 diffuse;	// 확산
};

struct VertexType
{
	XMFLOAT3 pos;
	XMFLOAT2 texture;
	XMFLOAT3 normal;
};

typedef struct
{
	int vIndex1, vIndex2, vIndex3;
	int tIndex1, tIndex2, tIndex3;
	int nIndex1, nIndex2, nIndex3;
}FaceType;

enum class PixelShaderNumber
{
	lightPixelShader = 0,
	normalPixelShader = 1,
};