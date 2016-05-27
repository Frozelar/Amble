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


#include "Collectible.h"
#include "Game.h"
#include "Graphics.h"
#include "Level.h"
#include "Texture.h"

Collectible::Collectible(SDL_Rect* box, int subtype, int unit) : Thing(box, Game::ThingType["collectible"], unit)
{
	tgHitboxRect.w = Game::DEFAULT_COLLECTIBLE_W;
	tgHitboxRect.h = Game::DEFAULT_COLLECTIBLE_H;
	// tgHitboxRect.x = (box == NULL ? ((unit - ((unit / Level::LEVEL_W) * Level::LEVEL_W)) * Game::DEFAULT_W) : box->x);
	// tgHitboxRect.y = (box == NULL ? ((unit / Level::LEVEL_W) * Game::DEFAULT_H) : box->y);
	tgHitboxRect.x = (box == NULL ? unit * Game::DEFAULT_W / Game::DEFAULT_W : box->x);
	tgHitboxRect.y = (box == NULL ? unit * Game::DEFAULT_H / Game::DEFAULT_H : box->y);
	tgGFXrect.x = tgHitboxRect.x - Game::DEFAULT_GFX_OFFSET;
	tgGFXrect.y = tgHitboxRect.y - Game::DEFAULT_GFX_OFFSET;
	tgGFXrect.w = Game::DEFAULT_ENEMY_W + Game::DEFAULT_GFX_OFFSET * 2;
	tgGFXrect.h = Game::DEFAULT_ENEMY_H + Game::DEFAULT_GFX_OFFSET * 2;
	clType = subtype;
}

void Collectible::tgRender(void)
{
	tgSyncTexture();

	/*
	++tgFrameWaitCounter;
	if (tgFrameWaitCounter > 4)
	{
		tgFrame++;
		tgFrameWaitCounter = 0;
	}
	if (tgFrame >= Game::ObjectFrameType["total"])
	{
		tgFrame = 0;
	}
	*/
	Graphics::collectibleTextures[clType - 1][tgFrame]->txRect = tgGFXrect;
	Graphics::collectibleTextures[clType - 1][tgFrame]->txRender();
}

/*
void Collectible::clCollect(void)
{
	/*
	switch (clType)
	{
	case BIT:
		Game::gScore += 1;
		break;
	case BYTE:
		Game::gScore += 5;
		break;
	}
	


	delete Game::things[tgLevelUnit];
	Game::things[tgLevelUnit] = NULL;
}
*/

void Collectible::tgResolveCollision(Thing* thing, int dir)
{
	// tgColliding[dir].thing1 = Game::things[tgLevelUnit];
	tgColliding[dir] = thing->tgLevelUnit;

	if (thing->tgType == Game::ThingType["player"])
		tgHealth = 0;
}

void Collectible::tgApplyAI(void)
{
	// if (tgHealth == 0)
	//	clCollect();

	if (!Game::checkCollision(Game::things[tgLevelUnit], NULL, tgLevelUnit, false))
		for (int i = 0; i < tgColliding.size(); i++)
			tgColliding[i] = -1;
}

void Collectible::tgHandleVerticals(void)
{
	tgHitboxRect.y += Game::floatArray[tgVerticals++];
	if (tgVerticals >= (int)Game::floatArray.size())
		tgVerticals = 0;
}

int Collectible::tgGetSubtype(void)
{
	return clType;
}

void Collectible::tgSetSubtype(int newst)
{
	clType = newst;
}