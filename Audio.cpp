/*
Amble: a platformer game being made as a hobby.
Copyright (C) 2015, 2016 Frozelar

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "Audio.h"
#include "Game.h"

Audio* Game::gAudio;

int Audio::volume[2] = { 100, 100 };	// sfx, music
const int Audio::SFX_VOL_INDEX = 0;
const int Audio::MUSIC_VOL_INDEX = 1;
std::vector<Mix_Music*> Audio::music;// (Game::MusicType["total"]);
std::vector<Mix_Chunk*> Audio::sfx;// (Game::SoundEffectType["total"]);
std::vector<std::string> Audio::musicIdentifiers;
std::vector<std::string> Audio::sfxIdentifiers;

/*
std::string Audio::musicIdentifiers[TOTAL_MUSIC_TYPES] = { "Underground" };
std::string Audio::sfxIdentifiers[TOTAL_SFX_TYPES] = { "Boom" };
*/

Audio::Audio()
{
	music.resize(Game::MusicType["total"]);
	sfx.resize(Game::SoundEffectType["total"]);
	if (!auInit())
		std::cout << "Error initializing sounds!" << std::endl;
}

Audio::~Audio()
{
	for (int i = 0; i < music.size(); i++)
	{
		Mix_FreeMusic(music[i]);
		music[i] = NULL;
	}
	for (int i = 0; i < sfx.size(); i++)
	{
		Mix_FreeChunk(sfx[i]);
		sfx[i] = NULL;
	}
}

bool Audio::auInit()
{
	std::string dir = "resources/";
	std::string ext = ".wav";
	std::string identifier = "";

	// initialize SDL_Mixer for music
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < NULL)
	{
		std::cout << "SDL_Mixer error: " << std::endl;
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

void Audio::auPlay(int track, char type)
{
	if (type == 's')
		Mix_PlayChannel(-1, sfx[track], 0);
	else if (type == 'm')
		Mix_PlayMusic(music[track], -1);
}

void Audio::auIncVolume(int what, bool incvol)
{
	if (incvol)
	{
		if (volume[what] >= 100)
			volume[what] = 0;
		else
			volume[what] += 10;
	}
	if (what == SFX_VOL_INDEX)
		for (int i = 0; i < sfx.size(); i++)
			Mix_VolumeChunk(sfx[i], volume[what]);
			// sfx[i]->volume = volume[what];
	else if (what == MUSIC_VOL_INDEX)
		// Mix_Volume(-1, volume[what]);
		Mix_VolumeMusic(volume[what]);
}