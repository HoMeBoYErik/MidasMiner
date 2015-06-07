#include "AudioManager.h"

AudioManager* AudioManager::s_pInstance = 0;


AudioManager::~AudioManager()
{
	// Free every loaded music and sounds chunks
	Mix_CloseAudio();
	Mix_Quit();
}

bool AudioManager::init()
{

	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
		return false; // audio engine init fail
	}

	// Load Background music and puts it to play right away
	GAudioManager::Instance()->loadMusic("assets/music/Soundtrack.mp3", "puzzleMusic");	
	// Load sounds FX
	GAudioManager::Instance()->loadSound("assets/sfx/Combo1.wav", "combo1");
	GAudioManager::Instance()->loadSound("assets/sfx/Combo2.wav", "combo2");
	GAudioManager::Instance()->loadSound("assets/sfx/Combo3.wav", "combo3");
	GAudioManager::Instance()->loadSound("assets/sfx/RemoveChain.wav", "removeChain");
	GAudioManager::Instance()->loadSound("assets/sfx/SwapSuccess.wav", "swapSuccess");
	GAudioManager::Instance()->loadSound("assets/sfx/SwapFailed.wav", "swapFailed"); 

	return true;
}

bool AudioManager::loadMusic(std::string fileName, std::string id)
{
	Mix_Music* pMusic = Mix_LoadMUS(fileName.c_str());
	if (pMusic == 0)
	{
		std::cout << "Could not load music: ERROR - " << Mix_GetError() << std::endl;
		return false;
	}

	// Add the music file in our playlist
	m_musicPlaylist[id] = pMusic;
	
	return true;
}

bool AudioManager::loadSound(std::string fileName, std::string id)
{
	Mix_Chunk* pChunk = Mix_LoadWAV(fileName.c_str());
	if (pChunk == 0)
	{
		std::cout << "Could not load SFX: ERROR - " << Mix_GetError() << std::endl;
		return false;
	}

	m_soundClips[id] = pChunk;
	return true;
}

void AudioManager::playMusic(std::string id, int loop)
{
	Mix_PlayMusic(m_musicPlaylist[id], loop);
}

void AudioManager::pauseMusic()
{
	Mix_PauseMusic();	
}

void AudioManager::resumeMusic()
{
	Mix_ResumeMusic();
}

void AudioManager::stopMusic()
{
	Mix_HaltMusic();
}

void AudioManager::playSound(std::string id, int loop)
{
	Mix_PlayChannel(-1, m_soundClips[id], loop);
}



