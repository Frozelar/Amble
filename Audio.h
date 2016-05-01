#pragma once
#include "Include.h"

class Audio
{
public:
	Audio();
	~Audio();

	static bool auInit(void);
	static void auPlay(int, char);
	static void auIncVolume(int, bool = true);

	static int volume[2];
	static const int SFX_VOL;
	static const int MUSIC_VOL;
	static std::vector<Mix_Music*> music;
	static std::vector<Mix_Chunk*> sfx;
	static std::vector<std::string> musicIdentifiers;
	static std::vector<std::string> sfxIdentifiers;
};