//
//  SpriteManager.h

#ifndef __SpriteManager__
#define __SpriteManager__

#include <iostream>
#include <string>
#include <map>
#include "SDL.h"

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
	bool load(std::string fileName, std::string id, SDL_Renderer* pRenderer);

	void clearTextureMap();
	void clearFromTextureMap(std::string id);

	void draw(std::string id, int x, int y, int width, int height, SDL_Renderer* pRenderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void drawCropped(std::string id, int startX, int startY, int x, int y, int width, int height, SDL_Renderer* pRenderer, SDL_RendererFlip flip);
	void drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer* pRenderer, double angle, int alpha, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void drawTile(std::string id, int margin, int spacing, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer *pRenderer);

	std::map<std::string, SDL_Texture*> getTextureMap() { return m_textureMap; }

	std::string defaultSpriteId;

private:

	SpriteManager() {};
	~SpriteManager() {};

	SpriteManager(const SpriteManager&);
	SpriteManager& operator=(const SpriteManager&);

	std::map<std::string, SDL_Texture*> m_textureMap;

	static SpriteManager* s_pInstance;
};

typedef SpriteManager GSpriteManager;

#endif /* defined(__SpriteManager__) */
