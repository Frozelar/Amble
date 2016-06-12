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
#include "Thing.h"

class Player : public Thing
{
public:
	// set properties
	Player(SDL_Rect* = NULL, int = -1, int = -1);

	// destroy texture
	// ~Player();

	// check to see if the input has to do with the player (returns whether it did or not)
	void plHandleEvent(SDL_Event*);

	// moves the player and checks collision (takes a moveType, or -1 if the direction is built in to the speed)
	void plMove(void);

	void tgRender(void);

	void tgHandleVerticals(void);

	void plHandleDashing(void);

	void tgResolveCollision(int, int);

	int plJumps = 0;
	int plActionCounter = 0;
	int plActionFrames = 0;
	int plState = 0;
	int plPower = 1;
	int plOldVerticals = 0;
	SDL_Rect plOldHitboxRect;
	// ControlScheme plControls;
	std::map< std::string, int > plControls;
};