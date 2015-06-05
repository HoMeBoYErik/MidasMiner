#include "GameObject.h"
#include "SpriteManager.h"


GameObject::GameObject(Uint8 id, std::string name) : GameObject(id, name, 0.0f, 0.0f){}

GameObject::GameObject(Uint8 id, std::string name, float posX, float posY)
{
	this->id = id;
	this->name = name;
	this->mPosX = posX;
	this->mPosY = posY;
	this->mScaleX = 1.0f;
	this->mScaleY = 1.0f;
	this->mSpriteSizeX = 35;
	this->mSpriteSizeY = 35;

	// Other members init
	isAnimating = false;
	mVelX = mVelY = 0.0f;					// set start velocity to 0
	animOrigX = animOrigY = animDestX = animDestY = 0.0f;	// set animation params to 0
	animTotalTime = 0;					
	animCurrentTime = 0;
	mNormalSpriteId = "";					// sprite when in normal state
	mSelectedSpriteId = "";					// sprite when in selected state (optional)
	isSelected = false;
}

// Destructor and clean
GameObject::~GameObject(){}


void GameObject::setPosition(float posX, float posY)
{
	this->mPosX = posX;
	this->mPosY = posY;
}

void GameObject::setSelected(bool sel)
{
	this->isSelected = sel;
}

void GameObject::setNormalSprite(std::string normalSpriteId)
{
	this->mNormalSpriteId = normalSpriteId;
}

void GameObject::setSelectedSprite(std::string selectedSpriteId)
{
	this->mSelectedSpriteId = selectedSpriteId;
}

void GameObject::update(float timeStep)
{
	if (isAnimating)
	{
		
	}
}

void GameObject::render(SDL_Renderer* pRenderer)
{
	// normal state and has a sprite referenced
	if (!isSelected && !mNormalSpriteId.empty())
	{
		GSpriteManager::Instance()->draw(mNormalSpriteId, (int)mPosX, (int)mPosY, 35, 35, pRenderer, SDL_FLIP_NONE);
	}
	else if (isSelected && !mSelectedSpriteId.empty())
	{
		GSpriteManager::Instance()->draw(mSelectedSpriteId, (int)mPosX, (int)mPosY, 35, 35, pRenderer, SDL_FLIP_NONE);
	}

	// else render nothing for this game object
}

