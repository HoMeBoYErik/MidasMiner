// GameManager.cpp

#include "GameManager.h"

GameManager* GameManager::s_pInstance = 0;

Uint8 GameManager::gameObjectCounter = 0;

GameObject* GameManager::factory(std::string gameObjectType)
{
	return new GameObject(0, "Test");
}

// Add a new gameobject to the gameobject map
void GameManager::registerGameObject(Uint8 id, GameObject *pGameObject)
{
	m_GameObjects[id] = pGameObject;
}

bool GameManager::init()
{
	createNewBoard();
	populateBoard();

	return true;
}

void GameManager::update(float timestep)
{

}

void GameManager::render(SDL_Renderer* pRenderer)
{
	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); ++it)
	{
		it->second->render(pRenderer);
	}
}

void GameManager::createNewBoard()
{
	for (int row = 0; row < BOARD_ROWS; ++row)
	{
		for (int col = 0; col < BOARD_COLS; ++col)
		{
			board[row][col] = 1 + (Uint8)rand() % DIFFERENT_GEMS; // generate random new board
		}
	}	
}

void GameManager::populateBoard()
{
	for (int row = 0; row < BOARD_ROWS; ++row)
	{
		for (int col = 0; col < BOARD_COLS; ++col)
		{
			GameObject *go = NULL;
			
			// Switch Value Contained board to draw the gem
			switch ( board[row][col] )
			{				
				case BLUE_GEM:
					go = new GameObject(++GameManager::gameObjectCounter, "BlueGem");					
					go->setNormalSprite("blueGem");
					go->setSelectedSprite("blueGemSelected");				
					break;

				case GREEN_GEM:
					go = new GameObject(++GameManager::gameObjectCounter, "GreenGem");
					go->setNormalSprite("greenGem");
					go->setSelectedSprite("greenGemSelected");
					break;

				case PURPLE_GEM:
					go = new GameObject(++GameManager::gameObjectCounter, "PurpleGem");
					go->setNormalSprite("purpleGem");
					go->setSelectedSprite("purpleGemSelected");
					break;

				case RED_GEM:
					go = new GameObject(++GameManager::gameObjectCounter, "RedGem");
					go->setNormalSprite("redGem");
					go->setSelectedSprite("redGemSelected");
					break;

				case YELLOW_GEM:
					go = new GameObject(++GameManager::gameObjectCounter, "YellowGem");
					go->setNormalSprite("yellowGem");
					go->setSelectedSprite("yellowGemSelected");
					break;

				default:
					break;
			}

			// Complete other game object data
			// SET POSITION
			int posX, posY;
			GGameManager::mapBoardCellToPoint(row, col, posX, posY);
			if (go != NULL)
			{
				go->setPosition((float)posX, (float)posY);
				/// END OF SET POSITION
				registerGameObject(go->id, go);
				boardGameObjects[row][col] = go;
			}			
		}
	}
}


void GameManager::shuffleBoard()
{
	// swap two random elements for a random number of times
}

// UTILS FUNCTIONS TO MAP CLICK AND PIXELS TO GAME BOARD
// Return true if point expressed in pixels fall inside the game board
bool GameManager::isPointInBoard(int x, int y)
{
	if (x >= BOARD_RECT.x &&
		x < BOARD_RECT.x + BOARD_RECT.w  &&
		y >= BOARD_RECT.y &&
		y < BOARD_RECT.y + BOARD_RECT.h )
	{
		return true;
	}

	return false;
}

// Given a point in pixels (inside the board) return corresponding cell indices
void GameManager::mapPointToBoardCell(int x, int y, int &row, int &col)
{
	if (isPointInBoard(x, y))
	{
		row = (y - BOARD_RECT.y) / BOARD_CELL_Y;
		col = (x - BOARD_RECT.x) / BOARD_CELL_X;

		// clamp values
		if (row > BOARD_ROWS - 1)
		{
			row = BOARD_ROWS - 1;
		}
		if (col > BOARD_COLS - 1)
		{
			col = BOARD_COLS - 1;
		}
	}
	else
	{
		row = 0;
		col = 0;
	}
	
}

// Given valid indices (row, col) for the game boards, returns start pixels to draw the sprite 
void GameManager::mapBoardCellToPoint(int row, int col, int &x, int &y)
{
	x = BOARD_RECT.x + (col *  BOARD_CELL_X);
	y = BOARD_RECT.y + (row *  BOARD_CELL_Y);
}

void GameManager::calculatePossibleSwaps()
{

}

void handlePlayerInput();

bool GameManager::isValidSwap()
{

}


void GameManager::detectMatches()
{

}

void GameManager::removeChains()
{

}

void GameManager::updateScore()
{

}

void GameManager::makeGemsFall()
{

}

void GameManager::addNewGems()
{

}

