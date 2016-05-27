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

class Tile : public Thing
{
public:
	// initialize everything
	Tile(SDL_Rect* = NULL, int = -1, int = -1);

	// destroy texture
	// ~Tile();

	void tgResolveCollision(Thing*, int);
	void tgRender(void);
	void tgApplyAI(void);
	int tgGetSubtype(void);
	void tgSetSubtype(int);

	int tiType = 0;
	bool tiIsSolid = true;
	bool tiIsAnimated = false;
};