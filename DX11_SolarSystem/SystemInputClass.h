#pragma once
#include <dinput.h>

class SystemInputClass
{
private:
	// direct �Է�, Ű����, ���콺 ��ġ �������̽�
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;

	bool m_keys[256];	 // Ű �Է� ���� ����
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

	void Shutdown();
};

