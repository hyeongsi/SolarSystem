#pragma once
#include "vertexNbufferStructResource.h"

class LightClass
{
private:
	ID3D11Device* m_pd3dDevice = NULL;
	ID3D11DeviceContext* m_pImmediateContext = NULL;
	ID3D11Buffer* m_pLightBuffer = NULL;
	LightBufferType lightBufferType;

public:
	HRESULT Init(ID3D11Device* d3dDevice, ID3D11DeviceContext* immediateContext);
	
	ID3D11Buffer* GetLightConstantBuffer();
	LightBufferType GetLightBufferType();

	void Update();
	void Shutdown();

};

