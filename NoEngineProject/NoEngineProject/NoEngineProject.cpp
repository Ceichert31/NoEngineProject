// NoEngineProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <windowsx.h>

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

int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd);

LRESULT CALLBACK WindowProc(HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam);



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

    //Create and cache window
    hWnd = CreateWindowEx(NULL,
        L"WindowClass1", //Class name
        L"Title 1", //Title
        WS_OVERLAPPEDWINDOW,
        WINDOW_X,
        WINDOW_Y,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        NULL, //Parent window
        NULL,
        hInstance,
        NULL);

    //Display window
    ShowWindow(hWnd, nShowCmd);

    //Struct holds window event messages
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0))
    {
        //Translate keystroke messages
        TranslateMessage(&msg);

        //Send message to WindowProc function
        DispatchMessage(&msg);
    }

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
