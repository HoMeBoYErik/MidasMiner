#ifndef __GameObject__
#define __GameObject__

#include "SDL.h"
#include <string>

class GameObject
{
public:
	
	// Attributes
	// Transform component
	Uint8 id;					// game object unique id
	std::string name;			// game object name
	float mPosX, mPosY;			// 2D position | maybe map to a Vector2
	float mScaleX, mScaleY;		// 2D scale
	
	// Rigidbody component
	float mVelX, mVelY;

	// Animator component
	bool isAnimating;
	float animOrigX, animOrigY, animDestX, animDestY;
	float animTotalTime;
	float animCurrentTime;

	// Renderer component
	//TODO change it to uint8 and use a handle table on compile time
	int mSpriteSizeX;					// sprite horizontal pixel size
	int mSpriteSizeY;					// sprite vertical pixel size
	std::string mNormalSpriteId;		// sprite when in normal state
	std::string mSelectedSpriteId;		// sprite when in selected state (optional)
	bool isSelected;

	// Methods
	
	GameObject(Uint8 id, std::string name );	// init GO with just id and name
	GameObject(Uint8 id, std::string name, float posX, float posY); // init GO with a start position
	~GameObject();

	// Methods to interact with components
	// Transform
	void setPosition(float posX, float posY);

	// Animator
	void animate(float origX, float origY, float destX, float destY, float duration);
	
	// Renderer 
	void setSelected(bool sel);
	void setNormalSprite(std::string normalSprite);
	void setSelectedSprite(std::string selectedSprite);

	void update(float timeStep);			// update logic call
	void render(SDL_Renderer* pRenderer);	// render on screen

	void clean();							// clean object and release memory

private:
	GameObject(){};
};

#endif

