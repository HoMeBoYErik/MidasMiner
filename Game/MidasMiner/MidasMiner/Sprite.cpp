#include "Sprite.h"
#include "Constants.h"
#include "SpriteManager.h"




Sprite::Sprite()
{
	mPosX = 0;
	mPosY = 0;
	mVelX = 0;
	mVelY = 0;
}


Sprite::~Sprite()
{
}

void Sprite::handleEvent(SDL_Event& e)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0 )
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
			case SDLK_UP: mVelY -= 200; break;
			case SDLK_DOWN: mVelY += 200; break;
			case SDLK_LEFT: mVelX -= 200; break;
			case SDLK_RIGHT: mVelX += 200; break;
		}
	}
	//If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0 )
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
			case SDLK_UP: mVelY += 200; break;
			case SDLK_DOWN: mVelY -= 200; break;
			case SDLK_LEFT: mVelX += 200; break;
			case SDLK_RIGHT: mVelX -= 200; break;
		}
	}
}

void Sprite::move(float timeStep)
{
	// Move the sprite left or right
	mPosX += mVelX * timeStep;

	// Move the sprite up or down
	mPosY += mVelY * timeStep;

	// Avoid sprite exiting the screen
	if (mPosX < 0)
	{
		mPosX = 0;
	}
	else if (mPosX > SCREEN_WIDTH_RESOLUTION - DEFAULT_SPRITE_SIZE)
	{
		mPosX = SCREEN_WIDTH_RESOLUTION - DEFAULT_SPRITE_SIZE;
	}

	// Avoid sprite exiting the screen
	if (mPosY < 0)
	{
		mPosY = 0;
	}
	else if (mPosY > SCREEN_HEIGHT_RESOLUTION - DEFAULT_SPRITE_SIZE)
	{
		mPosY = SCREEN_HEIGHT_RESOLUTION - DEFAULT_SPRITE_SIZE;
	}

	
}

void Sprite::render(SDL_Renderer* pRenderer)
{
	GSpriteManager::Instance()->draw("redGem", (int)mPosX, (int)mPosY, 35, 35, pRenderer, SDL_FLIP_NONE);
}
