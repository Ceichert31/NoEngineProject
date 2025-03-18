#pragma once

#include <windows.h>
#include <windowsx.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

class GraphicsPipeline;

class WindowsPipeline
{
public:
	void InitWindows();
	void CleanupWindows();

	//Windows functions
	int WINAPI WinMain(HINSTANCE mHInstance,
		HINSTANCE hPrevInstance,
		LPSTR lpCmdLine,
		int nShowCmd);
private:
	GraphicsPipeline* mpGraphicsPipeline = nullptr;
	HINSTANCE mHInstance = HINSTANCE();

	const int mWINDOW_X = 300;
	const int mWINDOW_Y = 300;
};

