#pragma once
#include <dinput.h>

class SystemInputClass
{
private:
	// direct 입력, 키보드, 마우스 장치 인터페이스
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;

public:
	SystemInputClass();
	HRESULT Init(HINSTANCE hinstance, HWND hwnd, const int screenWidth, const int screenHeight);
	bool Frame();

	bool ReadKeyBoard();
	bool ReadMouse();
	void ProcessInput();
	bool IsEscapePressed();
	void GetMouseLocation(int& mouseX, int& mouseY);

	void Shutdown();
};

