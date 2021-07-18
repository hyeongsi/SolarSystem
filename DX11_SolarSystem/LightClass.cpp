#include "LightClass.h"

HRESULT LightClass::Init(ID3D11Device* d3dDevice, ID3D11DeviceContext* immediateContext)
{
	HRESULT hr = S_OK;

	m_pd3dDevice = d3dDevice;
	m_pImmediateContext = immediateContext;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(LightBufferType);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	hr = m_pd3dDevice->CreateBuffer(&bufferDesc, NULL, &m_pLightBuffer);

	lightBufferType.diffuseColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	lightBufferType.lightPosition = { 0.0f, 0.0f, 0.0f };
	lightBufferType.padding = 3.0f;

	return hr;
}

ID3D11Buffer* LightClass::GetLightConstantBuffer()
{
	return m_pLightBuffer;
}

LightBufferType LightClass::GetLightBufferType()
{
	return lightBufferType;
}

void LightClass::Update()
{
	m_pImmediateContext->UpdateSubresource(m_pLightBuffer, 0, NULL, &lightBufferType, 0, 0);

	m_pImmediateContext->VSSetConstantBuffers(1, 1, &m_pLightBuffer);
	m_pImmediateContext->PSSetConstantBuffers(1, 1, &m_pLightBuffer);
}

void LightClass::Shutdown()
{
	if (m_pLightBuffer)	m_pLightBuffer->Release();
}