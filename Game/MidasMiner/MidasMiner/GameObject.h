/* Game Object Class */
/* With very little effort it can be converted use the composition pattern */
/* See comments*/

#ifndef __GameObject__
#define __GameObject__

#include "SDL.h"
#include <string>
#include <array>

class GameObject
{
public:
	
	// Attributes
	// Transform component (position, scale)
	Uint8 id;					// game object unique id
	
	float mPosX, mPosY;			// 2D position | maybe map to a Vector2
	float mScaleX, mScaleY;		// 2D scale
	
	// Rigidbody component
	float mVelX, mVelY;		    // not used, but if any physics simulation is necessary...

	// Animator component
	bool isAnimating;
	float animOrigX, animOrigY, animDestX, animDestY; // the origin of the next animation
	float animOrigScaleX, animOrigScaleY, animDestScaleX, animDestScaleY; // the destination of the next animation
	float animTotalTime; // total duration of next animation
	float animCurrentTime; // current animation time. if initialized to values < 0 it simulates a delay
	bool doBounce; // if true animate the movement with a bounce effect
	bool hasLanded; // bool to determine when to reproduce bounce sound effect
	
	bool isAnimatingScale; // wanted to simulate scale to remove chain from board but still to implemente correctly due to sprite pivots
	float animScaleTotalTime;
	float animScaleCurrentTime;

	// Renderer component	
	bool isVisible;					// during render the object is shown only if isVisible = true
	int mSpriteSizeX;				// sprite horizontal pixel size
	int mSpriteSizeY;				// sprite vertical pixel size
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

