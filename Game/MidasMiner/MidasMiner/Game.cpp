#include "Game.h"

#include "SpriteManager.h"
#include "InputManager.h"
#include "AudioManager.h"
/*#include "InputHandler.h"
#include "MainMenuState.h"
#include "GameObjectFactory.h"
#include "MenuButton.h"
#include "AnimatedGraphic.h"
#include "Player.h"
#include "ScrollingBackground.h"
#include "SoundManager.h"
#include "GameOverState.h"
#include "Snail.h"*/
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

	gemTest = new Sprite();	

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
	gemTest->move(timeStep);
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
		gemTest->handleEvent(event);
	}
}

void Game::render()
{
	SDL_RenderClear(m_pRenderer);		// clear the frame buffer

	//m_pGameStateMachine->render();
	
	// Draw bottom part of the tunnel first
	GSpriteManager::Instance()->drawCropped("background", 0, 94, 0, 94, 755, 506, m_pRenderer, SDL_FLIP_NONE);
	
	// Testing first column
	GSpriteManager::Instance()->draw("redGem", 325, 100, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("blueGem", 325, 145, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("greenGem", 325, 190, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("yellowGem", 325, 235, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("purpleGem",	325, 280, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("redGem",		325, 325, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("blueGem",		325, 370, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("greenGem",		325, 415, 35, 35, m_pRenderer, SDL_FLIP_NONE);

	// Testing second column
	GSpriteManager::Instance()->draw("redGem",		370, 100, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("blueGem",		370, 145, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("greenGem",		370, 190, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("yellowGem",	370, 235, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("purpleGem",	370, 280, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("redGem",		370, 325, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("blueGem",		370, 370, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("greenGem",		370, 415, 35, 35, m_pRenderer, SDL_FLIP_NONE);

	// Testing third column
	GSpriteManager::Instance()->draw("redGem",		415, 100, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("blueGem",		415, 145, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("greenGem",		415, 190, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("yellowGem",	415, 235, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("purpleGem",	415, 280, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("redGem",		415, 325, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("blueGem",		415, 370, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("greenGem",		415, 415, 35, 35, m_pRenderer, SDL_FLIP_NONE);

	// Testing fourth column
	GSpriteManager::Instance()->draw("redGem",		460, 100, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("blueGem",		460, 145, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("greenGem",		460, 190, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("yellowGem",	460, 235, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("purpleGem",	460, 280, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("redGem",		460, 325, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("blueGem",		460, 370, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("greenGem",		460, 415, 35, 35, m_pRenderer, SDL_FLIP_NONE);

	// Testing fifth column
	GSpriteManager::Instance()->draw("redGem",		505, 100, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("blueGem",		505, 145, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("greenGem",		505, 190, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("yellowGem",	505, 235, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("purpleGem",	505, 280, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("redGem",		505, 325, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("blueGem",		505, 370, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("greenGem",		505, 415, 35, 35, m_pRenderer, SDL_FLIP_NONE);

	// Testing sixth column
	GSpriteManager::Instance()->draw("redGem",		550, 100, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("blueGem",		550, 145, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("greenGem",		550, 190, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("yellowGem",	550, 235, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("purpleGem",	550, 280, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("redGem",		550, 325, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("blueGem",		550, 370, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("greenGem",		550, 415, 35, 35, m_pRenderer, SDL_FLIP_NONE);

	// Testing seventh column
	GSpriteManager::Instance()->draw("redGem",		595, 100, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("blueGem",		595, 145, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("greenGem",		595, 190, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("yellowGem",	595, 235, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("purpleGem",	595, 280, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("redGem",		595, 325, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("blueGem",		595, 370, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("greenGem",		595, 415, 35, 35, m_pRenderer, SDL_FLIP_NONE);

	// Testing eight column
	GSpriteManager::Instance()->draw("redGem",		640, 100, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("blueGem",		640, 145, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("greenGem",		640, 190, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("yellowGem",	640, 235, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("purpleGem",	640, 280, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("redGem",		640, 325, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("blueGem",		640, 370, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	GSpriteManager::Instance()->draw("greenGem",		640, 415, 35, 35, m_pRenderer, SDL_FLIP_NONE);

	gemTest->render(m_pRenderer);

	// Draw top part of tunnel last to simulate gems occlusion
	GSpriteManager::Instance()->drawCropped("background", 0, 0, 0, 0, 755, 94, m_pRenderer, SDL_FLIP_NONE);
	



	SDL_RenderPresent(m_pRenderer);		// render to the screen
}



void Game::clean()
{
#ifdef _DEBUG
	cout << "Cleaning game\n";
#endif

	//TheInputHandler::Instance()->clean();

	//m_pGameStateMachine->clean();

	//m_pGameStateMachine = 0;
	//delete m_pGameStateMachine;

	GSpriteManager::Instance()->clearTextureMap();
	delete gemTest;


	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyRenderer(m_pRenderer);
	SDL_Quit();
}

