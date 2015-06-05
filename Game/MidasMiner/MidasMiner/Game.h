//
//  Game.h
//
//

#ifndef __Game__
#define __Game__

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Sprite.h"
//#include "GameStateMachine.h"
//#include "SDL_mixer.h"
//#include "Vector2D.h"
#include <vector>

enum game_context
{
	MENU = 0,
	PLAYING = 1,
	PAUSED = 2,
	GAMEOVER = 3
};


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
	void start();
	void handleEvents();
	void update(float timeStep);
	void render();	
	void clean();

	SDL_Renderer* getRenderer() const { return m_pRenderer; }
	SDL_Window* getWindow() const { return m_pWindow; }
	//GameStateMachine* getStateMachine() { return m_pGameStateMachine; }
	
	bool running() { return m_bRunning; }
	
	void quit() { m_bRunning = false; }

	int getGameWidth() const { return m_gameWidth; }
	int getGameHeight() const { return m_gameHeight; }	

	Sprite *gemTest;

private:

	Game();
	~Game();

	Game(const Game&);
	Game& operator=(const Game&);

	static Game* s_pInstance;

	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;

	//GameStateMachine* m_pGameStateMachine;

	bool m_bRunning;

	int m_gameWidth;
	int m_gameHeight;
};

typedef Game TheGame;

#endif /* defined(__Game__) */
