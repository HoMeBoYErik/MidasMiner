#include "InputManager.h"
#include "SDL.h"
#include "AudioManager.h"
#include "GameManager.h"
#include "Game.h"

InputManager* InputManager::s_pInstance = 0;


bool InputManager::noGemSelected()
{
	return (fromRow == -1 && fromCol == -1);
}

void InputManager::selectSwapFrom(int x, int y)
{
	// Store current row and col as from selection
	GGameManager::Instance()->mapPointToBoardCell(x, y, fromRow, fromCol);
	// Player select a gem
	selectedGameObjectFrom = GGameManager::Instance()->boardGameObjects[fromRow][fromCol];
	selectedGameObjectFrom->setSelected(true);
}

void InputManager::selectSwapTo(int x, int y)
{
	// Store current row and col as to selection
	GGameManager::Instance()->mapPointToBoardCell(x, y, toRow, toCol);
	selectedGameObjectTo = GGameManager::Instance()->boardGameObjects[toRow][toCol];
	selectedGameObjectTo->setSelected(true);
}

void InputManager::clearSwapTo()
{
	toRow = -1;
	toCol = -1;
	selectedGameObjectTo = NULL;
}

void InputManager::clearAllSwap()
{

	selectedGameObjectFrom->setSelected(false);
	selectedGameObjectTo->setSelected(false);

	// NULLIFY FROM object
	selectedGameObjectFrom = NULL;
	fromRow = -1;
	fromCol = -1;
	// NULLIFY TO object
	selectedGameObjectTo = NULL;
	toRow = -1;
	toCol = -1;
}

void InputManager::resetSwap()
{
	// Turn off old FROM object
	selectedGameObjectFrom->setSelected(false);	
	// Swap pointers and coords
	fromRow = toRow;
	fromCol = toCol;
	selectedGameObjectFrom = selectedGameObjectTo;
	
	// Turn on new FROM object
	selectedGameObjectFrom->setSelected(true);	
	
	// NULLIFY OLD TO object
	selectedGameObjectTo = NULL;
	toRow = -1;
	toCol = -1;
}

void InputManager::update(float timestep)
{
	/*if (GGameManager::Instance()->hasAnimationRunning() || GGameManager::Instance()->hasToCheckSwap )
	{
		userInteractionEnabled = false;
	}
	else
	{
		userInteractionEnabled = true;
	}*/
}

void InputManager::handleEvent(SDL_Event& e)
{
	// In Game Interaction
	if (userInteractionEnabled)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		// Begin click of left button
		if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && e.key.repeat == 0)
		{

			//std::cout << "Left mouse button pressed down " << std::endl;
			// if is a valid cell
			if (GGameManager::Instance()->isPointInBoard(x, y))
			{
				// there is no previous gem selected
				if (noGemSelected())
				{
					selectSwapFrom(x, y);
					// Play a sound for selected gem
					GAudioManager::Instance()->playSound("combo1", 0);
				}
				// second click on another gem: two step swap
				else
				{
					selectSwapTo(x, y);
					GAudioManager::Instance()->playSound("combo1", 0);

					if (GGameManager::Instance()->isValidSwap(fromRow, fromCol, toRow, toCol))
					{
						//std::cout << "1 Valid swap from (" << fromRow << "," << fromCol << ") to :(" << toRow << "," << toCol << ")" << std::endl;
						//std::cout << "Update game board, start animations and more" << std::endl;
						//if (GGameManager::Instance()->isPossibleSwap(fromRow, fromCol, toRow, toCol))
						//{
							GGameManager::Instance()->swapGameObjects(fromRow, fromCol, toRow, toCol, true);
							GGameManager::Instance()->registerLastUserSwap(fromRow, fromCol, toRow, toCol);
							
						//}			
												
						//userInteractionEnabled = false;
						
						clearAllSwap();
					}
					else if (GGameManager::Instance()->isSameBoardCell(fromRow, fromCol, toRow, toCol))
					{
						// clear swap to
						clearSwapTo();
					}
					// invalid swap, reset selection and leave selected the new one				
					else
					{
						resetSwap();
					}
				}
			}
		}

		else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT && e.key.repeat == 0)
		{
			//std::cout << " Left Mouse button up event" << std::endl;
			// if is a valid cell
			if (GGameManager::Instance()->isPointInBoard(x, y))
			{
				// there is a previous gem selected
				if (!noGemSelected())
				{
					selectSwapTo(x, y);

					if (GGameManager::Instance()->isValidSwap(fromRow, fromCol, toRow, toCol))
					{
						//std::cout << "2 Valid swap from (" << fromRow << "," << fromCol << ") to :(" << toRow << "," << toCol << ")" << std::endl;
						//std::cout << "Update game board, start animations and more" << std::endl;
						//if (GGameManager::Instance()->isPossibleSwap(fromRow, fromCol, toRow, toCol))
						//{
							GGameManager::Instance()->swapGameObjects(fromRow, fromCol, toRow, toCol, true);
							GGameManager::Instance()->registerLastUserSwap(fromRow, fromCol, toRow, toCol);
							
						//}//userInteractionEnabled = false;
						clearAllSwap();
					}
					else if (GGameManager::Instance()->isSameBoardCell(fromRow, fromCol, toRow, toCol))
					{
						// clear swap to
						clearSwapTo();
					}
					// player just clicked one gem as a simple select
					else
					{
						resetSwap();
					}
				}
			}
		}
	}

	// Game Over Menu Interaction
	else if (GGameManager::Instance()->isGameOver)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && e.key.repeat == 0)
		{
			// DETECT Click on X Button close game
			if (x > 358 && x < 430 && y > 298 && y < 370 )
			{
				TheGame::Instance()->quit();
			}
			else if (x > 560 && x < 630 && y > 298 && y < 370)
			{
				GGameManager::Instance()->restartGame();
			}
			
		}		
	}
}
