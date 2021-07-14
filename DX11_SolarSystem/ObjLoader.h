#pragma once
#include <D3D11.h>
#include <DirectXMath.h>
#include <vector>
#include "vertexNbufferStructResource.h"

using namespace std;

class ObjLoader
{
private:
	int vertexCount = 0;
	int textureCount = 0;
	int normalCount = 0;
	int faceCount = 0;

	vector<XMFLOAT3> vertices;
	vector<XMFLOAT2> texcoords;
	vector<XMFLOAT3> normals;
	vector<FaceType> faces;

public:
	int GetVertexCount();
	int GetTextureCount();
	int GetNormalCount();
	int GetFaceCount();

	void Reset();
	bool ReadFileCounts(const char* fileName);
	bool LoadObjVertexData(const char* fileName, VertexType* vertexType, WORD* indices);

};

