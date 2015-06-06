// GameManager.h

#ifndef __GameManager__
#define __GameManager__

#include <iostream>
#include <string>
#include <map>
#include <set>
#include "SDL.h"
#include "GameObject.h"

static const int BOARD_COLS = 8;
static const int BOARD_ROWS = 8;
static const SDL_Rect BOARD_RECT = {325, // start at x pixel 
									100, // start at y pixel
									350, // is w pixel width
									350  // is h pixel height
									}; // valid game board region 
static const int BOARD_CELL_X = 45;
static const int BOARD_CELL_Y = 45;

static const Uint8 DIFFERENT_GEMS = 5;



enum board_elems
{
	EMPTY = 0,
	BLUE_GEM = 1,
	GREEN_GEM = 2,
	PURPLE_GEM = 3,
	RED_GEM = 4,
	YELLOW_GEM = 5
};


class SwapPair
{
public:
	GameObject* a;
	GameObject* b;

	SwapPair(){};
	~SwapPair(){};	
	
	SwapPair& SwapPair::operator= (const SwapPair &cSource)
	{
		// do the copy
		a = cSource.a;
		b = cSource.b;

		// return the existing object
		return *this;
	}

	/*bool operator==(SwapPair* other)
	{
		if ((this->a->id == other->a->id && this->b->id == other->b->id) ||
			(this->b->id == other->a->id && this->a->id == other->b->id))
		{
			return true;
		}

		return false;
	}

	bool operator<(const SwapPair* other)
	{
		if (this == other)return false;
		return true;
	}*/

};

// Compare functor for SwapPair set
/*struct swap_compare
{

	bool operator<(const SwapPair* other)const
	{
		
	}

	bool operator()(const SwapPair* sp1, const SwapPair* sp2)const
	{
		return false;
	}	
};
typedef struct swap_compare SwapCompare;*/

class GameManager
{
public:
	static Uint8 gameObjectCounter;

	static GameManager* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new GameManager();
			return s_pInstance;
		}

		return s_pInstance;
	}

	GameObject* factory(std::string gameObjectType);
	void registerGameObject(Uint8 id, GameObject *pGameObject);

	bool init();
	void update(float timestep);
	void render(SDL_Renderer* pRenderer);	
	void handlePlayerInput();
	
	bool isValidSwap(int fromRow, int fromCol, int toRow, int toCol);	
	void swapGameObjects(int fromRow, int fromCol, int toRow, int toCol, bool animated);
	bool hasChainAt(int row, int col);
	void detectPossibleSwaps();
	bool isPossibleSwap(SwapPair* swappable);
	bool isPossibleSwap(int fromRow, int fromCol, int toRow, int toCol);
	void registerLastUserSwap(int fromRow, int fromCol, int toRow, int toCol);
	void detectMatches();
	void removeChains();
	void updateScore();
	void makeGemsFall();
	void addNewGems();
	

	// board manipulation methods and accessors
	void createNewBoard();
	void shuffleBoard();
	void populateBoard();
	bool isPointInBoard(int x, int y);
	bool isSameBoardCell(int fromRow, int fromCol, int toRow, int toCol);
	void mapPointToBoardCell(int x, int y, int &row, int &col);
	void mapBoardCellToPoint(int row, int col, int &x, int &y);

	void startedAnimation();
	void endedAnimation();
	bool hasAnimationRunning();

	bool hasToCheckSwap;
	
	/* '0' = empty cell */
	/* 'b' =  Blue Gem */
	Uint8 board[BOARD_ROWS][BOARD_COLS]; // logic representation of game board
	GameObject* boardGameObjects[BOARD_ROWS][BOARD_COLS]; // game object mapping between cells and gems
	// Add a collection of gameobject to maintain relationships
	std::map<Uint8, GameObject*> m_GameObjects;

	std::set < SwapPair*> m_PossibleSwaps;
	SwapPair* lastUserSwap;
	int lastUserFromRow = 0;
	int lastUserFromCol = 0;
	int lastUserToRow = 0;
	int lastUserToCol = 0;

private:
	GameManager(){};
	~GameManager(){};

	Uint8 mNumOfActiveAnimations = 0;

	static GameManager* s_pInstance;
};
typedef GameManager GGameManager;
#endif

