#include "Audio.h"

Audio Game::gAudio;

std::vector<Mix_Music*> Audio::music(TOTAL_MUSIC_TYPES);
std::vector<Mix_Chunk*> Audio::sfx(TOTAL_SFX_TYPES);

/*
std::string Audio::musicIdentifiers[TOTAL_MUSIC_TYPES] = { "Underground" };
std::string Audio::sfxIdentifiers[TOTAL_SFX_TYPES] = { "Boom" };
*/

Audio::Audio()
{
	if (!auInit())
		std::cout << "Error initializing sounds!" << std::endl;
}

Audio::~Audio()
{
}

bool Audio::auInit()
{
	std::string dir = "resources/";
	std::string ext = ".wav";
	std::string identifier = "";

	// initialize SDL_Mixer for music
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < NULL)
	{
		std::cout << "SDL_Mixer error:" << std::endl;
		std::cout << Mix_GetError() << std::endl;
		return false;
	}

	for (int i = 0; i < musicIdentifiers.size(); i++)
	{
		identifier = dir + musicIdentifiers[i] + ext;
		music.resize(i + 1);
		music[i] = Mix_LoadMUS(identifier.c_str());
	}
	for (int i = 0; i < sfxIdentifiers.size(); i++)
	{
		identifier = dir + sfxIdentifiers[i] + ext;
		sfx.resize(i + 1);
		sfx[i] = Mix_LoadWAV(identifier.c_str());
	}
	return true;
}

void Audio::auPlay(int track)
{
	if (track > MUSIC_STARTING_VALUE && track < MUSIC_ENDING_VALUE)
	{
		Mix_PlayChannel(-1, sfx[track], 0);
	}
	else if (track > SFX_STARTING_VALUE && track < SFX_ENDING_VALUE)
	{
		Mix_PlayMusic(music[track], -1);
	}
}