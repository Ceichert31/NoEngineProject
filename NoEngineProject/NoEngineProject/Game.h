#pragma once

// include the basic windows header files and the Direct3D header files
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXColors.h>
#include <DirectXMath.h>

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

class Game
{
public:
	//Game startup
	void InitGame();
	void CleanupGame();
	
	//Init & Cleanup
	void InitD3D(HWND hWnd);
	void CleanD3D(void);

	//Shaders
	void InitPipeline();
	void InitGraphics();

	//Utility
	void RenderFrame(FLOAT clearColor[4]);
	void Update();

	//Windows functions
	int WINAPI WinMain(HINSTANCE mHInstance,
		HINSTANCE hPrevInstance,
		LPSTR lpCmdLine,
		int nShowCmd);

private:
	//Global 
	HINSTANCE mHInstance;
	IDXGISwapChain* mpSwapChain = nullptr;
	ID3D11Device* mpDev = nullptr;
	ID3D11DeviceContext* mpDevcon = nullptr;
	ID3D11RenderTargetView* mpBackBuffer = nullptr;

	//Shader Global
	ID3D11VertexShader* mpVS = nullptr;
	ID3D11PixelShader* mpPS = nullptr;
	ID3D11Buffer* mpVBuffer;
	ID3D11InputLayout* mpLayout = nullptr;

	//Size
	//const int WINDOW_WIDTH = 500;
	//const int WINDOW_HEIGHT = 400;
	const int mWINDOW_X = 300;
	const int mWINDOW_Y = 300;
};

struct VERTEX
{
	FLOAT X, Y, Z; //Position
	FLOAT R, G, B, A; //color
};

