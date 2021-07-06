#pragma once
#include <D3D11.h>
#include <xnamath.h>

typedef struct
{
	int vIndex1, vIndex2, vIndex3;
	int tIndex1, tIndex2, tIndex3;
	int nIndex1, nIndex2, nIndex3;
}FaceType;

class ObjLoader
{
private:


public:
	bool ReadFileCounts(const char* fileName, int& vertexCount, int& textureCount, int& normalCount, int& faceCount);
	bool LoadObjVertexData(const char* fileName, XMFLOAT3* vertexPosition, XMFLOAT2* vertexTexture, XMFLOAT3* vertexNormal, FaceType* faceType);

};

