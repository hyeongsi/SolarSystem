#include "CameraClass.h"

HRESULT CameraClass::Init(const float width, const float height, 
	ID3D11Device* m_pd3dDevice, ID3D11DeviceContext* m_pImmediateContext)
{
	HRESULT hr = S_OK;

	this->m_pd3dDevice = m_pd3dDevice;
	this->m_pImmediateContext = m_pImmediateContext;

	coordinateConstantBuffer.mWorld = XMMatrixIdentity();
	coordinateConstantBuffer.mView = XMMatrixLookAtLH(Eye, At, Up);
	coordinateConstantBuffer.mProjection = XMMatrixPerspectiveFovLH(XM_PIDIV2, width / (FLOAT)height, 0.01f, 1000.0f);

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

ID3D11Buffer* CameraClass::GetConstantBuffer()
{
	return m_pConstantBuffer;
}

void CameraClass::SetCameraPosition()
{
	coordinateConstantBuffer.mView = XMMatrixLookAtLH(Eye, At, Up);
}

void CameraClass::Update(std::vector<XMMATRIX> world)
{
	static bool inputFunctionKey = false;

	if (inputFunctionKey)
	{
		Eye = originEye;
		At = originAt;
		SetCameraPosition();
		inputFunctionKey = false;
	}

	if (GetAsyncKeyState(VK_F1))
	{
		At = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), world[1]);
		Eye = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 3.0f, 0.0f), world[1]);
		SetCameraPosition();
		inputFunctionKey = true;
	}
	else if (GetAsyncKeyState(VK_F2))	
	{
		At = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), world[2]);
		Eye = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 3.0f, 0.0f), world[2]);
		SetCameraPosition();
		inputFunctionKey = true;
	}
	else if (GetAsyncKeyState(VK_F3))
	{
		At = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), world[3]);
		Eye = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 3.0f, 0.0f), world[3]);
		SetCameraPosition();
		inputFunctionKey = true;
	}
	else if (GetAsyncKeyState(VK_F4))
	{
		At = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), world[4]);
		Eye = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 3.0f, 0.0f), world[4]);
		SetCameraPosition();
		inputFunctionKey = true;
	}
	else if (GetAsyncKeyState(VK_F5))
	{
		At = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), world[5]);
		Eye = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 3.0f, 0.0f), world[5]);
		SetCameraPosition();
		inputFunctionKey = true;
	}
	else if (GetAsyncKeyState(VK_F6))
	{
		At = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), world[6]);
		Eye = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 3.0f, 0.0f), world[6]);
		SetCameraPosition();
		inputFunctionKey = true;
	}
	else if (GetAsyncKeyState(VK_F7))
	{
		At = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), world[7]);
		Eye = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 3.0f, 0.0f), world[7]);
		SetCameraPosition();
		inputFunctionKey = true;
	}
	else if (GetAsyncKeyState(VK_F8))
	{
		At = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), world[8]);
		Eye = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 3.0f, 0.0f), world[8]);
		SetCameraPosition();
		inputFunctionKey = true;
	}

	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
}

void CameraClass::Shutdown()
{
	if (m_pConstantBuffer)	m_pConstantBuffer->Release();
}
