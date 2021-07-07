#include "CameraClass.h"

HRESULT CameraClass::Init(const float width, const float height, 
	ID3D11Device* m_pd3dDevice, ID3D11DeviceContext* m_pImmediateContext)
{
	HRESULT hr = S_OK;

	this->m_pd3dDevice = m_pd3dDevice;
	this->m_pImmediateContext = m_pImmediateContext;

	coordinateConstantBuffer.mWorld = XMMatrixIdentity();
	coordinateConstantBuffer.mView = XMMatrixLookAtLH(Eye, At, Up);
	coordinateConstantBuffer.mProjection = XMMatrixPerspectiveFovLH(XM_PIDIV2, width / (FLOAT)height, 0.01f, 100.0f);

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(ConstantBuffer);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	hr = m_pd3dDevice->CreateBuffer(&bufferDesc, NULL, &m_pConstantBuffer);

	return hr;
}

ConstantBuffer* CameraClass::GetCoordinateConstantBuffer()
{
	return &coordinateConstantBuffer;
}

void CameraClass::Update()
{
	ConstantBuffer constantBufferData;
	constantBufferData.mWorld = XMMatrixTranspose(coordinateConstantBuffer.mWorld);
	constantBufferData.mView = XMMatrixTranspose(coordinateConstantBuffer.mView);
	constantBufferData.mProjection = XMMatrixTranspose(coordinateConstantBuffer.mProjection);

	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &constantBufferData, 0, 0);

	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
}

void CameraClass::Shutdown()
{
	if (m_pConstantBuffer)	m_pConstantBuffer->Release();
}
