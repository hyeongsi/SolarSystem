#pragma once
#include <D3D11.h>
#include <xnamath.h>
#include <vector>
#include "CameraClass.h"
#include "vertexNbufferStructResource.h"

class ObjectClass
{
private:
    VertexType* m_vertices = NULL;
    ID3D11Buffer* m_pVertexBuffer = NULL;
    WORD* m_indices = NULL;
    ID3D11Buffer* m_pIndexBuffer = NULL;
    std::vector<XMMATRIX> mWorld;
    std::vector<ConstantBuffer> constantBufferData;

    int vertexTypeCount = 0;
    int indexCount = 0;
    UINT stride = sizeof(VertexType);
    UINT offset = 0;
public:
    VertexType* GetVertices();
    WORD* GetIndices();
    ID3D11Buffer* GetVertexBuffer();
    ID3D11Buffer* GetIndexBuffer();
    int GetIndexcount();
    UINT GetStride();
    UINT GetOffset();

    void DynamicAllocationVertices(const int size);
    void DynamicAllocationIndices(const int size);
    HRESULT CreateVertexBuffer(ID3D11Device* pd3dDevice);
    HRESULT CreateIndexBuffer(ID3D11Device* pd3dDevice);

    void Update(ID3D11DeviceContext* m_pImmediateContext, float deltaTime);
    void Render(ID3D11DeviceContext* m_pImmediateContext, CameraClass* cameraClass, std::vector<ID3D11ShaderResourceView*> shaderResourceView);

    ObjectClass();
    ~ObjectClass();
};

