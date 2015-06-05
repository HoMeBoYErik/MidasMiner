// GameManager.h

#ifndef __GameManager__
#define __GameManager__

#include <iostream>
#include <string>
#include <map>
#include "SDL.h"

class GameManager
{
public:

	static GameManager* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new GameManager();
			return s_pInstance;
		}

		return s_pInstance;
	}

	bool init();
	void createNewBoard();
	void calculatePossibleSwaps();
	void handlePlayerInput();
	bool isValidSwap();
	void detectMatches();
	void removeChains();
	void updateScore();
	void makeGemsFall();
	void addNewGems();
	void shuffleBoard();

private:
	GameManager(){};
	~GameManager(){};

	bool isClickOnBoard(int x, int y);

	static GameManager* s_pInstance;
};

#endif

