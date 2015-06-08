//
// AudioManager.h

#ifndef __AudioManager__
#define __AudioManager__

#include <iostream>
#include <string>
#include <map>
#include "SDL.h"
#include "SDL_mixer.h"

class AudioManager
{
public:
	static AudioManager* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new AudioManager();
			return s_pInstance;
		}

		return s_pInstance;
	}

	// Init audio system and load audio assets
	bool init();

	/* Resources load */
	// Add a music track file to the playlist
	bool loadMusic(std::string fileName, std::string id);
	// Add a sound fx clip to sound collection
	bool loadSound(std::string fileName, std::string id);
	
	/* Music controls */
	// Play a track in the playlist identified by id, -1 means loop infinite times
	void playMusic(std::string id, int loop);
	// Pause current playing track
	void pauseMusic();
	// Resume current playing track
	void resumeMusic();
	// Stop current playing track
	void stopMusic();
	// Rewind track to beginnig
	void rewindMusic();

	// Play a sound effect
	int playSound(std::string id, int loop);
	// Stop the sound on a particular channel
	void stopChannel(int channel);
	

	

private:
	AudioManager(){};
	~AudioManager();

	AudioManager(const AudioManager&);
	AudioManager& operator=(const AudioManager&);

	// singleton instance reference
	static AudioManager *s_pInstance;

	//The music that will be played
	Mix_Music *gMusic = NULL;
	//The sound effects that will be used
	Mix_Chunk *gScratch = NULL;
	Mix_Chunk *gHigh = NULL;
	Mix_Chunk *gMedium = NULL;
	Mix_Chunk *gLow = NULL;

	std::map<std::string, Mix_Music*> m_musicPlaylist;
	std::map<std::string, Mix_Chunk*> m_soundClips;
};
typedef AudioManager GAudioManager;

#endif /* defined(__AudioManager__)*/

