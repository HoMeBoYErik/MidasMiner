// GameManager.h

#ifndef __GameManager__
#define __GameManager__

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <queue>
#include "SDL.h"
#include "GameObject.h"
#include "SwapPair.h" 
#include "GemChain.h"


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

static const int MATCH_TIME = 60;

static const int BASE_SCORE = 100;


enum board_elems
{
	EMPTY = 0,
	BLUE_GEM_ELEM = 1,
	GREEN_GEM_ELEM = 2,
	PURPLE_GEM_ELEM = 3,
	RED_GEM_ELEM = 4,
	YELLOW_GEM_ELEM = 5
};


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

	void registerGameObject(Uint8 id, GameObject *pGameObject);

	bool init();
	void update(float timestep);
	void render(SDL_Renderer* pRenderer);	
	void handlePlayerInput();
	void restartGame();
	void refreshBoardOnRestartGame();
	
	bool isValidSwap(int fromRow, int fromCol, int toRow, int toCol);	
	void swapGameObjects(int fromRow, int fromCol, int toRow, int toCol, bool animated);
	bool hasChainAt(int row, int col);
	void detectPossibleSwaps();
	bool isPossibleSwap(SwapPair* swappable);
	bool isPossibleSwap(int fromRow, int fromCol, int toRow, int toCol);
	void registerLastUserSwap(int fromRow, int fromCol, int toRow, int toCol);
	void handleMatches();
	bool detectChains();
	void detectHorizontalMatches();
	void detectVerticalMatches();	
	void removeChains();
	void fillHoles();	
	void makeGemsFall();
	void addNewGems();
	void updateScore(int newScore);
	void beginNextTurn();	

	// board manipulation methods and accessors
	void createNewBoard();
	void shuffleBoard();
	void populateBoard();
	bool isPointInBoard(int x, int y);
	bool isSameBoardCell(int fromRow, int fromCol, int toRow, int toCol);
	void mapPointToBoardCell(int x, int y, int &row, int &col);
	void mapBoardCellToPoint(int row, int col, int &x, int &y);
	void removeChainFromBoard(GemChain *);
	void removeGemFromBoard(int row, int col);
	void removeGemFromBoard(GameObject *);

	// Manage the animation counter
	// If there is an animation active prevent user from interacting
	void startedAnimation();
	void endedAnimation();
	bool hasAnimationRunning();

	bool hasToCheckSwap;
	bool hasToHandleMatches;

	int mScore;
	float mGameTime;
	bool isGameRunning;
	bool hasTimeWarning;
	bool isGameOver;
	int warningChannel;
	
	/* '0' = empty cell */
	/* 'b' =  Blue Gem */
	Uint8 board[BOARD_ROWS][BOARD_COLS]; // logic representation of game board
	GameObject* boardGameObjects[BOARD_ROWS][BOARD_COLS]; // game object mapping between cells and gems
	// Add a collection of gameobject to maintain relationships
	std::map<Uint8, GameObject*> m_GameObjects; // access with game object ID

	

	std::set < SwapPair*> m_PossibleSwaps;
	SwapPair* lastUserSwap;
	int lastUserFromRow = 0;
	int lastUserFromCol = 0;
	int lastUserToRow = 0;
	int lastUserToCol = 0;

	std::set<GemChain*> m_detectedHorizontalChains;
	std::set<GemChain*> m_detectedVerticalChains;

	std::queue<GameObject*> m_gemFallingAnimationQueue;
	std::queue<GameObject *> m_reusableGems;	

private:
	GameManager(){};
	~GameManager(){};

	Uint8 mNumOfActiveAnimations = 0;

	static GameManager* s_pInstance;
};
typedef GameManager GGameManager;
#endif

