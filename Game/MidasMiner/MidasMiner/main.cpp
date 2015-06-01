#include <SDL.h>

#define SCREEN_WIDTH_RESOLUTION 640
#define SCREEN_HEIGHT_RESOLUTION 480

SDL_Window* g_pWindow = 0;
SDL_Renderer* g_pRenderer = 0;

int main(int argc, char* args[])
{
	// initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) >= 0)
	{
		// if succeded create a window
		g_pWindow = SDL_CreateWindow("Midas Miner",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			SCREEN_WIDTH_RESOLUTION,
			SCREEN_HEIGHT_RESOLUTION,
			SDL_WINDOW_SHOWN);

		// if window creation was OK create a renderer
		if (g_pWindow != 0)
		{
			g_pRenderer = SDL_CreateRenderer(g_pWindow, -1, 0);
		}
	}
	else
	{
		// SDL failed to initialize
		return -1;
	}

	// draw the window
	SDL_SetRenderDrawColor(g_pRenderer, 0, 0, 0, 255); // clear buffer color set to black

	// clear the window
	SDL_RenderClear(g_pRenderer);

	// show the window
	SDL_RenderPresent(g_pRenderer);

	// set a delay before quitting
	SDL_Delay(5000);

	// clean up SDL
	SDL_Quit();

	return 0;
}