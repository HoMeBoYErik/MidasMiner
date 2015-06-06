// GameManager.cpp

#include "GameManager.h"
#include "InputManager.h"
#include "AudioManager.h"

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
	lastUserSwap = new SwapPair();
	lastUserSwap->a = NULL;
	lastUserSwap->b = NULL;

	hasToCheckSwap = false;

	createNewBoard();
	populateBoard();
	detectPossibleSwaps();

	return true;
}

void GameManager::update(float timestep)
{
	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); ++it)
	{
		it->second->update(timestep);
	}

	if (!hasAnimationRunning() && hasToCheckSwap)
	{
		// the swap was effective, play a nice sound
		if (isPossibleSwap(lastUserSwap))
		{
			GAudioManager::Instance()->playSound("swapSuccess", 0);
			hasToCheckSwap = false;

			std::cout << "SWAP SUCCESS " << std::endl;
		}
		else
		{
			GAudioManager::Instance()->playSound("swapFailed", 0);
			// swap things back
			
			swapGameObjects(lastUserToRow, lastUserToCol, lastUserFromRow, lastUserFromCol, true);

			hasToCheckSwap = false;

			std::cout << "SWAP FAIL " << std::endl;
		}
	}
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
	Uint8 newGem;

	for (int row = 0; row < BOARD_ROWS; ++row)
	{
		for (int col = 0; col < BOARD_COLS; ++col)
		{
			// avoid to fill the board with chains of 3 gems
			// check the 2 gems to the left and the 2 up
			do{ newGem = 1 + (Uint8)rand() % DIFFERENT_GEMS; } // generate random new board )
			while( (col >= 2 &&
				   board[row][col - 1] == newGem &&
				   board[row][col - 2] == newGem )
				   ||
				   (row >= 2 &&
				   board[row - 1][col] == newGem &&
				   board[row - 2][col] == newGem) );

			board[row][col] = newGem;
		}
	}	
}

// Create gems gameobject and assign positions and colors in the game board
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

void GameManager::handlePlayerInput()
{

}

bool GameManager::hasChainAt(int row, int col)
{
	Uint8 gemType = board[row][col];

	int horzLenght = 1;
	
	// detecting an horizontal chain to the left
	for (int i = col - 1; i >= 0 && board[row][i] == gemType; i--, horzLenght++);
	// detecting an horizontal chain to the right
	for (int i = col + 1; i < BOARD_COLS && board[row][i] == gemType; i++, horzLenght++);

	if (horzLenght >= 3) return true;

	int vertLenght = 1;
	// detecting a vertical chain above
	for (int i = row - 1; i >= 0 && board[i][col] == gemType; i--, vertLenght++);
	// detecting a vertical chain below
	for (int i = row + 1; i < BOARD_ROWS && board[i][col] == gemType; i++, vertLenght++);

	return (vertLenght >= 3);	
}

void GameManager::detectPossibleSwaps()
{
	for (int row = 0; row < BOARD_ROWS; row++)
	{
		for (int col = 0; col < BOARD_COLS; col++)
		{
			GameObject* gem = boardGameObjects[row][col];
			if (gem != NULL)
			{
				
				//Is it possible to swap this cookie with the one on the right ?
				if (col < BOARD_COLS - 1)
				{
					GameObject* gemToTheRight = boardGameObjects[row][col + 1];					
					if (gemToTheRight != NULL)
					{
						swapGameObjects(row, col, row, col + 1, false);

						if (hasChainAt(row, col + 1) || hasChainAt(row, col))
						{
							SwapPair *swappable = new SwapPair();
							swappable->a = gem;
							swappable->b = gemToTheRight;
							m_PossibleSwaps.insert(swappable);
						}
						// Swap elements back
						swapGameObjects(row, col + 1, row, col, false);
					}					
				}

				//Is it possible to swap this cookie with the one below
				if (row < BOARD_ROWS - 1)
				{
					GameObject* gemBelow = boardGameObjects[row+1][col];
					if (gemBelow != NULL)
					{
						// Swap elements
						swapGameObjects(row, col, row+1, col, false);
						if (hasChainAt(row, col) || hasChainAt(row+1, col))
						{
							SwapPair *swappable = new SwapPair();
							swappable->a = gem;
							swappable->b = gemBelow;
							m_PossibleSwaps.insert(swappable);
						}
						// Swap elements back
						swapGameObjects(row+1, col, row, col, false);
					}
				}
			}
		}
	}
}

// Determine if is a valid input swap (the player clicked two adiacent gems)
bool GameManager::isValidSwap(int fromRow, int fromCol, int toRow, int toCol)
{
	// potentially valid vertical swap
	if (abs(fromRow - toRow) == 1)
	{
		// valid vertical swap
		if (fromCol == toCol)
		{
			return true;
		}
		// is a diagonal swap
		else
		{
			return false;
		}
	}
	// potentially valid horizontal swap
	else if (abs(fromCol - toCol) == 1)
	{
		// valid horizontal swap
		if (fromRow == toRow)
		{
			return true;
		}
		// is a diagonal swap
		else
		{
			return false;
		}
	}

	return false; // invalid swap or same cell
}

/* IS POSSIBLE SWAP - 2 overloads*/
bool GameManager::isPossibleSwap(int fromRow, int fromCol, int toRow, int toCol)
{
	SwapPair* swappable = new SwapPair();
	swappable->a = boardGameObjects[fromRow][fromCol];
	swappable->b = boardGameObjects[toRow][toCol];

	return isPossibleSwap(swappable);
}

bool GameManager::isPossibleSwap(SwapPair* swappable)
{
	// iterate iver all possible valid swaps and match it with current desired swap
	for (auto f : m_PossibleSwaps ) 
	{
		if (f->a->id == swappable->a->id && f->b->id == swappable->b->id) return true;
		if (f->b->id == swappable->a->id && f->a->id == swappable->b->id) return true;
	}

	return false;		
}

/* END OF IS POSSIBLE SWAP*/



bool GameManager::isSameBoardCell(int fromRow, int fromCol, int toRow, int toCol)
{
	if (fromRow == toRow && fromCol == toCol)
	{
		return true;
	}

	return false;
}

void GameManager::swapGameObjects(int fromRow, int fromCol, int toRow, int toCol, bool animated)
{
	GameObject *a;
	GameObject *b;

	a = boardGameObjects[fromRow][fromCol];
	b = boardGameObjects[toRow][toCol];	

	// Swap object in logic representatio of the board
	std::swap(board[fromRow][fromCol], board[toRow][toCol]);	
	std::swap(boardGameObjects[fromRow][fromCol], boardGameObjects[toRow][toCol]);

	// Begin animated swap
	if (animated)
	{
		a->animate(a->mPosX, a->mPosY, b->mPosX, b->mPosY, 0.3f); startedAnimation();
		b->animate(b->mPosX, b->mPosY, a->mPosX, a->mPosY, 0.3f); startedAnimation();
	}
	// swap directly
	else
	{
		int aPosX, aPosY;
		aPosX = a->mPosX; aPosY = a->mPosY;
		a->setPosition(b->mPosX, b->mPosY);
		b->setPosition(aPosX, aPosY);
	}	
}

void GameManager::registerLastUserSwap(int fromRow, int fromCol, int toRow, int toCol)
{
	lastUserSwap->a = boardGameObjects[fromRow][fromCol];
	lastUserSwap->b = boardGameObjects[toRow][toCol];
	lastUserFromRow = fromRow;
	lastUserFromCol = fromCol;
	lastUserToRow = toRow;
	lastUserToCol = toCol;
	// set the flag to true so will be checked for validity
	hasToCheckSwap = true;
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

void GameManager::startedAnimation()
{
	++mNumOfActiveAnimations;

}

void GameManager::endedAnimation()
{
	--mNumOfActiveAnimations;	
}

bool GameManager::hasAnimationRunning()
{
	if (mNumOfActiveAnimations > 0)
	{
		return true;
	}
	return false;
}

