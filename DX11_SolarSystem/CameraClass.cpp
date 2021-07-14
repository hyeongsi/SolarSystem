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

void CameraClass::Update(std::vector<XMMATRIX> world, std::vector<float> scale)
{
	if (GetAsyncKeyState(VK_F1))
	{
		XMVector4Transform(At, world[0]);
		world[0] *= XMMatrixTranslation(0, scale[0], -scale[0]);
		XMVector4Transform(Eye, world[0]);
		SetCameraPosition();
	}
	else if (GetAsyncKeyState(VK_F2))
	{
		XMVector4Transform(At, world[1]);
		XMVector4Transform(Eye, world[1]);
		Eye = XMVectorSet(XMVectorGetX(Eye), XMVectorGetY(Eye) + scale[1], XMVectorGetZ(Eye) - scale[1], XMVectorGetW(Eye));
		SetCameraPosition();
	}
	else if (GetAsyncKeyState(VK_F3))
	{
		XMVector4Transform(At, world[2]);
		XMVector4Transform(Eye, world[2]);
		Eye = XMVectorSet(XMVectorGetX(Eye), XMVectorGetY(Eye) + scale[2], XMVectorGetZ(Eye) - scale[2], XMVectorGetW(Eye));
		SetCameraPosition();
	}
	else if (GetAsyncKeyState(VK_F4))
	{
		XMVector4Transform(At, world[3]);
		XMVector4Transform(Eye, world[3]);
		Eye = XMVectorSet(XMVectorGetX(Eye), XMVectorGetY(Eye) + scale[3], XMVectorGetZ(Eye) - scale[3], XMVectorGetW(Eye));
		SetCameraPosition();
	}
	else if (GetAsyncKeyState(VK_F5))
	{
		XMVector4Transform(At, world[4]);
		XMVector4Transform(Eye, world[4]);
		Eye = XMVectorSet(XMVectorGetX(Eye), XMVectorGetY(Eye) + scale[4], XMVectorGetZ(Eye) - scale[4], XMVectorGetW(Eye));
		SetCameraPosition();
	}
	else if (GetAsyncKeyState(VK_F6))
	{
		XMVector4Transform(At, world[5]);
		XMVector4Transform(Eye, world[5]);
		Eye = XMVectorSet(XMVectorGetX(Eye), XMVectorGetY(Eye) + scale[5], XMVectorGetZ(Eye) - scale[5], XMVectorGetW(Eye));
		SetCameraPosition();
	}
	else if (GetAsyncKeyState(VK_F7))
	{
		XMVector4Transform(At, world[6]);
		XMVector4Transform(Eye, world[6]);
		Eye = XMVectorSet(XMVectorGetX(Eye), XMVectorGetY(Eye) + scale[6], XMVectorGetZ(Eye) - scale[6], XMVectorGetW(Eye));
		SetCameraPosition();
	}
	else if (GetAsyncKeyState(VK_F8))
	{
		XMVector4Transform(At, world[7]);
		XMVector4Transform(Eye, world[7]);
		Eye = XMVectorSet(XMVectorGetX(Eye), XMVectorGetY(Eye) + scale[7], XMVectorGetZ(Eye) - scale[7], XMVectorGetW(Eye));
		SetCameraPosition();
	}
	else
	{
		Eye = originEye;
		At = originAt;
		SetCameraPosition();
	}

	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
}

void CameraClass::Shutdown()
{
	if (m_pConstantBuffer)	m_pConstantBuffer->Release();
}
