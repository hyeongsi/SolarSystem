#pragma once
#include <dinput.h>

class SystemInputClass
{
private:
	// direct 입력, 키보드, 마우스 장치 인터페이스
	IDirectInput8* m_directInput = NULL;
	IDirectInputDevice8* m_keyboard = NULL;
	IDirectInputDevice8* m_mouse = NULL;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth = 0, m_screenHeight = 0;
	int m_mouseX = 0, m_mouseY = 0;

	bool m_keys[256];	 // 키 입력 유무 저장
	unsigned int moveKey[4];

public:
	SystemInputClass();
	HRESULT Init(HINSTANCE hinstance, HWND hwnd, const int screenWidth, const int screenHeight);
	bool Frame();

	void KeyDown(unsigned int input);
	void KeyUp(unsigned int input);

	bool ReadKeyBoard();
	bool ReadMouse();
	void ProcessInput();
	bool IsEscapePressed();
	void GetMouseLocation(int& mouseX, int& mouseY);
	void GetFunctionKeyPressed(unsigned int& key, bool& isKeyUp);
	void GetMoveKeyPressed(unsigned int& key);

	DIMOUSESTATE* GetMouseState();

	void Shutdown();
};

