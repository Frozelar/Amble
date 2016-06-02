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


#include "Projectile.h"
#include "Game.h"
#include "Graphics.h"
#include "Texture.h"
#include "Thing.h"

Projectile::Projectile(SDL_Rect* rect, int type, int power, int num, int what, int dir)
{
	pjRect = *rect;
	pjType = type;
	pjSpeed = (dir == Game::Direction["left"] ? -Game::DEFAULT_SPEED : Game::DEFAULT_SPEED);
	pjLife = Game::DEFAULT_PROJECTILE_LIFE;
	pjVerticals = 0;
	pjFrame = 0;
	pjNumber = num;
	pjWhatShotIt = what;
	pjPower = power;
}

Projectile::~Projectile()
{

}

void Projectile::pjMove()
{
	// everything handled in lua?
	if (pjLife > 0)
		pjLife--;
	else
		pjLife = 0;

	for (int i = 0; i < Game::things.size(); i++)
		if (Game::things[i] != NULL)
			if (Game::checkCollisionRects(&pjRect, &Game::things[i]->tgHitboxRect))
			{
				pjResolveCollision(i);
				break;
			}
}

void Projectile::pjRender()
{
	Graphics::projectileTextures[pjType][pjFrame]->txRect = pjRect;
	Graphics::projectileTextures[pjType][pjFrame]->txRender();
}

void Projectile::pjResolveCollision(int whichThing)
{
	if ((Game::things[whichThing]->tgType == Game::ThingType["player"] && pjWhatShotIt == Game::ThingType["enemy"]) ||
		(Game::things[whichThing]->tgType == Game::ThingType["enemy"] && pjWhatShotIt == Game::ThingType["player"]))
	{
		Game::things[whichThing]->tgHealth -= pjPower;
		pjLife = 0;
		pjColliding = whichThing;
	}
	else if (Game::things[whichThing]->tgType == Game::ThingType["tile"])
	{
		pjLife = 0;
		pjColliding = whichThing;
	}
}