//
//  SpriteManager.cpp

#include "SpriteManager.h"
#include "SDL_image.h"

SpriteManager* SpriteManager::s_pInstance = 0;

bool SpriteManager::load(std::string fileName, std::string id, SDL_Renderer* pRenderer)
{
	SDL_Surface* pTempSurface = IMG_Load(fileName.c_str());

	if (pTempSurface == 0)
	{
#ifdef _DEBUG
		std::cout << IMG_GetError();
#endif;
		return false;
	}

	SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRenderer, pTempSurface);

	SDL_FreeSurface(pTempSurface);

	if (pTexture != 0)
	{
		m_textureMap[id] = pTexture;
		return true;
	}

	// something went wrong
	return false;
}

bool SpriteManager::init(SDL_Renderer* pRenderer)
{
	// TODO add check error control
	GSpriteManager::Instance()->load("assets/sprites/BackGround.jpg", "background", pRenderer);
	// Loading gems image assets
	GSpriteManager::Instance()->load("assets/sprites/Blue.png", "blueGem", pRenderer);
	GSpriteManager::Instance()->load("assets/sprites/Green.png", "greenGem", pRenderer);
	GSpriteManager::Instance()->load("assets/sprites/Purple.png", "purpleGem", pRenderer);
	GSpriteManager::Instance()->load("assets/sprites/Red.png", "redGem", pRenderer);
	GSpriteManager::Instance()->load("assets/sprites/Yellow.png", "yellowGem", pRenderer);
	// Loading gems in selected state assets
	GSpriteManager::Instance()->load("assets/sprites/BlueSelected.png", "blueGemSelected", pRenderer);
	GSpriteManager::Instance()->load("assets/sprites/GreenSelected.png", "greenGemSelected", pRenderer);
	GSpriteManager::Instance()->load("assets/sprites/PurpleSelected.png", "purpleGemSelected", pRenderer);
	GSpriteManager::Instance()->load("assets/sprites/RedSelected.png", "redGemSelected", pRenderer);
	GSpriteManager::Instance()->load("assets/sprites/YellowSelected.png", "yellowGemSelected", pRenderer);

	GSpriteManager::Instance()->defaultSpriteId = std::string("blueGem");

	
	return true;
}

void SpriteManager::draw(std::string id, int x, int y, int width, int height, SDL_Renderer* pRenderer, SDL_RendererFlip flip)
{
	SDL_Rect srcRect;
	SDL_Rect destRect;

	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = destRect.w = width;
	srcRect.h = destRect.h = height;
	destRect.x = x;
	destRect.y = y;

	SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect, &destRect, 0, 0, flip);
}

void SpriteManager::drawCropped(std::string id, int startX, int startY, int x, int y, int width, int height, SDL_Renderer* pRenderer, SDL_RendererFlip flip)
{
	SDL_Rect srcRect;
	SDL_Rect destRect;

	srcRect.x = startX;
	srcRect.y = startY;
	srcRect.w = destRect.w = width;
	srcRect.h = destRect.h = height;
	destRect.x = x;
	destRect.y = y;

	SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect, &destRect, 0, 0, flip);
}

void SpriteManager::drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer *pRenderer, double angle, int alpha, SDL_RendererFlip flip)
{
	SDL_Rect srcRect;
	SDL_Rect destRect;
	srcRect.x = width * currentFrame;
	srcRect.y = height * currentRow;
	srcRect.w = destRect.w = width;
	srcRect.h = destRect.h = height;
	destRect.x = x;
	destRect.y = y;

	SDL_SetTextureAlphaMod(m_textureMap[id], alpha);
	SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect, &destRect, angle, 0, flip);
}

void SpriteManager::drawTile(std::string id, int margin, int spacing, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer *pRenderer)
{
	SDL_Rect srcRect;
	SDL_Rect destRect;
	srcRect.x = margin + (spacing + width) * currentFrame;
	srcRect.y = margin + (spacing + height) * currentRow;
	srcRect.w = destRect.w = width;
	srcRect.h = destRect.h = height;
	destRect.x = x;
	destRect.y = y;

	SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect, &destRect, 0, 0, SDL_FLIP_NONE);
}


void SpriteManager::clearTextureMap()
{
	m_textureMap.clear();
}

void SpriteManager::clearFromTextureMap(std::string id)
{
	m_textureMap.erase(id);
}

