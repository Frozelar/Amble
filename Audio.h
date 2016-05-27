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
	static const int SFX_VOL_INDEX;
	static const int MUSIC_VOL_INDEX;
	static std::vector<Mix_Music*> music;
	static std::vector<Mix_Chunk*> sfx;
	static std::vector<std::string> musicIdentifiers;
	static std::vector<std::string> sfxIdentifiers;
};