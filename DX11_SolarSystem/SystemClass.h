#pragma once
#include "framework.h"
#include "resource.h"
#include <D3D11.h>

class GraphicClass;
class CameraClass;
class ObjLoader;
class ObjectClass;
class GameTimer;
class LightClass;
class SkyMapClass;
class SystemInputClass;
class SystemClass
{
private:
	HWND m_hWnd;
	HINSTANCE m_hInst;
	const char* m_className = "SolarSystemClass";
	GameTimer* gameTimer = NULL;
	GraphicClass* graphicClass = NULL;
	CameraClass* cameraClass = NULL;
	ObjLoader* objLoader = NULL;
	ObjectClass* objectClass = NULL;
	LightClass* lightClass = NULL;
	SkyMapClass* skyMapClass = NULL;
	SystemInputClass* inputclass = NULL;

	const char* loadFileName = "Sphere.obj";

public:
	HRESULT InitWindow(int& nCmdShow);
	void Run();
	void Shutdown();

	HWND* GetHwnd();

};

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);