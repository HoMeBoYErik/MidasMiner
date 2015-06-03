#include "Game.h"

#include "TextureManager.h"
#include "InputManager.h"
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
m_bRunning(false),
m_bLevelComplete(false)
{
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
	

	/*SDL_Surface* pTempSurface = IMG_Load("assets/BackGround.jpg");
	m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, pTempSurface);
	SDL_FreeSurface(pTempSurface);
	SDL_QueryTexture(m_pTexture, NULL, NULL, &m_sourceRectangle.w, &m_sourceRectangle.h);
	m_destinationRectangle.x = m_sourceRectangle.x = 0;
	m_destinationRectangle.y = m_sourceRectangle.y = 0;
	m_destinationRectangle.w = m_sourceRectangle.w;
	m_destinationRectangle.h = m_sourceRectangle.h;*/

	// Loading background image asset
	TheTextureManager::Instance()->load("assets/BackGround.jpg", "background", m_pRenderer);
	// Loading gems image assets
	TheTextureManager::Instance()->load("assets/Blue.png", "blueGem", m_pRenderer);
	TheTextureManager::Instance()->load("assets/Green.png", "greenGem", m_pRenderer);
	TheTextureManager::Instance()->load("assets/Purple.png", "purpleGem", m_pRenderer);
	TheTextureManager::Instance()->load("assets/Red.png", "redGem", m_pRenderer);
	TheTextureManager::Instance()->load("assets/Yellow.png", "yellowGem", m_pRenderer);

	gemTest = new Sprite();


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
	
	TheTextureManager::Instance()->draw("background", 0, 0, 755, 600, m_pRenderer, SDL_FLIP_NONE);
	
	// Testing first column
	TheTextureManager::Instance()->draw("redGem",		325, 100, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("blueGem",		325, 145, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("greenGem",		325, 190, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("yellowGem",	325, 235, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("purpleGem",	325, 280, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("redGem",		325, 325, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("blueGem",		325, 370, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("greenGem",		325, 415, 35, 35, m_pRenderer, SDL_FLIP_NONE);

	// Testing second column
	TheTextureManager::Instance()->draw("redGem",		370, 100, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("blueGem",		370, 145, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("greenGem",		370, 190, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("yellowGem",	370, 235, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("purpleGem",	370, 280, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("redGem",		370, 325, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("blueGem",		370, 370, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("greenGem",		370, 415, 35, 35, m_pRenderer, SDL_FLIP_NONE);

	// Testing third column
	TheTextureManager::Instance()->draw("redGem",		415, 100, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("blueGem",		415, 145, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("greenGem",		415, 190, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("yellowGem",	415, 235, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("purpleGem",	415, 280, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("redGem",		415, 325, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("blueGem",		415, 370, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("greenGem",		415, 415, 35, 35, m_pRenderer, SDL_FLIP_NONE);

	// Testing fourth column
	TheTextureManager::Instance()->draw("redGem",		460, 100, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("blueGem",		460, 145, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("greenGem",		460, 190, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("yellowGem",	460, 235, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("purpleGem",	460, 280, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("redGem",		460, 325, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("blueGem",		460, 370, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("greenGem",		460, 415, 35, 35, m_pRenderer, SDL_FLIP_NONE);

	// Testing fifth column
	TheTextureManager::Instance()->draw("redGem",		505, 100, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("blueGem",		505, 145, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("greenGem",		505, 190, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("yellowGem",	505, 235, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("purpleGem",	505, 280, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("redGem",		505, 325, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("blueGem",		505, 370, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("greenGem",		505, 415, 35, 35, m_pRenderer, SDL_FLIP_NONE);

	// Testing sixth column
	TheTextureManager::Instance()->draw("redGem",		550, 100, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("blueGem",		550, 145, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("greenGem",		550, 190, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("yellowGem",	550, 235, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("purpleGem",	550, 280, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("redGem",		550, 325, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("blueGem",		550, 370, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("greenGem",		550, 415, 35, 35, m_pRenderer, SDL_FLIP_NONE);

	// Testing seventh column
	TheTextureManager::Instance()->draw("redGem",		595, 100, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("blueGem",		595, 145, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("greenGem",		595, 190, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("yellowGem",	595, 235, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("purpleGem",	595, 280, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("redGem",		595, 325, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("blueGem",		595, 370, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("greenGem",		595, 415, 35, 35, m_pRenderer, SDL_FLIP_NONE);

	// Testing eight column
	TheTextureManager::Instance()->draw("redGem",		640, 100, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("blueGem",		640, 145, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("greenGem",		640, 190, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("yellowGem",	640, 235, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("purpleGem",	640, 280, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("redGem",		640, 325, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("blueGem",		640, 370, 35, 35, m_pRenderer, SDL_FLIP_NONE);
	TheTextureManager::Instance()->draw("greenGem",		640, 415, 35, 35, m_pRenderer, SDL_FLIP_NONE);

	gemTest->render(m_pRenderer);

	SDL_RenderPresent(m_pRenderer);		// render to the screen
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

		// Pass on the input
		TheInputManager::Instance()->handleEvent(event);
		gemTest->handleEvent(event);
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
	delete gemTest;


	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyRenderer(m_pRenderer);
	SDL_Quit();
}

