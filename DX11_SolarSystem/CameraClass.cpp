#include "CameraClass.h"
#include "SystemInputClass.h"

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

void CameraClass::SetFixedViewPoint(std::vector<XMMATRIX> world)
{
	int index = 0;

	index = (inputKey.key - DIK_F1)+1;

	if (index < 1)
		return;
	if (index > 8)
		return;

	if (inputKey.isKeyup)
	{
		Eye = originEye;
		At = originAt;
		SetCameraPosition();
	}
	else
	{
		At = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), world[index]);
		Eye = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 3.0f, 0.0f), world[index]);
		SetCameraPosition();
	}
}

void CameraClass::SetCameraPosition()
{
	coordinateConstantBuffer.mView = XMMatrixLookAtLH(Eye, At, Up);
}

XMVECTOR CameraClass::GetCameraEye()
{
	return Eye;
}

void CameraClass::Input(SystemInputClass* inputClass)
{
	static int currentMouseX = 0;
	static int currentMouseY = 0;

	static int prevMouseX = 0;
	static int prevMouseY = 0;

	inputClass->GetMouseLocation(currentMouseX, currentMouseY);	// 해당 값을 통해 view를 rotation 수행하여 시점 변환

	// ... 시점 rotation


	inputClass->GetFunctionKeyPressed(inputKey.key, inputKey.isKeyup);

	prevMouseX = currentMouseX;
	prevMouseY = currentMouseY;
}

void CameraClass::Update(std::vector<XMMATRIX> world)
{
	SetFixedViewPoint(world);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
}

void CameraClass::Shutdown()
{
	if (m_pConstantBuffer)	m_pConstantBuffer->Release();
}
