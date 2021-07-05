#pragma once
#include "framework.h"
#include "resource.h"
#include <D3D11.h>

class SystemClass
{
public:
	HRESULT InitWindow(int& nCmdShow);
	void Run();
	void Shutdown();

private:
	HWND m_hWnd = NULL;
	HINSTANCE m_hInst = NULL;
	const char* m_className = "SolarSystemClass";

};

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);