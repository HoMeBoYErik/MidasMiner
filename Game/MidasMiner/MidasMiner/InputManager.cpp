#include "InputManager.h"
#include "SDL.h"

InputManager* InputManager::s_pInstance = 0;

void InputManager::handleEvent(SDL_Event& e)
{
	//If mouse event happened
	if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		if (e.type == SDL_MOUSEMOTION)
		{
			std::cout << "Mouse Position (" << x << ", " << y << ")" << std::endl;
		}
		else if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
				SDL_Log("Mouse Button 1 (left) is pressed.");
				std::cout << "Mouse Position (" << x << ", " << y << ")" << std::endl;
		}
			
		}
		
	}
}
