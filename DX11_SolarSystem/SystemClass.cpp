#include "SystemClass.h"
#include "GraphicClass.h"
#include "CameraClass.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

HRESULT SystemClass::InitWindow(int& nCmdShow)
{
    HRESULT hr = S_OK;

    m_hInst = GetModuleHandle(NULL);

    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInst;
    wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_DX11SOLARSYSTEM));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = m_className;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    if (!RegisterClassEx(&wcex))
        return E_FAIL;

    RECT rc = { 0, 0, 640, 480 };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    m_hWnd = CreateWindow(m_className, "Direct3D 11 SolarSystem", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, m_hInst,
        NULL);
    if (!m_hWnd)
        return E_FAIL;

    ShowWindow(m_hWnd, nCmdShow);


    graphicClass = new GraphicClass(&m_hWnd);
    hr = graphicClass->InitGraphicClass();
    if (FAILED(hr))
    {
        graphicClass->Shutdown();
        delete graphicClass;
        graphicClass = nullptr;

        return hr;
    }

    cameraClass = new CameraClass();
    hr = cameraClass->Init(graphicClass->GetWidth(), graphicClass->GetHeight(), graphicClass->GetDevice(), graphicClass->GetImmediateContext());
    if(FAILED(hr))
    {
        cameraClass->Shutdown();
        delete cameraClass;
        cameraClass = nullptr;

        MessageBox(NULL,
            "cameraClassInit Error ", "Error", MB_OK);
        return hr;
    }

    return hr;
}

void SystemClass::Run()
{
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));

    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            graphicClass->Update();
            cameraClass->Update();
            graphicClass->Render();
        }
    }
}

void SystemClass::Shutdown()
{
    UnregisterClass(m_className, m_hInst);

    if (cameraClass != nullptr)
    {
        cameraClass->Shutdown();
        delete cameraClass;
        cameraClass = nullptr;
    }

    if (graphicClass != nullptr)
    {
        graphicClass->Shutdown();
        delete graphicClass;
        graphicClass = nullptr;
    }
}

HWND* SystemClass::GetHwnd()
{
    return &m_hWnd;
}
