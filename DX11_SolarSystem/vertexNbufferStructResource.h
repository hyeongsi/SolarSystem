#pragma once
#include <D3D11.h>
#include <D3DX11.h>
#include <xnamath.h>

const int SOLAR_SYSTEM_SIZE = 9;	// 태양,수금지화목토천해

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMFLOAT4 mMeshColor;
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