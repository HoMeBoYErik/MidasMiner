//#include <SDL.h>
#include "Game.h"
#include "windows.h"
#include <iostream>

const int SCREEN_WIDTH_RESOLUTION = 755;
const int SCREEN_HEIGHT_RESOLUTION = 600;
const int FPS = 60;
const int DELAY_TIME = 1000 / FPS;

SDL_Window* g_pWindow = 0;
SDL_Renderer* g_pRenderer = 0;
bool g_bRunning = false; // bool for the game loop


int main(int argc, char* args[])
{
#ifdef _DEBUG
	AllocConsole();	
	freopen("CONOUT$", "w", stdout);
#endif

	Uint32 frameStart, frameTime;

#ifdef _DEBUG
	std::cout << "game init attempt...\n";
#endif

	// initialize SDL
	if (TheGame::Instance()->init("Midas Miner",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH_RESOLUTION,
		SCREEN_HEIGHT_RESOLUTION,
		false))
	{
#ifdef _DEBUG
		std::cout << "game init success!\n";
#endif
		
		/* //Game Loop reference
		double lastTime = getCurrentTime(); 
		while (true) { 
			double current = getCurrentTime(); 
			double elapsed = current - lastTime; 
			processInput(); 
			update( elapsed); render(); lastTime = current; 
		}
		*/

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
#ifdef _DEBUG
		std::cout << "game init failure - " << SDL_GetError() << "\n";
#endif
		return -1; // something went wrong
	}

#ifdef _DEBUG
	std::cout << "game closing...\n";
#endif
	TheGame::Instance()->clean();

	return 0;
}