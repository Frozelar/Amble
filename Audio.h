#pragma once
#include "Include.h"

class Audio
{
public:
	Audio();
	~Audio();

	bool auInit(void);

	void auPlay(int, char);

	static std::vector<Mix_Music*> music;
	static std::vector<Mix_Chunk*> sfx;
	static std::vector<std::string> musicIdentifiers;
	static std::vector<std::string> sfxIdentifiers;
};