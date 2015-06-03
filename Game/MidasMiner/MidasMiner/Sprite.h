#ifndef __Sprite__
#define __Sprite__

#include "SDL.h"

class Sprite
{
public:
	Sprite();
	~Sprite();

	void handleEvent(SDL_Event& e);
	void move(float timeStep);
	void render(SDL_Renderer* pRenderer);

private:
	float mPosX, mPosY;
	float mVelX, mVelY;
};

#endif /* defined __Sprite__ */

