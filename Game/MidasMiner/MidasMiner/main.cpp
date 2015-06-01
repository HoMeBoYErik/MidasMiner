//#include <SDL.h>
#include "Game.h"
#include "windows.h"
#include <iostream>

const int SCREEN_WIDTH_RESOLUTION = 640;
const int SCREEN_HEIGHT_RESOLUTION = 480;
const int FPS = 60;
const int DELAY_TIME = 1000.0f / FPS;

SDL_Window* g_pWindow = 0;
SDL_Renderer* g_pRenderer = 0;
bool g_bRunning = false; // bool for the game loop


int main(int argc, char* args[])
{
	//AllocConsole();
	//freopen_s("CON", "w", stdout);
	Uint32 frameStart, frameTime;

	std::cout << "game init attempt...\n";

	// initialize SDL
	if (TheGame::Instance()->init("Midas Miner",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH_RESOLUTION,
		SCREEN_HEIGHT_RESOLUTION,
		false))
	{
		std::cout << "game init success!\n";

		// Main Game Loop
		while (TheGame::Instance()->running())
		{
			frameStart = SDL_GetTicks();

			TheGame::Instance()->handleEvents();
			TheGame::Instance()->update();
			TheGame::Instance()->render();

			frameTime = SDL_GetTicks() - frameStart;

			if (frameTime < DELAY_TIME)
			{
				SDL_Delay((int)(DELAY_TIME - frameTime));
			}
		}		
	}
	else
	{
		std::cout << "game init failure - " << SDL_GetError() << "\n";
		return -1; // something went wrong
	}

	std::cout << "game closing...\n";
	TheGame::Instance()->clean();

	return 0;
}