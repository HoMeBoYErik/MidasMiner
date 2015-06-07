//
//  SpriteManager.h

#ifndef __SpriteManager__
#define __SpriteManager__

#include <iostream>
#include <string>
#include <map>
#include "SDL.h"

enum sprite_assets
{
	BACKGROUND = 1,
	BLUE_GEM = 2,
	BLUE_GEM_SELECTED = 3,
	GREEN_GEM = 4,
	GREEN_GEM_SELECTED = 5,
	PURPLE_GEM = 6,
	PURPLE_GEM_SELECTED = 7,
	RED_GEM = 8,
	RED_GEM_SELECTED = 9,
	YELLOW_GEM = 10,
	YELLOW_GEM_SELECTED = 11
};


class SpriteManager
{
public:

	static SpriteManager* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new SpriteManager();
			return s_pInstance;
		}

		return s_pInstance;
	}

	bool init(SDL_Renderer* pRenderer);
	bool load(std::string fileName, int id, SDL_Renderer* pRenderer);

	void clearTextureMap();
	void clearFromTextureMap(int id);

	void draw(int id, int x, int y, int width, int height, SDL_Renderer* pRenderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void drawCropped(int id, int startX, int startY, int x, int y, int width, int height, SDL_Renderer* pRenderer, SDL_RendererFlip flip);
	void drawFrame(int id, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer* pRenderer, double angle, int alpha, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void drawTile(int id, int margin, int spacing, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer *pRenderer);

	std::map<int, SDL_Texture*> getTextureMap() { return m_textureMap; }

	

private:

	SpriteManager() {};
	~SpriteManager() {};

	SpriteManager(const SpriteManager&);
	SpriteManager& operator=(const SpriteManager&);

	std::map<int, SDL_Texture*> m_textureMap;

	static SpriteManager* s_pInstance;
};

typedef SpriteManager GSpriteManager;

#endif /* defined(__SpriteManager__) */
