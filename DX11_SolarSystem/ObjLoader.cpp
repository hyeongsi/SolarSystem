#include "ObjLoader.h"
#include <fstream>

using namespace std;

bool ObjLoader::ReadFileCounts(const char* fileName, int& vertexCount, 
	int& textureCount, int& normalCount, int& faceCount)
{
	ifstream fin;
	char input;

	int tempVertexCount = 0;
	int tempTextureCount = 0;
	int tempNormalCount = 0;
	int tempFaceCount = 0;

	fin.open(fileName);

	if (fin.fail())
	{
		return false;
	}

	fin.get(input);
	while (!fin.eof())
	{
		switch (input)
		{
		case 'v':
			fin.get(input);
			if (input == ' ')		// ���ؽ� ��ǥ
				tempVertexCount++;
			else if (input == 't')	// �ؽ�ó ��ǥ
				tempTextureCount++;
			else if (input == 'n')	// ���� ��ǥ
				tempNormalCount++;
			break;
		case 'f':
			tempFaceCount++;
			break;
		}

		while (input != '\n')
			fin.get(input);

		fin.get(input);
	}

	vertexCount = tempVertexCount;
	textureCount = tempTextureCount;
	normalCount = tempNormalCount;
	faceCount = tempFaceCount;

	fin.close();

	return true;
}

bool ObjLoader::LoadObjVertexData(const char* fileName, XMFLOAT3 * vertexPosition, XMFLOAT2* vertexTexture, XMFLOAT3* vertexNormal, FaceType* faceType)
{
	if (vertexPosition == nullptr)
		return false;
	if (vertexTexture == nullptr)
		return false;
	if (vertexNormal == nullptr)
		return false;
	if (faceType == nullptr)
		return false;

	ifstream fin;
	char input, input2;
	int vertexIndex, textureIndex, normalIndex, faceIndex;

	fin.open(fileName);

	if (fin.fail())
		return false;

	fin.get(input);
	while (!fin.eof())
	{
		switch (input)
		{
		case 'v':
			fin.get(input);
			if (input == ' ')		// ���ؽ� ��ǥ
			{
				fin >> vertexPosition[vertexIndex].x >> vertexPosition[vertexIndex].y >> vertexPosition[vertexIndex].z;
				vertexPosition[vertexIndex].z = vertexPosition[vertexIndex].z * -1.0f;		// ������ ��ǥ��(obj) -> �޼� ��ǥ�� ��ȯ(DX11)

				vertexIndex++;
			}
			else if (input == 't')	// �ؽ�ó ��ǥ
			{
				fin >> vertexTexture[textureIndex].x >> vertexTexture[textureIndex].y;
				vertexTexture[textureIndex].y = 1.0f - vertexTexture[textureIndex].y;		// ������ ��ǥ��(obj) -> �޼� ��ǥ�� ��ȯ(DX11)

				textureIndex++;
			}
			else if (input == 'n')	// ���� ��ǥ
			{
				fin >> vertexNormal[normalIndex].x >> vertexNormal[normalIndex].y >> vertexNormal[normalIndex].z;
				vertexNormal[normalIndex].z = vertexNormal[normalIndex].z * -1.0f;		// ������ ��ǥ��(obj) -> �޼� ��ǥ�� ��ȯ(DX11)

				normalIndex++;
			}
			break;
		case 'f':
			fin.get(input);
			if (input == ' ')
			{
				fin >> faceType[faceIndex].vIndex3 >> input2 >> faceType[faceIndex].tIndex3 >> input2 >> faceType[faceIndex].nIndex3;
				fin >> faceType[faceIndex].vIndex2 >> input2 >> faceType[faceIndex].tIndex2 >> input2 >> faceType[faceIndex].nIndex2;
				fin >> faceType[faceIndex].vIndex1 >> input2 >> faceType[faceIndex].tIndex1 >> input2 >> faceType[faceIndex].nIndex1;

				faceIndex++;
			}
			break;
		}

		while (input != '\n')
			fin.get(input);

		fin.get(input);
	}

	return false;
}
