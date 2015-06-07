#ifndef __GameObject__
#define __GameObject__

#include "SDL.h"
#include <string>
#include <array>

class GameObject
{
public:
	
	// Attributes
	// Transform component
	Uint8 id;					// game object unique id
	
	float mPosX, mPosY;			// 2D position | maybe map to a Vector2
	float mScaleX, mScaleY;		// 2D scale
	
	// Rigidbody component
	float mVelX, mVelY;

	// Animator component
	bool isAnimating;
	float animOrigX, animOrigY, animDestX, animDestY;
	float animOrigScaleX, animOrigScaleY, animDestScaleX, animDestScaleY;
	float animTotalTime;
	float animCurrentTime;
	bool doBounce; // if true animate the movement with a bounce effect

	bool isAnimatingScale;
	float animScaleTotalTime;
	float animScaleCurrentTime;

	// Renderer component
	//TODO change it to uint8 and use a handle table on compile time
	bool isVisible;
	int mSpriteSizeX;					// sprite horizontal pixel size
	int mSpriteSizeY;					// sprite vertical pixel size
	int  mNormalSpriteId;			// sprite when in normal state
	int  mSelectedSpriteId;			// sprite when in selected state (optional)
	bool isSelected;

	// Methods
	
	GameObject(Uint8 id );	// init GO with just id 
	GameObject(Uint8 id, float posX, float posY); // init GO with a start position
	~GameObject();

	// Methods to interact with components
	// Transform
	void setPosition(float posX, float posY);
	void setScale(float scaleX, float scaleY);

	// Animator
	void animate(float origX, float origY, float destX, float destY, float duration, float delay = 0);
	void animateScale(float origX, float origY, float destX, float destY, float duration);
	void animateBounce(float origX, float origY, float destX, float destY, float duration, float delay = 0);
	
	// Renderer 
	void setSelected(bool sel);
	void setNormalSprite(int normalSprite);
	void setSelectedSprite(int selectedSprite);
	void setSprites(Uint8 gemType);

	void update(float timeStep);			// update logic call
	void render(SDL_Renderer* pRenderer);	// render on screen

	void clean();							// clean object and release memory

private:
	GameObject(){};
};

#endif

