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


#include "Thing.h"
#include "Game.h"

Thing::Thing(SDL_Rect* box, int type, int unit)
{
	if (box != NULL)
	{
		tgHitboxRect = *box;
		tgSyncTexture();
	}
	else
		tgHitboxRect = { 0, 0, 0, 0 };
	/*
	else
	{
		tgHitboxRect = new SDL_Rect();
		tgGFXrect = new SDL_Rect();
	}
	*/

	tgColliding.resize(Game::Direction["total"]/* - 1*/);
	for (int i = 0; i < tgColliding.size(); i++)
		tgColliding[i] = -1;

	tgType = type;
	tgLevelUnit = unit;
	tgHealth = Game::DEFAULT_HEALTH;
	tgSpeed = 0;
	tgFrame = 0;
	// tgFrameWaitCounter = 0;
	tgVerticals = 0;
	tgDashing = 0;
	tgSelected = 0;
	// tgColliding = 0;
	// tgColDir = 0;
}

Thing::~Thing()
{
	/*
	if (tgHitboxRect != NULL)
	{
		delete tgHitboxRect;
		tgHitboxRect = NULL;
	}
	if (tgGFXrect != NULL)
	{
		delete tgGFXrect;
		tgHitboxRect = NULL;
	}
	*/
}

void Thing::tgSyncTexture(void)
{
	if (tgType != Game::ThingType["tile"])
		tgGFXrect = { tgHitboxRect.x - Game::DEFAULT_GFX_OFFSET, tgHitboxRect.y - Game::DEFAULT_GFX_OFFSET, 
			tgHitboxRect.w + Game::DEFAULT_GFX_OFFSET * 2, tgHitboxRect.h + Game::DEFAULT_GFX_OFFSET * 2 };
	else // if (tgType == Game::ThingType["tile"])
		tgGFXrect = { tgHitboxRect.x, tgHitboxRect.y, tgHitboxRect.w, tgHitboxRect.h };
}

void Thing::tgHandleVerticals(void)
{
	if (tgVerticals >= (int)Game::gravityArray.size() - 1)
		tgVerticals = Game::gravityArray.size() - 2;
	else if (-tgVerticals >= (int)Game::jumpArray.size() - 1)
		tgVerticals = 0;
	
	if (tgVerticals < 0)
		tgHitboxRect.y -= Game::jumpArray[-(tgVerticals--)];
	else if (tgVerticals > 0)
		tgHitboxRect.y += Game::gravityArray[tgVerticals++];
}

void Thing::tgRender(void)
{
	std::cout << "Error: Executing the rendering of a Thing!" << std::endl;
}

void Thing::tgResolveCollision(Thing* thing, int dir)
{
	if (dir == Game::Direction["left"] || dir == Game::Direction["right"])
	{
		tgSpeed = -tgSpeed;
		// tgDirection = invertDir(tgDirection);
	}
	else if (dir == Game::Direction["up"])
	{
		if (thing->tgVerticals < 0)
			thing->tgVerticals = 1;
	}
	else if (dir == Game::Direction["down"])
	{
		if (thing->tgVerticals > 0)
			thing->tgVerticals = 0;
	}
}

void Thing::tgApplyAI(void)
{
	std::cout << "Error: Applying AI to a Thing!" << std::endl;
}

int Thing::tgGetSubtype(void)
{
	return -1;
}

void Thing::tgSetSubtype(int newst)
{
	std::cout << "Error: Setting subtype of a thing: " << newst << std::endl;
}