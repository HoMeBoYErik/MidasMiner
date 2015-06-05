// GameManager.h

#ifndef __GameManager__
#define __GameManager__

#include <iostream>
#include <string>
#include <map>
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
	
	void calculatePossibleSwaps();
	void handlePlayerInput();
	bool isValidSwap();
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
	void mapPointToBoardCell(int x, int y, int &row, int &col);
	void mapBoardCellToPoint(int row, int col, int &x, int &y);
	
	/* '0' = empty cell */
	/* 'b' =  Blue Gem */
	Uint8 board[BOARD_ROWS][BOARD_COLS]; // logic representation of game board
	GameObject* boardGameObjects[BOARD_ROWS][BOARD_COLS]; // game object mapping between cells and boards
	// Add a collection of gameobject to maintain relationships
	std::map<Uint8, GameObject*> m_GameObjects;


private:
	GameManager(){};
	~GameManager(){};

	bool isClickOnBoard(int x, int y);

	static GameManager* s_pInstance;
};
typedef GameManager GGameManager;
#endif

