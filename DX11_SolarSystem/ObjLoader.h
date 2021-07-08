#pragma once
#include <D3D11.h>
#include <xnamath.h>
#include "ObjectClass.h"

class ObjLoader
{
private:


public:
	bool ReadFileCounts(const char* fileName, int& vertexCount, int& textureCount, int& normalCount, int& faceCount);
	bool LoadObjVertexData(const char* fileName, XMFLOAT3* vertexPosition, XMFLOAT2* vertexTexture, XMFLOAT3* vertexNormal, FaceType* faceType);

};

