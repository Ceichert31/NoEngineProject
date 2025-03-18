#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXColors.h>
#include <DirectXMath.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

class GraphicsPipeline
{
public:
	GraphicsPipeline();
	~GraphicsPipeline();

	//Init & Cleanup
	void InitD3D(HWND hWnd);
	void CleanD3D(void);

	//Utility
	void RenderFrame(FLOAT clearColor[4]);
	void Update();

private:
	//Shaders
	void InitPipeline();
	void InitGraphics();

	//Global 
	IDXGISwapChain* mpSwapChain = nullptr;
	ID3D11Device* mpDev = nullptr;
	ID3D11DeviceContext* mpDevcon = nullptr;
	ID3D11RenderTargetView* mpBackBuffer = nullptr;

	//Shader Global
	ID3D11VertexShader* mpVS = nullptr;
	ID3D11PixelShader* mpPS = nullptr;
	ID3D11Buffer* mpVBuffer = nullptr;
	ID3D11InputLayout* mpLayout = nullptr;
};

struct VERTEX
{
	FLOAT X, Y, Z; //Position
	FLOAT R, G, B, A; //color
};