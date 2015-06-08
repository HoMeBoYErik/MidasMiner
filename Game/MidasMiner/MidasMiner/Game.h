//
//  Game.h
//
// This class manage main game loop and init of all the other
// systems. Also init the window and the renderer through SDL
#ifndef __Game__
#define __Game__

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
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

	// Init the game window
	bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void start();					// called on first frame
	void handleEvents();			// entry point for input events detected by SDL
	void update(float timeStep);	// update function called every tic
	void render();					// called every frame
	void clean();					// clean resources

	// Main renderer
	SDL_Renderer* getRenderer() const { return m_pRenderer; }
	// The window
	SDL_Window* getWindow() const { return m_pWindow; }	
	// When this go to false the game close
	bool running() { return m_bRunning; }
	// Called to quit the game from everywhere
	void quit() { m_bRunning = false; }
	// Get Window size in pixels
	int getGameWidth() const { return m_gameWidth; }
	int getGameHeight() const { return m_gameHeight; }	

private:

	Game();
	~Game();

	Game(const Game&);
	Game& operator=(const Game&);

	static Game* s_pInstance;

	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	
	//When this go to false the game close
	bool m_bRunning;
	// Window size in pixels
	int m_gameWidth;
	int m_gameHeight;
};

typedef Game TheGame;

#endif /* defined(__Game__) */
