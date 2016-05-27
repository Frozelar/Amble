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

class Particle
{
public:
	// ptRect, ptType, ptNum, ptDestination, ptLife, ptSpeed
	Particle(SDL_Rect*, int, int, SDL_Point* = NULL, int = -1, int = -1, int = -1);
	~Particle();

	void ptMove(void);

	void ptRender(void);

	// void ptDestroy(void);

	int ptType = 0;
	// int ptLifeSpan = 0;
	int ptSpeedX = 0;
	int ptSpeedY = 0;
	int ptAlpha = 255;
	int ptFrame = 0;
	int ptFrameWaitCounter = 1;
	int ptNumber = -1;
	int ptLife = 1;
	SDL_Point ptDestination;
	SDL_Rect ptRect;
};

