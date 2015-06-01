//
//  Game.h
//  SDL Game Programming Book
//
//

#ifndef __Game__
#define __Game__

#include "SDL.h"
#include <SDL_image.h>
//#include "GameStateMachine.h"
//#include "SDL_mixer.h"
//#include "Vector2D.h"
#include <vector>

class Game
{
public:

	static Game* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new Game();
			return s_pInstance;
		}

		return s_pInstance;
	}


	bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void render();
	void update();
	void handleEvents();
	void clean();

	SDL_Renderer* getRenderer() const { return m_pRenderer; }
	SDL_Window* getWindow() const { return m_pWindow; }
	//GameStateMachine* getStateMachine() { return m_pGameStateMachine; }

	SDL_Texture* m_pTexture;
	SDL_Rect m_sourceRectangle;
	SDL_Rect m_destinationRectangle;

	void setCurrentLevel(int currentLevel);
	const int getCurrentLevel() { return m_currentLevel; }

	void setNextLevel(int nextLevel) { m_nextLevel = nextLevel; }
	const int getNextLevel() { return m_nextLevel; }

	void setLevelComplete(bool levelComplete) { m_bLevelComplete = levelComplete; }
	const bool getLevelComplete() { return m_bLevelComplete; }

	bool running() { return m_bRunning; }

	void quit() { m_bRunning = false; }

	int getGameWidth() const { return m_gameWidth; }
	int getGameHeight() const { return m_gameHeight; }

	std::vector<std::string> getLevelFiles() { return m_levelFiles; }

private:

	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;

	//GameStateMachine* m_pGameStateMachine;

	bool m_bRunning;

	static Game* s_pInstance;

	int m_gameWidth;
	int m_gameHeight;

	int m_currentLevel;
	int m_nextLevel;
	bool m_bLevelComplete;

	std::vector<std::string> m_levelFiles;

	Game();
	~Game();

	Game(const Game&);
	Game& operator=(const Game&);
};

typedef Game TheGame;

#endif /* defined(__Game__) */
