// GameManager.cpp

#include "GameManager.h"
#include "SpriteManager.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "GemChain.h"

GameManager* GameManager::s_pInstance = 0;

Uint8 GameManager::gameObjectCounter = 0;



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
	hasToHandleMatches = false;

	createNewBoard();
	populateBoard();
	detectPossibleSwaps();

	mScore = 0;
	mGameTime = MATCH_TIME; // in seconds
	isGameRunning = true;
	hasTimeWarning = false;

	return true;
}

void GameManager::update(float timestep)
{
	// update game timer
	if (isGameRunning)
	{
		if (mGameTime <= 0)
		{
			isGameRunning = false;
			GInputManager::Instance()->userInteractionEnabled = false;
		}

		std::string gameTime = "Time: ";
		gameTime += std::to_string(static_cast<int>(mGameTime / MATCH_TIME));
		
		if ( (static_cast<int>(mGameTime) % MATCH_TIME) >= 10 ||
			static_cast<int>(mGameTime) == MATCH_TIME)
		{ 
			gameTime += ":"; 
		}
		else
		{ 
			if (!hasTimeWarning)
			{
				hasTimeWarning = true;
				GAudioManager::Instance()->playSound("timeWarning", -1);
			}
			gameTime += ":0";  
		}
		
		gameTime += std::to_string(static_cast<int>(mGameTime) % MATCH_TIME);

		GSpriteManager::Instance()->updateGameTime(gameTime, hasTimeWarning);
		
		mGameTime -= timestep;
	}

	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); ++it)
	{
		it->second->update(timestep);
	}

	if (hasAnimationRunning() && hasToHandleMatches)
	{		
		GInputManager::Instance()->userInteractionEnabled = false;
		return;
	}

	if (!hasAnimationRunning() && hasToHandleMatches)
	{
		GInputManager::Instance()->userInteractionEnabled = false;
		handleMatches();
		hasToHandleMatches = false;
		return;
	}

	if (!hasAnimationRunning() && hasToCheckSwap)
	{
		GInputManager::Instance()->userInteractionEnabled = false;
		// the swap was effective, play a nice sound
		if (isPossibleSwap(lastUserSwap))
		{
			GAudioManager::Instance()->playSound("swapSuccess", 0);
			hasToCheckSwap = false;			
			// handle matches
			handleMatches();
		}
		else
		{
			GAudioManager::Instance()->playSound("swapFailed", 0);
			// swap things back			
			swapGameObjects(lastUserToRow, lastUserToCol, lastUserFromRow, lastUserFromCol, true);
			hasToCheckSwap = false;			
		}

		return;
	}
	else
	{
		GInputManager::Instance()->userInteractionEnabled = true;
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

			Uint8 gem = board[row][col];

			go = new GameObject(++GameManager::gameObjectCounter);
			go->setSprites(gem);			

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
	m_PossibleSwaps.clear();

	for (int row = 0; row < BOARD_ROWS; row++)
	{
		for (int col = 0; col < BOARD_COLS; col++)
		{
			GameObject* gem = boardGameObjects[row][col];
			if (gem != NULL)
			{
				
				//Is it possible to swap this gem with the one on the right ?
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

				//Is it possible to swap this gem with the one below ?
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

	if (a == b)
	{
		std::cout << " Warning call same object animation multiple times " << std::endl;
	}

	// Begin animated swap
	if (animated)
	{
		a->animate(a->mPosX, a->mPosY, b->mPosX, b->mPosY, 0.3f); 
		GGameManager::Instance()->startedAnimation(); 
		//std::cout << " [" << static_cast<unsigned int>(a->id) << "] star anim" << std::endl;


		b->animate(b->mPosX, b->mPosY, a->mPosX, a->mPosY, 0.3f); 
		GGameManager::Instance()->startedAnimation();  
		//std::cout << " [" << static_cast<unsigned int>(b->id) << "] start anim" << std::endl;
	}
	// swap directly
	else
	{
		int aPosX, aPosY;
		aPosX =(int) a->mPosX; aPosY = (int) a->mPosY;
		a->setPosition((float)b->mPosX, (float)b->mPosY);
		b->setPosition((float)aPosX, (float) aPosY);
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


void GameManager::detectHorizontalMatches()
{
	m_detectedHorizontalChains.clear();
	
	for (int row = 0; row < BOARD_ROWS; row++)
	{
		for (int col = 0; col < BOARD_COLS - 2;)
		{
			Uint8 matchType = board[row][col];
			if (board[row][col + 1] == matchType
				&& board[row][col + 2] == matchType) 
			{
				GemChain* chain = new GemChain();
				chain->chainType = ChainTypeHorizontal;
				
				do 
				{
					chain->addGem(boardGameObjects[row][col]);
					col += 1;
				} while (col < BOARD_COLS && board[row][col] == matchType);

				// add the chain to current detected chains
				m_detectedHorizontalChains.insert(chain);
				continue;
			}
			col += 1;
		}
	}
 // result is saved in m_detectedHorizontalChains
}

void GameManager::detectVerticalMatches()
{
	m_detectedVerticalChains.clear();

	for (int col = 0; col < BOARD_COLS; ++col)
	{
		for (int row = 0; row < BOARD_ROWS - 2;)
		{
			Uint8 matchType = board[row][col];
			if (board[row + 1][col] == matchType
				&& board[row + 2][col] == matchType)
			{
				GemChain* chain = new GemChain();
				chain->chainType = ChainTypeVertical;
				do
				{
					chain->addGem(boardGameObjects[row][col]);
					row += 1;
				} while (row < BOARD_ROWS && board[row][col] == matchType);

				// add the chain to current detected chains
				m_detectedVerticalChains.insert(chain);
				continue;
			}
			row += 1;
		}
	}
}

void GameManager::handleMatches()
{
	// step 1: detect chains
	bool chainDetected = detectChains();
	// if no more chains exit and give back control to user
	if ( !chainDetected )
	{
		beginNextTurn();
		return;		
	}
	// step 2: if any, remove gem chains
	removeChains();
	// step 3: make gems falling down filling holes
	fillHoles();
	// step 4: add new gems from top
	addNewGems();	
	// step 5: recalculate possible swaps for the next round
	detectPossibleSwaps();
}

void GameManager::beginNextTurn()
{
	detectPossibleSwaps();
	GInputManager::Instance()->userInteractionEnabled = true;
}

bool GameManager::detectChains()
{
	detectHorizontalMatches();  // detect all new horizontal chains
	detectVerticalMatches();	// detect all new vertical chains

	// if we found new chains
	if (!m_detectedHorizontalChains.empty() || !m_detectedVerticalChains.empty())
	{
		return true;
	}
	return false;

}


void GameManager::removeChains()
{
	// remove horizontal chains first
	if (!m_detectedHorizontalChains.empty())
	{
		for (auto f : m_detectedHorizontalChains)
		{
			removeChainFromBoard(f);			
			GAudioManager::Instance()->playSound("removeChain", false);
			updateScore(100);

		}
	}
	// remove vertical chains then
	if(!m_detectedVerticalChains.empty())
	{
		for (auto f : m_detectedVerticalChains)
		{
			removeChainFromBoard(f);			
			GAudioManager::Instance()->playSound("removeChain", false);
			updateScore(100);
		}
	}	
}


void GameManager::removeChainFromBoard(GemChain* chain)
{
	for (auto c : chain->gems)
	{		
		// hide the gem
		c->isVisible = false;
		// TODO: spawn particles and point effect		

		// mark the cell as empty in logic representation
		int row, col;
		mapPointToBoardCell((int)c->mPosX, (int)c->mPosY, row, col);
		board[row][col] = board_elems::EMPTY;

		// move gameObjects to a reusable pool
		// if we formed a T shaped chain, one element is repeated and is already null
		if (boardGameObjects[row][col] != NULL)
		{
			m_reusableGems.push(boardGameObjects[row][col]);	//add the gem to a reusable pool of gameobject
			boardGameObjects[row][col] = NULL;					//mark the cell as empty in data representation
		}
		
	}
}

// Fill the holes left in the game board with new gems
void GameManager::fillHoles()
{
	for (int col = 0; col < BOARD_COLS; col++)
	{
		for (int row = BOARD_ROWS - 1; row > 0; row--)
		{
			// if this is a hole
			if (board[row][col] == board_elems::EMPTY)
			{	// try to make gems above fall down
				for (int lookup = row - 1; lookup >= 0; lookup--)
				{					
					if (board[lookup][col] != board_elems::EMPTY)
					{
						// move down the gem one cell
						board[row][col] = board[lookup][col];
						// move down game object
						boardGameObjects[row][col] = boardGameObjects[lookup][col];
						
						// update its animation position so it's prepared for animation
						boardGameObjects[row][col]->animOrigX = boardGameObjects[row][col]->mPosX;
						boardGameObjects[row][col]->animOrigY = boardGameObjects[row][col]->mPosY;
						
						int destX, destY;
						mapBoardCellToPoint(row, col, destX, destY);
						boardGameObjects[row][col]->animDestX = (float) destX;
						boardGameObjects[row][col]->animDestY = (float) destY;
						// copy the gameobject to an array of objects to animate
						m_gemFallingAnimationQueue.push(boardGameObjects[row][col]);
						
						// mark the new empty cell
						board[lookup][col] = board_elems::EMPTY;
						boardGameObjects[lookup][col] = NULL;

						break; // gem moved down, no further scan for this cell
					}					
				}
			}
		}
	}

	// finally start gems fall animation
	makeGemsFall();	
}

void GameManager::updateScore(int newScore)
{
	mScore += newScore;

	GSpriteManager::Instance()->updateScore(mScore);
}

void GameManager::makeGemsFall()
{
	//while there is something to animate	
	while (!m_gemFallingAnimationQueue.empty())
	{
		GameObject* g = m_gemFallingAnimationQueue.front();
		m_gemFallingAnimationQueue.pop();
		g->animateBounce(g->animOrigX, g->animOrigY, g->animDestX, g->animDestY, 0.35f);
		GGameManager::Instance()->startedAnimation(); 
		//std::cout << " Gameobject [" << static_cast<unsigned int>(g->id) << "] started animation" << std::endl;
		
	}
}

void GameManager::addNewGems()
{
	Uint8 gemType = board_elems::EMPTY;

	for (int col = 0; col < BOARD_COLS; col++)
	{
		for (int row = 0; row < BOARD_ROWS && board[row][col] == 0; row++)
		{
			Uint8 newGemType;
			do
			{
				newGemType = 1 + (Uint8)rand() % DIFFERENT_GEMS;
			} while (newGemType == gemType);
			gemType = newGemType; //avoid too many equal gems				

			// and we update the logic
			board[row][col] = gemType;

			// now that we have a new gem type to insert
			// we get a reusable gameobject and prepare it to fall back into scene
			if (!m_reusableGems.empty())
			{
				//SDL_assert(boardGameObjects[row][col] == NULL);
				// copy back this gem to the board in his future new position
				boardGameObjects[row][col] = m_reusableGems.front();
				m_reusableGems.pop();

				GameObject* gem = boardGameObjects[row][col];
				// update his visual parameters and position depending on gemtype
				if (gem != NULL)
				{
					gem->setSprites(gemType);

					// calculate new final position and animation params
					// put it outside the screen or animate with a delay
					// add a delay on animation

					// for now do not animate just place
					int posX, posY; // final destination position
					mapBoardCellToPoint(row, col, posX, posY);

					gem->animDestX = (float)posX;
					gem->animDestY = (float)posY;

					gem->animOrigX = static_cast<float>(posX);
					gem->animOrigY = static_cast<float>(BOARD_RECT.y - BOARD_CELL_Y);
					gem->mPosX = (float)posX;
					gem->mPosY = (float)(BOARD_RECT.y - BOARD_CELL_Y);

					boardGameObjects[row][col]->animateBounce(
						gem->animOrigX,
						gem->animOrigY,
						gem->animDestX,
						gem->animDestY,
						0.35f,
						static_cast<float>((BOARD_ROWS - row))*0.035f
						);
					GameManager::Instance()->startedAnimation();

					gem->isVisible = true;
				}
			}
			else
			{
				std::cout << "Out of reusable objects" << std::endl;
			}
		}
	}
	
	// sort of recursive call to handleMatches to verify if we formed new chains
	hasToHandleMatches = true;
}

void GameManager::startedAnimation()
{
	++mNumOfActiveAnimations;

	//std::cout << "Start-> Counter " << static_cast<unsigned int>(mNumOfActiveAnimations) << std::endl;
}

void GameManager::endedAnimation()
{
	--mNumOfActiveAnimations;
	
	//std::cout << "End-> Counter " << static_cast<unsigned int>(mNumOfActiveAnimations) << std::endl;
}

bool GameManager::hasAnimationRunning()
{
	if (mNumOfActiveAnimations > 0)
	{
		return true;
	}
	return false;
}

