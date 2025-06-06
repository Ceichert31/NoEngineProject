#include "GraphicsPipeline.h"
#include "WindowsPipeline.h"

GraphicsPipeline::GraphicsPipeline()
{
    
}

GraphicsPipeline::~GraphicsPipeline()
{
    CleanD3D();
}

/// <summary>
/// Initializes Direct3D
/// </summary>
/// <param name="hWnd"></param>
void GraphicsPipeline::InitD3D(HWND hWnd)
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

    //Create debug layer
    UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_1,
    };

    //Create new device
    HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
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

    //Check result
    assert(SUCCEEDED(hr));

    //Set render target
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = mpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    //Check result
    assert(SUCCEEDED(hr));

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
    viewport.Width = SCREEN_WIDTH;
    viewport.Height = SCREEN_HEIGHT;

    //Activate viewport struct
    mpDevcon->RSSetViewports(1, &viewport);

    InitPipeline();
    InitGraphics();
}

void GraphicsPipeline::CleanD3D(void)
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

void GraphicsPipeline::InitPipeline()
{
    //Load and compile vertex and fragment shader
    ID3DBlob* pVertexBuffer, * pPixelBuffer, * pErrorBlob;
    HRESULT hr = D3DCompileFromFile(L"shaders.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VertexMain", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pVertexBuffer, &pErrorBlob);

    //If failed, output error
    if (FAILED(hr))
    {
        if (pErrorBlob)
        {
            OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
        }
    }

    hr = D3DCompileFromFile(L"shaders.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PixelMain", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pPixelBuffer, &pErrorBlob);

    //If failed, output error
    if (FAILED(hr))
    {
        if (pErrorBlob)
        {
            OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
        }
    }


    //Create shader objects
    mpDev->CreateVertexShader(pVertexBuffer->GetBufferPointer(), pVertexBuffer->GetBufferSize(), NULL, &mpVS);
    mpDev->CreatePixelShader(pPixelBuffer->GetBufferPointer(), pPixelBuffer->GetBufferSize(), NULL, &mpPS);

    //Set shader objects
    mpDevcon->VSSetShader(mpVS, 0, 0);
    mpDevcon->PSSetShader(mpPS, 0, 0);

    //Input layout
    //Each value has 4 bytes
    //D3D11_APPEND_ALIGNED_ELEMENT (figures out byte spacing)
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    UINT numElements = ARRAYSIZE(layout);

    //Create input layout
    hr = mpDev->CreateInputLayout(layout, numElements, pVertexBuffer->GetBufferPointer(), pVertexBuffer->GetBufferSize(), &mpLayout);

    //Check result
    assert(SUCCEEDED(hr));

    //Set input layout
    mpDevcon->IASetInputLayout(mpLayout);
}

void GraphicsPipeline::InitGraphics()
{
    //Create vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC; //write access by CPU read by GPU
    bd.ByteWidth = sizeof(VERTEX) * 3; //size of buffer
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; //set buffer as vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //allow CPU to write to buffer

    //Verticies
    VERTEX triangleVerticies[3] = {
        {0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
        {0.45f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
        {-0.45f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    };

    //Create vertex buffer
    HRESULT hr = mpDev->CreateBuffer(&bd, NULL, &mpVBuffer); //Create buffer

    //Check result
    assert(SUCCEEDED(hr));

    //Copy verts into buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    mpDevcon->Map(mpVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms); //Map buffer
    memcpy(ms.pData, triangleVerticies, sizeof(triangleVerticies)); //Copy data
    mpDevcon->Unmap(mpVBuffer, NULL); //Unmap buffer 
}

FLOAT CLEAR_COLOR[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
void GraphicsPipeline::RenderFrame(FLOAT clearColor[4])
{
    //Clear backbuffer
    mpDevcon->ClearRenderTargetView(mpBackBuffer, CLEAR_COLOR);

    //Render

    //Set vertex buffer
    UINT stride = sizeof(VERTEX);
    UINT offset = 0;
    mpDevcon->IASetVertexBuffers(0, 1, &mpVBuffer, &stride, &offset);

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

void GraphicsPipeline::Update()
{
    RenderFrame(CLEAR_COLOR);
}