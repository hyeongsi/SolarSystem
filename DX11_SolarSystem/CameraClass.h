#pragma once
#include <D3D11.h>
#include <D3DX11.h>
#include <DirectXMath.h>
#include <vector>
#include "vertexNbufferStructResource.h"

typedef struct InputKey
{
	unsigned int key = 0;
	bool isKeyup = false;
};

typedef struct InputMouseLocation
{
	int diffMouseX = 0;
	int diffMouseY = 0;
};

class SystemInputClass;
class CameraClass
{
private:
	ConstantBuffer coordinateConstantBuffer;

	XMVECTOR Eye = XMVectorSet(0.0f, 50.0f, -150.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMVECTOR originEye = XMVectorSet(0.0f, 50.0f, -150.0f, 0.0f);
	XMVECTOR originAt = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	ID3D11Device* m_pd3dDevice = NULL;
	ID3D11DeviceContext* m_pImmediateContext = NULL;
	ID3D11Buffer* m_pConstantBuffer = NULL;

	InputKey inputKey[2];	// function, wsad
	InputMouseLocation inputMouseLocation;
public:
	HRESULT Init(const float width, const float height, 
		ID3D11Device* m_pd3dDevice, ID3D11DeviceContext* m_pImmediateContext);

	ConstantBuffer* GetCoordinateConstantBuffer();
	ID3D11Buffer* GetConstantBuffer();
	XMVECTOR GetCameraEye();
	void SetCameraPosition();
	void SetFixedViewPoint(std::vector<XMMATRIX> world);
	void HandleMouseMovement(float deltaTime);
	void HandleKeyboardMovement(float deltaTime);

	void Update(SystemInputClass* inputClass, std::vector<XMMATRIX> world, float deltaTime);
	void Shutdown();
};

