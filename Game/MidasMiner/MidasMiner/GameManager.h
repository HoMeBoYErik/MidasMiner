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
#include <memory>


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

	// add a new gameobject to a std::map used to update and render
	void registerGameObject(Uint8 id, std::shared_ptr<GameObject> pGameObject);

	bool init();	// init all variables						
	void update(float timestep); // update game logic
	void render(SDL_Renderer* pRenderer); // draw game objects on screen	
	void handlePlayerInput(); // handle input
	void restartGame();	// restart the game logic for a new game
	void refreshBoardOnRestartGame(); // recalculate sprite renderer for new game
	
	bool isValidSwap(int fromRow, int fromCol, int toRow, int toCol); // determine if is a valid user move (eg: no diagonal)	
	void swapGameObjects(int fromRow, int fromCol, int toRow, int toCol, bool animated); // swap 2 gems in the board
	bool hasChainAt(int row, int col); // helper function that detect chain of 3 gems above and to the left of a cell
	void detectPossibleSwaps();	// calculate all possible valid moves (moves that generate a chain of 3 or more) useful to give hints to the player
	bool isPossibleSwap(std::shared_ptr<SwapPair> swappable);	// valid if the swap generate a chain
	bool isPossibleSwap(int fromRow, int fromCol, int toRow, int toCol); // valid if the swap generate a chain
	void registerLastUserSwap(int fromRow, int fromCol, int toRow, int toCol); // register last player move to undo that if it's invalid
	void handleMatches();	// start the process to look for chains, remove chains, fill holes and make fall down new gems
	bool detectChains(); // detect if there are any chains
	void detectHorizontalMatches();// detect if there are any chains in horizontal
	void detectVerticalMatches();	// detect if there are any chains in vertical
	void removeChains();	// remove detected chains
	void fillHoles();	// calculate the movement downwards to make the gem fall down
	void makeGemsFall(); // start gem fall animation
	void addNewGems(); // reuse gameobjects to simulate the fall of new gems
	void updateScore(int newScore); // update the score in logic and call the gui
	void beginNextTurn();	// user is able to input again

	// board manipulation methods and accessors
	void createNewBoard(); // generate a new logic board
	void shuffleBoard(); // shuffle logic elemenets: TODO not implemented
	void populateBoard(); // assign a gameobject (gem) to every logic cell
	bool isPointInBoard(int x, int y); // if is a valid input in game board (the click is inside the board?)
	bool isSameBoardCell(int fromRow, int fromCol, int toRow, int toCol); // user clicked two times the same cell?
	void mapPointToBoardCell(int x, int y, int &row, int &col); // given a point in pixels return the cell coordinates
	void mapBoardCellToPoint(int row, int col, int &x, int &y); // given row and col of the table return the pixel whare to draw the sprite
	void removeChainFromBoard(std::shared_ptr<GemChain>); // remove a detected chain from game board
	void removeGemFromBoard(int row, int col); // hide and remove a gem from the game
	void removeGemFromBoard(GameObject *); // hide and remove a gem from the game

	// Manage the animation counter
	// If there is an animation active prevent user from interacting
	void startedAnimation(); // increment animation reference counter
	void endedAnimation(); // decrement animation reference counter
	bool hasAnimationRunning(); // when animation counter is 0 there are no active animation so the user can interact

	bool hasToCheckSwap; // does the game logic has to check a new user swap?
	bool hasToHandleMatches; // does the game logic has to calculate and handle new chain matches?

	int mScore; // player current score
	float mGameTime; // time of a game match = 60 secs
	bool isGameRunning; // true when mGameTime > 0
	bool hasTimeWarning; // used to reproduce time running out sound
	bool isGameOver; // when the match ended = true
	int warningChannel; // record the time running out sound channel to stop it from looping 
	
	/* '0' = empty cell */
	/* 'b' =  Blue Gem */
	Uint8 board[BOARD_ROWS][BOARD_COLS]; // logic representation of game board
	std::shared_ptr<GameObject> boardGameObjects[BOARD_ROWS][BOARD_COLS]; // game object mapping between cells and gems
	// Add a collection of gameobject to maintain relationships
	std::map<Uint8, std::shared_ptr<GameObject>> m_GameObjects; // access with game object ID

	

	std::set <std::shared_ptr<SwapPair>> m_PossibleSwaps;
	std::shared_ptr<SwapPair> lastUserSwap;
	int lastUserFromRow = 0;
	int lastUserFromCol = 0;
	int lastUserToRow = 0;
	int lastUserToCol = 0;

	// structure to hold detected chains at every logic update
	std::set<std::shared_ptr<GemChain>> m_detectedHorizontalChains;
	std::set<std::shared_ptr<GemChain>> m_detectedVerticalChains;

	// gems in the queue are processed to reproduce fall animation 
	std::queue<std::shared_ptr<GameObject>> m_gemFallingAnimationQueue;
	// gems removed from the game are reused to simulate new gems entering the screen
	std::queue<std::shared_ptr<GameObject>> m_reusableGems;

private:
	GameManager(){};
	~GameManager(){};

	Uint8 mNumOfActiveAnimations = 0;

	static GameManager* s_pInstance;
};
typedef GameManager GGameManager;
#endif

