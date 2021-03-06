#include "Game.h"

#include "Constants.h" 
#include "SpriteManager.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "GameManager.h"
#include <time.h>

#include <iostream>

using namespace std;


Game* Game::s_pInstance = 0; // singleton static instance of Game

Game::Game() : 
m_pWindow(0), 
m_pRenderer(0), 
m_bRunning(false)
{
	
}


Game::~Game()
{
	// clean up
	m_pRenderer = 0;
	m_pWindow = 0;
}

bool Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	// Init pseudo random number generator
	srand(static_cast<unsigned int>(time(NULL)));
	
	int flags = 0;

	// store the game width and height
	m_gameWidth = width;
	m_gameHeight = height;

	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	/* ALL SDL INITIALIZATIONS */
	
	// attempt to initialise SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
#ifdef _DEBUG
		cout << "SDL init success\n";
#endif
		// init the window
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

		if (m_pWindow != 0) // window init success
		{
#ifdef _DEBUG
			cout << "window creation success\n";
#endif
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);

			if (m_pRenderer != 0) // renderer init success
			{
#ifdef _DEBUG
				cout << "renderer creation success\n";
#endif
				SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
			}
			else
			{
#ifdef _DEBUG
				cout << "renderer init fail\n";
#endif
				return false; // renderer init fail
			}


		}
		else
		{
#ifdef _DEBUG
			cout << "window init fail\n";
#endif
			return false; // window init fail
		}

		
		
	}
	else
	{
#ifdef _DEBUG
		cout << "SDL init fail\n";
#endif
		return false; // SDL init fail
	}
	/* END OF ALL SDL INITIALIZATIONS */	



	/* INIT SPRITE MANAGER AND LOADING SPRITE ASSETS */
	if (!GSpriteManager::Instance()->init(m_pRenderer))
	{
		return false; // something went wrong loading sprites assets
	}

	/* INIT AUDIO MANAGER AND LOAD AUDIO ASSETS */
	if (!GAudioManager::Instance()->init())
	{
		return false; // something went wrong loading audio assets
	}

	/* INIT AUDIO MANAGER AND LOAD AUDIO ASSETS */
	if (!GGameManager::Instance()->init())
	{
		return false; // something went wrong loading audio assets
	}

	// start the menu state
	//m_pGameStateMachine = new GameStateMachine();
	//m_pGameStateMachine->changeState(new MainMenuState());

	m_bRunning = true; // everything inited successfully, start the main game loop
	
	return true;
}

// This runs one time on first frame
void Game::start()
{
	// Start background music
	GAudioManager::Instance()->playMusic("puzzleMusic", -1);
}

void Game::update(float timeStep)
{
	//m_pGameStateMachine->update();
	GGameManager::Instance()->update(timeStep);
	GInputManager::Instance()->update(timeStep);
	//gemTest->move(timeStep);
}

void Game::handleEvents()
{
	//TheInputHandler::Instance()->update();
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// User close game window
		case SDL_QUIT:
			m_bRunning = false;
			break;

		default:
			break;
		}

		// Propagate the input
		GInputManager::Instance()->handleEvent(event);
		
	}
}

void Game::render()
{
	SDL_RenderClear(m_pRenderer);		// clear the frame buffer	
	
	// Draw bottom part of the tunnel first	
	GSpriteManager::Instance()->drawCropped(sprite_assets::BACKGROUND, 0, 94, 0, 94, 755, 506, m_pRenderer, SDL_FLIP_NONE);

	// Draw board of GEMS
	GGameManager::Instance()->render(m_pRenderer);

	// Draw top part of tunnel last to simulate gems occlusion when falling
	GSpriteManager::Instance()->drawCropped(sprite_assets::BACKGROUND, 0, 0, 0, 0, 755, 94, m_pRenderer, SDL_FLIP_NONE);


	// DRAW GUI ELEMENTS
	GSpriteManager::Instance()->draw(sprite_assets::MENU_CONTAINER, 1, 0, 180, 100, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->drawScoreText(m_pRenderer);
	GSpriteManager::Instance()->drawTimeText(m_pRenderer);
	
	
	// if game is over show final GUI
	if (GGameManager::Instance()->isGameOver)
	{
		GSpriteManager::Instance()->draw(sprite_assets::MENU_CONTAINER, SCREEN_WIDTH_RESOLUTION / 2 - 80, SCREEN_HEIGHT_RESOLUTION / 2 - 125, 400, 250, m_pRenderer, SDL_FLIP_NONE);
		GSpriteManager::Instance()->draw(sprite_assets::BUTTON_OK, SCREEN_WIDTH_RESOLUTION / 2 + 180, SCREEN_HEIGHT_RESOLUTION / 2 + 5, 71, 69, m_pRenderer, SDL_FLIP_NONE);
		GSpriteManager::Instance()->draw(sprite_assets::BUTTON_NO, SCREEN_WIDTH_RESOLUTION / 2 - 20, SCREEN_HEIGHT_RESOLUTION / 2, 76, 78, m_pRenderer, SDL_FLIP_NONE);
		GSpriteManager::Instance()->drawGameOverText(m_pRenderer);
	}
	
	SDL_RenderPresent(m_pRenderer);		// render to the screen
}



void Game::clean()
{
#ifdef _DEBUG
	cout << "Cleaning game\n";
#endif
	GSpriteManager::Instance()->clearTextureMap();	
	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyRenderer(m_pRenderer);
	SDL_Quit();
}

