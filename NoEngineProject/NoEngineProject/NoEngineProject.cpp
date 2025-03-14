// NoEngineProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// include the basic windows header files and the Direct3D header files
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")

//Global 
IDXGISwapChain* swapChain;
ID3D11Device* dev;
ID3D11DeviceContext* devcon;
ID3D11RenderTargetView* backBuffer;

//Init & Cleanup
void InitD3D(HWND hWnd);
void CleanD3D(void);

//Utility
void RenderFrame(void);

const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 400;

/// <summary>
/// Window X starting position
/// </summary>
const int WINDOW_X = 300;
/// <summary>
/// Window Y starting position
/// </summary>
const int WINDOW_Y = 300;

//Windows 
int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd);

LRESULT CALLBACK WindowProc(HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam);



/// <summary>
/// Initializes Direct3D
/// </summary>
/// <param name="hWnd"></param>
void InitD3D(HWND hWnd)
{
    //Struct to hold info
    DXGI_SWAP_CHAIN_DESC scd;

    //Clear out struct
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //32-bit color
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hWnd;
    scd.SampleDesc.Count = 4;
    scd.Windowed = TRUE;

    //Create new device
    D3D11CreateDeviceAndSwapChain(NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        NULL,
        NULL,
        NULL,
        D3D11_SDK_VERSION,
        &scd,
        &swapChain,
        &dev,
        NULL,
        &devcon);

    //Set render target
    ID3D11Texture2D* pBackBuffer = nullptr;
    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    //Set back buffer then clear memory
    dev->CreateRenderTargetView(pBackBuffer, NULL, &backBuffer);
    pBackBuffer->Release();

    //Set render target as back buffer
    devcon->OMSetRenderTargets(1, &backBuffer, NULL);

    //Set viewport
    D3D11_VIEWPORT viewport;

    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = WINDOW_WIDTH;
    viewport.Height = WINDOW_HEIGHT;

    //Activate viewport struct
    devcon->RSSetViewports(1, &viewport);
}

void CleanD3D(void)
{
    //Clean all devices
    swapChain->Release();
    backBuffer->Release();
    dev->Release();
    devcon->Release();
}

const FLOAT CLEAR_COLOR[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
void RenderFrame(void)
{
    //Clear backbuffer
    devcon->ClearRenderTargetView(backBuffer, CLEAR_COLOR);

    //Render

    //Switch buffers
    swapChain->Present(0, 0);
}

int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd)
{
    //Window handle
    HWND hWnd;

    //Holds window class info
    WNDCLASSEX wc;

    //Clear memeory for window class
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    //Init struct with info
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    //wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = L"WindowClass1";

    //Register window class
    RegisterClassEx(&wc);

    //Set size and adjust
    RECT windowRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    //Create and cache window
    hWnd = CreateWindowEx(NULL,
        L"WindowClass1", //Class name
        L"Title 1", //Title
        WS_OVERLAPPEDWINDOW,
        WINDOW_X,
        WINDOW_Y,
        windowRect.right - windowRect.left, //Width
        windowRect.bottom - windowRect.top, //Height
        NULL, //Parent window
        NULL,
        hInstance,
        NULL);

    //Display window
    ShowWindow(hWnd, nShowCmd);

    //Struct holds window event messages
    MSG msg = { 0 };

    //Init DirectX
    InitD3D(hWnd);

    //Infinite loop
    while (true)
    {
        //Check for messages in queue
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            //Translate keystroke messages
            TranslateMessage(&msg);

            //Send message to WindowProc function
            DispatchMessage(&msg);
            
            //Quit message
            if (msg.message == WM_QUIT)
                break;
        }
        //Game Code
        RenderFrame();
    }

    //Cleanup DirectX
    CleanD3D();

    //Return quit message
    return msg.wParam;
}

//Message handler
LRESULT CALLBACK WindowProc(HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    //Find what code to run for message given
    switch (message)
    {
    case WM_DESTROY:
        //Close app
        PostQuitMessage(0);
        return 0;
        break;
    }
    //Handle additional messages switch didn't handle
    return DefWindowProc(hWnd, message, wParam, lParam);
}

int main()
{
    HINSTANCE hInstance = HINSTANCE();

    WinMain(hInstance, hInstance, NULL, SW_SHOWNORMAL);

    //MessageBox(NULL, L"Hello World!", L"Hello World Window", MB_ICONEXCLAMATION | MB_OK);

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
