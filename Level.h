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

class Level
{
public:
	Level();
	~Level();

	static const int levelW(void);
	static const int levelH(void);
	static bool generateLevel(int);
	static void closeLevel(void);
	static void moveLevel(void);
	static void playMusic(void);

	static int levelTrack;
	static int levelBG;
	static int LEVEL_UNITS;
	static int LEVEL_W;
	static int LEVEL_H;
	static int LEVEL_W_PIXELS;
	static int LEVEL_H_PIXELS;
	static int LEVEL_PIXELS;
	static int gLevelMovementsX;
	static int gLevelMovementsY;
	// including title screen level
	static int totalLevels;
	static int currentLevel;
};