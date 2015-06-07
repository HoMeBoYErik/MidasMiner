#include "GameObject.h"
#include "Constants.h"
#include "SpriteManager.h"
#include "Quad.h"
#include "Back.h"
#include "Bounce.h"
#include "GameManager.h"


GameObject::GameObject(Uint8 id) : GameObject(id, 0.0f, 0.0f){}

GameObject::GameObject(Uint8 id, float posX, float posY)
{
	this->id = id;	
	this->mPosX = posX;
	this->mPosY = posY;
	this->mScaleX = 1.0f;
	this->mScaleY = 1.0f;
	this->mSpriteSizeX = DEFAULT_SPRITE_SIZE;
	this->mSpriteSizeY = DEFAULT_SPRITE_SIZE;

	// Other members init
	isAnimating = false;
	isAnimatingScale = false;
	doBounce = false;
	mVelX = mVelY = 0.0f;					// set start velocity to 0
	animOrigX = animOrigY = animDestX = animDestY = 0.0f;	// set animation params to 0
	animOrigScaleX = animOrigScaleY = animDestScaleX = animDestScaleY = 1.0f;	// set animation scale params to 1
	animTotalTime = 0;					
	animCurrentTime = 0;
	animScaleTotalTime = 0;
	animScaleCurrentTime = 0;
	
	mNormalSpriteId = 2;					// sprite when in normal state
	mSelectedSpriteId = 3;					// sprite when in selected state (optional)
	isSelected = false;
	isVisible = true;
}

// Destructor and clean
GameObject::~GameObject(){}


void GameObject::setPosition(float posX, float posY)
{
	this->mPosX = posX;
	this->mPosY = posY;
}

void GameObject::setScale(float scaleX, float scaleY)
{
	this->mScaleX = scaleX;
	this->mScaleY = scaleY;
}

void GameObject::setSelected(bool sel)
{
	isSelected = sel;
}

void GameObject::setNormalSprite(int normalSpriteId)
{
	mNormalSpriteId = normalSpriteId;
}

void GameObject::setSelectedSprite(int selectedSpriteId)
{
	mSelectedSpriteId = selectedSpriteId;
}

void GameObject::setSprites(Uint8 gemType)
{
	switch (gemType)
	{
	case board_elems::BLUE_GEM_ELEM :
			setNormalSprite(sprite_assets::BLUE_GEM);
			setSelectedSprite(sprite_assets::BLUE_GEM_SELECTED);
		break;

	case board_elems::GREEN_GEM_ELEM:
			setNormalSprite(sprite_assets::GREEN_GEM);
			setSelectedSprite(sprite_assets::GREEN_GEM_SELECTED);
		break;

	case board_elems::PURPLE_GEM_ELEM:
			setNormalSprite(sprite_assets::PURPLE_GEM);
			setSelectedSprite(sprite_assets::PURPLE_GEM_SELECTED);
		break;

	case board_elems::RED_GEM_ELEM:
			setNormalSprite(sprite_assets::RED_GEM);
			setSelectedSprite(sprite_assets::RED_GEM_SELECTED);
		break;

	case board_elems::YELLOW_GEM_ELEM:
		setNormalSprite(sprite_assets::YELLOW_GEM);
		setSelectedSprite(sprite_assets::YELLOW_GEM_SELECTED);
		break;

	default:
		break;
	}
}

void GameObject::animate(float origX, float origY, float destX, float destY, float duration, float delay)
{
	animOrigX = origX;
	animOrigY = origY;
	animDestX = destX;
	animDestY = destY;
	animTotalTime = duration;
	isAnimating = true;
	animCurrentTime = 0 - delay;
}

void GameObject::animateBounce(float origX, float origY, float destX, float destY, float duration, float delay)
{
	doBounce = true;
	animate(origX, origY, destX, destY, duration, delay);
}

void GameObject::animateScale(float origX, float origY, float destX, float destY, float duration)
{
	animOrigScaleX = origX;
	animOrigScaleY = origY;
	animDestScaleX = destX;
	animDestScaleY = destY;
	animScaleTotalTime = duration;
	isAnimatingScale = true;
	animScaleCurrentTime = 0;
}

void GameObject::update(float timeStep)
{
	if (isAnimating)
	{
		animCurrentTime += timeStep;
		// animCurrentTime may be less then zero if animation start with a delay
		
		if (animCurrentTime >= 0)
		{		
			if (doBounce)
			{
				setPosition(Bounce::easeOut(animCurrentTime, animOrigX, animDestX - animOrigX, animTotalTime),
					Bounce::easeOut(animCurrentTime, animOrigY, animDestY - animOrigY, animTotalTime));
			}
		
			else
			{
				setPosition(Quad::easeInOut(animCurrentTime, animOrigX, animDestX - animOrigX, animTotalTime),
					Quad::easeInOut(animCurrentTime, animOrigY, animDestY - animOrigY, animTotalTime));
			}
		}
		

		if (animCurrentTime >= animTotalTime)
		{
			//std::cout << "[" << static_cast<unsigned int>(id) << "] end animation" << std::endl;
			isAnimating = false;
			if (doBounce){ doBounce = false; }//reset bounce effect to false
			animCurrentTime = 0;
			//std::cout << " Animation ended for gameobject with id " << static_cast<unsigned int>(this->id) << std::endl;
			//clamp values to destination
			setPosition(animDestX, animDestY);

			// Notify end of animation to Game Manager
			GGameManager::Instance()->endedAnimation();
		}
	}
	if (isAnimatingScale)
	{
		animScaleCurrentTime += timeStep;
		setScale(Back::easeInOut(animScaleCurrentTime, animOrigScaleX, animDestScaleX - animOrigScaleX, animScaleTotalTime),
			     Back::easeInOut(animScaleCurrentTime, animOrigScaleY, animDestScaleY - animOrigScaleY, animScaleTotalTime));

		if (animScaleCurrentTime >= animScaleTotalTime)
		{
			isAnimatingScale = false;
			animScaleCurrentTime = 0;
			setScale(animDestScaleX, animDestScaleY);

			// Notify end of animation to Game Manager
			GGameManager::Instance()->endedAnimation();
		}		
	}
}

void GameObject::render(SDL_Renderer* pRenderer)
{
	if (isVisible)
	{
		// normal state and has a sprite referenced
		if (!isSelected )
		{
			GSpriteManager::Instance()->draw(mNormalSpriteId, (int)mPosX, (int)mPosY, (int)( mSpriteSizeX * mScaleX), (int)( mSpriteSizeY * mScaleY), pRenderer, SDL_FLIP_NONE);
		}
		else if (isSelected )
		{
			GSpriteManager::Instance()->draw(mSelectedSpriteId, (int)mPosX, (int)mPosY, (int)(mSpriteSizeX * mScaleX),(int)( mSpriteSizeY * mScaleY), pRenderer, SDL_FLIP_NONE);
		}

		// else render nothing for this game object
	}
	
}

