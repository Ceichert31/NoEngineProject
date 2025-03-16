// NoEngineProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Game.h"

int main()
{
    Game* pGame = new Game();
    pGame->InitGame();

    delete pGame;
    pGame = nullptr;

    return 0;
}