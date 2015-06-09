//#include <SDL.h>
#include "Constants.h"
#include "Game.h"
#include "Timer.h"
#include "windows.h"
#include <iostream>


//Keeps track of time between steps
Timer stepTimer;

int main(int argc, char* args[])
{
#ifdef _DEBUG
	// Create a console to dump debug
	// Remove this and CRT suppress warning when compiling for release
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
		

		// Start Function - initialize game objects
		TheGame::Instance()->start();

		// Main Game Loop
		while (TheGame::Instance()->running())
		{
			TheGame::Instance()->handleEvents();

			frameStart = SDL_GetTicks();	// milliseconds since the game started: as Unity Time.time			
			float timeStep = stepTimer.getTicks() / 1000.f; // as Unity Time.deltaTime
			//std::cout << "deltaTime = " << timeStep << "\n";
			
			TheGame::Instance()->update(timeStep);

			//Restart step timer
			stepTimer.start();
			
			TheGame::Instance()->render();

			// calculate frame duration until here
			frameTime = SDL_GetTicks() - frameStart;

			// If we have time left in the frame, put the CPU to sleep to 
			// reduce power consumption - thinking about mobiles
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