// NoEngineProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "WindowsPipeline.h"

int main()
{
    WindowsPipeline* pGame = new WindowsPipeline();
    pGame->InitWindows();

    delete pGame;
    pGame = nullptr;

    return 0;
}