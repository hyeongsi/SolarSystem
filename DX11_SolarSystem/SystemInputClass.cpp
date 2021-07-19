#include "SystemInputClass.h"

SystemInputClass::SystemInputClass()
{
	m_directInput = 0;
	m_keyboard = 0;
	m_mouse = 0;

	// Ű �Է� ��Ȱ��ȭ ó��
	for (int i = 0; i < ARRAYSIZE(m_keys); i++)
	{
		m_keys[i] = false;
	}
}

HRESULT SystemInputClass::Init(HINSTANCE hinstance, HWND hwnd, const int screenWidth, const int screenHeight)
{
	HRESULT hr = S_OK;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_mouseX = 0;
	m_mouseY = 0;

	// Direct �Է� �������̽� �ʱ�ȭ, ��ü ����
	hr = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(hr))
		return hr;

	// ������ ���� �Է� ��ü�� ���� Ű���� ��ü ����
	hr = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(hr))
		return hr;

	// ������ ���� ����, Ű���带 ����Ѵٰ� �˷��ִ� �׷��� �ε�
	hr = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
		return hr;

	// Ű���� ���� ���� ����, �ٸ� ���α׷����� ������ ����
	hr = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(hr))
		return hr;

	// Ű���� ���� ���� ���
	m_keyboard->Acquire();

	hr = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(hr))
		return hr;

	hr = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
		return hr;

	// DISCL_NONEXCLUSIVE
	hr = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(hr))
		return hr;

	m_mouse->Acquire();

	return hr;
}

bool SystemInputClass::Frame()
{
	bool result;

	result = ReadKeyBoard();
	if (!result)
		return false;

	result = ReadMouse();
	if (!result)
		return false;

	ProcessInput();

	return true;
}

void SystemInputClass::KeyDown(unsigned int input)
{
	m_keys[input] = true;
}

void SystemInputClass::KeyUp(unsigned int input)
{
	m_keys[input] = false;
}

bool SystemInputClass::ReadKeyBoard()
{
	HRESULT result;

	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// ��Ŀ���� �Ҿ��ų� acquire�� ���� �ʴ� ��Ȳ�̸�
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			m_keyboard->Acquire();
		}
		else
			return false;
	}

	return true;
}

bool SystemInputClass::ReadMouse()
{
	HRESULT result;

	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseState);
	if (FAILED(result))
	{
		// ��Ŀ���� �Ҿ��ų� acquire�� ���� �ʴ� ��Ȳ�̸�
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			m_mouse->Acquire();
		}
		else
			return false;	// �ٸ� ���� ��Ȳ�� ��� ���� ����
	}

	return true;
}

void SystemInputClass::ProcessInput()
{
	// ������ ������ ���� �Է���ġ���� �Ͼ ��ȭ ó��
	// ���콺 ��ġ ����, ���콺 Ŀ�� ��ġ ����
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	// ȭ�鿡�� ���콺�� ��� ���, ����� �ʵ��� 0���� ����
	if (m_mouseX < 0)
		m_mouseX = 0;

	if (m_mouseY < 0)
		m_mouseY = 0;

	// ���� ���������� ����� �ʵ��� ó��
	if (m_mouseX > m_screenWidth)
		m_mouseX = m_screenWidth;

	if (m_mouseY > m_screenHeight)
		m_mouseY = m_screenHeight;
}

bool SystemInputClass::IsEscapePressed()
{
	if (m_keyboardState[DIK_ESCAPE] & 0x80)
		return true;

	return false;
}

void SystemInputClass::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
}

void SystemInputClass::GetFunctionKeyPressed(unsigned int& key, bool& isKeyUp)
{
	bool isKeyDown = false;

	for (int i = DIK_F1; i <= DIK_F8; i++)
	{
		if (m_keyboardState[i] & 0x80)
		{
			m_keys[i] = true;
			key = i;
			isKeyUp = false;
			isKeyDown = true;
			return;
		}
	}

	if (!isKeyDown)
	{
		key = 0;
		isKeyUp = false;
	}

	for (int i = DIK_F1; i <= DIK_F8; i++)
	{
		if (m_keys[i])
		{
			key = i;
			isKeyUp = true;
			return;
		}
	}
}

void SystemInputClass::Shutdown()
{
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}
}
