#include "WindowsPipeline.h"
#include "GraphicsPipeline.h"

LRESULT CALLBACK WindowProc(HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam);

/// <summary>
/// Sets up a basic window
/// </summary>
void WindowsPipeline::InitWindows()
{
    //Create new graphics pipeline
    mpGraphicsPipeline = new GraphicsPipeline();

    //Create window
    WinMain(mHInstance, mHInstance, NULL, SW_SHOWNORMAL);
}

void WindowsPipeline::CleanupWindows()
{
    delete mpGraphicsPipeline;
    mpGraphicsPipeline = nullptr;
}

double currentFrame;
double lastFrame;

int WINAPI WindowsPipeline::WinMain(HINSTANCE mHInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
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
    wc.hInstance = mHInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    //wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = L"WindowClass1";

    //Register window class
    RegisterClassEx(&wc);

    //Set size and adjust
    RECT windowRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    //Create and cache window 
    hWnd = CreateWindowEx(NULL,
        L"WindowClass1", //Class name
        L"Title 1", //Title
        WS_OVERLAPPEDWINDOW,
        mWINDOW_X,
        mWINDOW_Y,
        windowRect.right - windowRect.left, //Width
        windowRect.bottom - windowRect.top, //Height
        NULL, //Parent window
        NULL,
        mHInstance,
        NULL);

    //Display window
    ShowWindow(hWnd, nShowCmd);

    //Set Window Transparent
    SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_LAYERED);
    SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 256, LWA_ALPHA);

    //Struct holds window event messages
    MSG msg = { 0 };

    //Init DirectX
    mpGraphicsPipeline->InitD3D(hWnd);

    //Game loop
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
        //Render 
        mpGraphicsPipeline->Update();
    }

    //Cleanup
    CleanupWindows();

    //Return quit message
    return (int)msg.wParam;
}

LRESULT WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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