#include "SystemClass.h"
#include "GraphicClass.h"
#include "CameraClass.h"
#include "ObjLoader.h"
#include "ObjectClass.h"
#include "GameTimer.h"
#include "LightClass.h"

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

    gameTimer = new GameTimer();
    gameTimer->Start();

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

    lightClass = new LightClass();
    hr = lightClass->Init(graphicClass->GetDevice(), graphicClass->GetImmediateContext());
    if (FAILED(hr))
    {
        lightClass->Shutdown();
        delete lightClass;
        lightClass = nullptr;

        MessageBox(NULL,
            "lightClassInit Error ", "Error", MB_OK);
        return hr;
    }

    objLoader = new ObjLoader();
    objLoader->Reset();
    objLoader->ReadFileCounts(loadFileName);    // .obj를 통해 각 데이터의 개수를 구함

    objectClass = new ObjectClass();
    objectClass->DynamicAllocationVertices(objLoader->GetFaceCount() * 3);  // vertex, texture, normal 3개를 저장해야해서 face*3
    objectClass->DynamicAllocationIndices(objLoader->GetFaceCount() * 3);

    objLoader->LoadObjVertexData(loadFileName, objectClass->GetVertices(), objectClass->GetIndices());  // 데이터 로드 + 저장

    objectClass->CreateVertexBuffer(graphicClass->GetDevice()); // 버텍스 버퍼 생성
    graphicClass->SetIAVertexBuffer(objectClass->GetVertexBuffer(), objectClass->GetStride(), objectClass->GetOffset());    // 버텍스 버퍼 등록
    objectClass->CreateIndexBuffer(graphicClass->GetDevice());   // 인덱스 버퍼 생성
    graphicClass->SetIAIndexBuffer(objectClass->GetIndexBuffer());  // 인덱스 버퍼 등록

    return hr;
}

void SystemClass::Run()
{
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));

    while (WM_QUIT != msg.message)
    {
        gameTimer->Tick();

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            graphicClass->Update();
            lightClass->Update();
            objectClass->Update(graphicClass->GetImmediateContext(), gameTimer->DeltaTime());
            cameraClass->Update(objectClass->GetWorldVector(), objectClass->GetScaleVector());
            objectClass->Render(graphicClass->GetImmediateContext(), cameraClass, graphicClass->GetShaderResourceViewVector(), graphicClass);
            graphicClass->Render();
        }
    }
}

void SystemClass::Shutdown()
{
    UnregisterClass(m_className, m_hInst);

    if (objectClass != nullptr)
    {
        delete objectClass;
        objectClass = nullptr;
    }

    if (objLoader != nullptr)
    {
        delete objLoader;
        objLoader = nullptr;
    }

    if (cameraClass != nullptr)
    {
        cameraClass->Shutdown();
        delete cameraClass;
        cameraClass = nullptr;
    }

    if (lightClass != nullptr)
    {
        lightClass->Shutdown();
        delete lightClass;
        lightClass = nullptr;
    }

    if (graphicClass != nullptr)
    {
        graphicClass->Shutdown();
        delete graphicClass;
        graphicClass = nullptr;
    }

    if (gameTimer != nullptr)
    {
        delete gameTimer;
        gameTimer = nullptr;
    }
}

HWND* SystemClass::GetHwnd()
{
    return &m_hWnd;
}
