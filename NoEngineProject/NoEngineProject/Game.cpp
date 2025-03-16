#include "Game.h"

void Game::InitGame()
{
    mHInstance = HINSTANCE();

    WinMain(mHInstance, mHInstance, NULL, SW_SHOWNORMAL);
}

void Game::CleanupGame()
{
    mHInstance = nullptr;
}

/// <summary>
/// Initializes Direct3D
/// </summary>
/// <param name="hWnd"></param>
void Game::InitD3D(HWND hWnd)
{
    //Struct to hold info
    DXGI_SWAP_CHAIN_DESC scd;

    //Clear out struct
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //32-bit color
    scd.BufferDesc.Width = SCREEN_WIDTH; //Backbuffer width
    scd.BufferDesc.Height = SCREEN_HEIGHT; //Backbuffer height
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //How swap chain is used
    scd.OutputWindow = hWnd; //Window to use
    scd.SampleDesc.Count = 1; //Multisamples
    scd.SampleDesc.Quality = 0; //Multisample quality level
    scd.Windowed = TRUE; //Windowed or full screen
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //Allows alt+enter to fullscreen

    //Create new device
    D3D11CreateDeviceAndSwapChain(NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        NULL,
        NULL,
        NULL,
        D3D11_SDK_VERSION,
        &scd,
        &mpSwapChain,
        &mpDev,
        NULL,
        &mpDevcon);

    //Set render target
    ID3D11Texture2D* pBackBuffer = nullptr;
    mpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    //Set back buffer then clear memory
    mpDev->CreateRenderTargetView(pBackBuffer, NULL, &mpBackBuffer);
    pBackBuffer->Release();

    //Set render target as back buffer
    mpDevcon->OMSetRenderTargets(1, &mpBackBuffer, NULL);

    //Set viewport
    D3D11_VIEWPORT viewport;

    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = SCREEN_HEIGHT;
    viewport.Height = SCREEN_HEIGHT;

    //Activate viewport struct
    mpDevcon->RSSetViewports(1, &viewport);

    InitPipeline();
    InitGraphics();
}

void Game::CleanD3D(void)
{
    mpSwapChain->SetFullscreenState(FALSE, NULL); //Set windowed

    //Cleanup shader objects
    mpLayout->Release();
    mpVS->Release();
    mpPS->Release();
    mpVBuffer->Release();

    //Clean all devices
    mpSwapChain->Release();
    mpBackBuffer->Release();
    mpDev->Release();
    mpDevcon->Release();
}

void Game::InitPipeline()
{
    //Load and compile vertex and fragment shader
    ID3DBlob* pVS, * pPS, *pErrorBlob;
    HRESULT hr = D3DCompileFromFile(L"Shaders.shader", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "vs_main", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pVS, &pErrorBlob);
    
    //If failed, output error
    if (FAILED(hr))
    {
        if (pErrorBlob)
        {
            OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
        }
    }

    hr = D3DCompileFromFile(L"Shaders.shader", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PShader", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pPS, &pErrorBlob);

    //If failed, output error
    if (FAILED(hr))
    {
        if (pErrorBlob)
        {
            OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
        }
    }


    //Create shader objects
    mpDev->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), NULL, &mpVS);
    mpDev->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), NULL, &mpPS);

    //Set shader objects
    mpDevcon->VSSetShader(mpVS, 0, 0);
    mpDevcon->PSSetShader(mpPS, 0, 0);

    //Input layout
    //Each value has 4 bytes
    //D3D11_APPEND_ALIGNED_ELEMENT (figures out byte spacing)
    D3D11_INPUT_ELEMENT_DESC ied[2] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    //Create input layout
    mpDev->CreateInputLayout(ied, 2, pVS->GetBufferPointer(), pVS->GetBufferSize(), &mpLayout);
    mpDevcon->IASetInputLayout(mpLayout);
}

void Game::InitGraphics()
{
    //Create vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC; //write access by CPU read by GPU
    bd.ByteWidth = sizeof(VERTEX) * 3; //size of buffer
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; //set buffer as vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //allow CPU to write to buffer

    mpDev->CreateBuffer(&bd, NULL, &mpVBuffer); //Create buffer

    //Verticies
    VERTEX triangleVerticies[3] = {
        {0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
        {0.45f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
        {-0.45f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    };

    //Copy verts into buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    mpDevcon->Map(mpVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms); //Map buffer
    memcpy(ms.pData, triangleVerticies, sizeof(triangleVerticies)); //Copy data
    mpDevcon->Unmap(mpVBuffer, NULL); //Unmap buffer 
}

FLOAT CLEAR_COLOR[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
void Game::RenderFrame(FLOAT clearColor[4])
{
    //Clear backbuffer
    mpDevcon->ClearRenderTargetView(mpBackBuffer, CLEAR_COLOR);

    //Render

    //Set vertex buffer
    UINT stride = sizeof(VERTEX);
    UINT offset = 0;
    mpDevcon->IAGetVertexBuffers(0, 1, &mpVBuffer, &stride, &offset);

    //Set primitive type
    mpDevcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //Draw shape
    mpDevcon->Draw(
        3, //Vertex count 
        0  //Starting vertex
    );

    //Switch buffers
    mpSwapChain->Present(0, 0);
}

void Game::Update()
{
    RenderFrame(CLEAR_COLOR);
}

//Prototype here for now
LRESULT CALLBACK WindowProc(HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam);

int WINAPI Game::WinMain(HINSTANCE mHInstance,
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

    //Struct holds window event messages
    MSG msg = { 0 };

    //Init DirectX
    InitD3D(hWnd);

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
        //Game Code
        Update();
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
