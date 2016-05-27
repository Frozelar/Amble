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

class Thing
{
public:
	// SDL_Rect = hitbox rect; int = thing type; int = thing subtype; int = level unit
	Thing(SDL_Rect* = NULL, int = -1, int = -1);
	~Thing();

	virtual void tgRender(void);

	// Thing* = thing being collided with, int = direction of collision relative to this thing (not the passed thing)
	virtual void tgResolveCollision(Thing*, int);

	// uses default gravity and jumping
	virtual void tgHandleVerticals(void);

	virtual void tgApplyAI(void);

	void tgSyncTexture(void);

	virtual int tgGetSubtype(void);
	virtual void tgSetSubtype(int);

	int tgType;
	int tgHealth;
	int tgSpeed;
	int tgFrame;
	// int tgFrameWaitCounter;
	int tgLevelUnit;
	int tgVerticals;
	int tgDashing;
	// int tgColliding;
	// int tgColDir;
	bool tgSelected;
	// Direction tgDirection;
	SDL_Rect tgHitboxRect;
	SDL_Rect tgGFXrect;
	std::vector<int> tgColliding;	// index for each direction
};