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

class Enemy : public Thing
{
public:
	// initialize everything
	Enemy(SDL_Rect* = NULL, int = -1, int = -1);

	// apply AI to the enemy
	void tgApplyAI(void);

	// render the enemy
	void tgRender(void);

	// Thing* = what it is colliding with, Direction = which side of the Thing* the collision occurred at
	void tgResolveCollision(Thing*, int);

	int tgGetSubtype(void);
	void tgSetSubtype(int);

	// the type of enemy
	int enType = 0;
	int enPower = 5;
};