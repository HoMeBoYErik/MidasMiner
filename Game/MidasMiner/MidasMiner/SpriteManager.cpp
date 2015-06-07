//
//  SpriteManager.cpp

#include "SpriteManager.h"
#include "SDL_image.h"


SpriteManager* SpriteManager::s_pInstance = 0;



bool SpriteManager::load(std::string fileName, int id, SDL_Renderer* pRenderer)
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

void SpriteManager::updateScore(int newScore)
{
	std::string score = "Score: ";
	score += std::to_string(newScore);
	updateScoreText(score, mScoreColor, pRenderer);
}

void SpriteManager::updateScoreText(std::string textureText, SDL_Color textColor, SDL_Renderer* pRenderer)
{
	SDL_Surface* pTextSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);

	if (pTextSurface == NULL)
	{
		return;
	}

	scoreTextWidth = pTextSurface->w;
	scoreTextHeight = pTextSurface->h;

	scoreText = SDL_CreateTextureFromSurface(pRenderer, pTextSurface);
	//Enable blending on texture
	//SDL_SetTextureBlendMode(scoreText, SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(pTextSurface);

	m_textureMap[sprite_assets::SCORE_TEXT] = scoreText;
}

bool SpriteManager::init(SDL_Renderer* pRenderer)
{
	GSpriteManager::Instance()->pRenderer = pRenderer;
	// Init SDL TTF library
	TTF_Init();

	gFont = TTF_OpenFont("assets/fonts/Lobster.ttf", 28);

	mScoreColor = { 255, 255, 255 };

	if (gFont == NULL)
	{
#ifdef _DEBUG
		std::cout << SDL_GetError() << std::endl;
#endif
		return false;
	}
	SDL_Color textColor = { 255, 255, 255, 255 };

	
	GSpriteManager::Instance()->load("assets/sprites/BackGround.jpg", sprite_assets::BACKGROUND , pRenderer);
	// Loading gems image assets
	GSpriteManager::Instance()->load("assets/sprites/Blue.png", sprite_assets::BLUE_GEM, pRenderer);
	GSpriteManager::Instance()->load("assets/sprites/Green.png", sprite_assets::GREEN_GEM, pRenderer);
	GSpriteManager::Instance()->load("assets/sprites/Purple.png", sprite_assets::PURPLE_GEM, pRenderer);
	GSpriteManager::Instance()->load("assets/sprites/Red.png", sprite_assets::RED_GEM, pRenderer);
	GSpriteManager::Instance()->load("assets/sprites/Yellow.png", sprite_assets::YELLOW_GEM, pRenderer);
	// Loading gems in selected state assets
	GSpriteManager::Instance()->load("assets/sprites/BlueSelected.png", sprite_assets::BLUE_GEM_SELECTED, pRenderer);
	GSpriteManager::Instance()->load("assets/sprites/GreenSelected.png", sprite_assets::GREEN_GEM_SELECTED, pRenderer);
	GSpriteManager::Instance()->load("assets/sprites/PurpleSelected.png", sprite_assets::PURPLE_GEM_SELECTED, pRenderer);
	GSpriteManager::Instance()->load("assets/sprites/RedSelected.png", sprite_assets::RED_GEM_SELECTED, pRenderer);
	GSpriteManager::Instance()->load("assets/sprites/YellowSelected.png", sprite_assets::YELLOW_GEM_SELECTED, pRenderer);
	
	GSpriteManager::Instance()->updateScoreText("Score: 0", textColor, pRenderer);
	
	
	return true;
}

void SpriteManager::draw(int id, int x, int y, int width, int height, SDL_Renderer* pRenderer, SDL_RendererFlip flip)
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

void SpriteManager::drawScoreText(SDL_Renderer* pRenderer)
{
	draw(sprite_assets::SCORE_TEXT, 10, 10, scoreTextWidth, scoreTextHeight, pRenderer, SDL_FLIP_NONE);
}

void SpriteManager::drawCropped(int id, int startX, int startY, int x, int y, int width, int height, SDL_Renderer* pRenderer, SDL_RendererFlip flip)
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

void SpriteManager::drawFrame(int id, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer *pRenderer, double angle, int alpha, SDL_RendererFlip flip)
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

void SpriteManager::drawTile(int id, int margin, int spacing, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer *pRenderer)
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

void SpriteManager::clearFromTextureMap(int id)
{
	m_textureMap.erase(id);
}

void SpriteManager::clean()
{
	TTF_CloseFont(gFont);
	gFont = NULL;
	
	free(scoreText);
	free(timeText);

	scoreText = NULL;
	timeText = NULL;

	clearTextureMap();
}

