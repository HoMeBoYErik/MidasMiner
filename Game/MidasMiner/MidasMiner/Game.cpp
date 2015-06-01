#include "Game.h"

/*#include "TextureManager.h"
#include "InputHandler.h"
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
m_bRunning(false),
m_bLevelComplete(false)
{
	// add some level files to an array
	//m_levelFiles.push_back("assets/map1.tmx");

	// start at this level
	m_currentLevel = 1;
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

	SDL_Surface* pTempSurface = IMG_Load("assets/BackGround.jpg");
	m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, pTempSurface);
	SDL_FreeSurface(pTempSurface);
	SDL_QueryTexture(m_pTexture, NULL, NULL, &m_sourceRectangle.w, &m_sourceRectangle.h);
	m_destinationRectangle.x = m_sourceRectangle.x = 0;
	m_destinationRectangle.y = m_sourceRectangle.y = 0;
	m_destinationRectangle.w = m_sourceRectangle.w;
	m_destinationRectangle.h = m_sourceRectangle.h;

	// add some sound effects - TODO move to better place
	/*TheSoundManager::Instance()->load("assets/DST-Away.ogg", "music1", SOUND_MUSIC);
	TheSoundManager::Instance()->load("assets/jump.wav", "jump", SOUND_SFX);
	//TheSoundManager::Instance()->load("phaser.wav", "shoot", SOUND_SFX);

	TheSoundManager::Instance()->playMusic("music1", -1);


	TheInputHandler::Instance()->initialiseJoysticks();

	// register the types for the game
	TheGameObjectFactory::Instance()->registerType("MenuButton", new MenuButtonCreator());
	TheGameObjectFactory::Instance()->registerType("Player", new PlayerCreator());
	TheGameObjectFactory::Instance()->registerType("AnimatedGraphic", new AnimatedGraphicCreator());
	TheGameObjectFactory::Instance()->registerType("ScrollingBackground", new ScrollingBackgroundCreator());
	TheGameObjectFactory::Instance()->registerType("Snail", new SnailCreator());*/

	// start the menu state
	//m_pGameStateMachine = new GameStateMachine();
	//m_pGameStateMachine->changeState(new MainMenuState());

	m_bRunning = true; // everything inited successfully, start the main game loop
	
	return true;
}

void Game::render()
{
	SDL_RenderClear(m_pRenderer);		// clear the frame buffer

	//m_pGameStateMachine->render();
	SDL_RenderCopy(m_pRenderer, m_pTexture, &m_sourceRectangle, &m_destinationRectangle);

	SDL_RenderPresent(m_pRenderer);		// render to the screen
}

void Game::update()
{
	//m_pGameStateMachine->update();
}

void Game::handleEvents()
{
	//TheInputHandler::Instance()->update();
	SDL_Event event;
	if (SDL_PollEvent(&event))
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
	}
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

	//TheTextureManager::Instance()->clearTextureMap();

	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyRenderer(m_pRenderer);
	SDL_Quit();
}

