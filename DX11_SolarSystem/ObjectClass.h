#pragma once
#include <D3D11.h>
#include <xnamath.h>

struct VertexType
{
    XMFLOAT3 pos;
    //XMFLOAT4 color;
};

//struct FaceType
//{
//    XMFLOAT3 pos;
//    XMFLOAT2 texture;
//    XMFLOAT3 normal;
//};

class ObjectClass
{
private:
    VertexType* m_vertices = NULL;
    ID3D11Buffer* m_pVertexBuffer = NULL;

    UINT stride = sizeof(VertexType);
    UINT offset = 0;
public:
    VertexType* GetVertices();
    ID3D11Buffer* GetVertexBuffer();
    UINT GetStride();
    UINT GetOffset();

    //void SetVertexPosition(XMFLOAT3* vertexPosition);

    void DynamicAllocationVertices(const int size);
    HRESULT CreateVertexBuffer(ID3D11Device* pd3dDevice);

    ~ObjectClass();
};

