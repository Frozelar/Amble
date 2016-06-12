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


#include "Tile.h"
#include "Game.h"
#include "Graphics.h"
#include "Level.h"
#include "Texture.h"

Tile::Tile(SDL_Rect* box, int subtype, int lunit, int tunit) : Thing(box, Game::ThingType["tile"], lunit, tunit)
{
	// tgHitboxRect.x = (box == NULL ? ((unit - ((unit / Level::LEVEL_W) * Level::LEVEL_W)) * Game::DEFAULT_W) : box->x);
	// tgHitboxRect.y = (box == NULL ? ((unit / Level::LEVEL_W) * Game::DEFAULT_H) : box->y);
	tgHitboxRect.x = (box == NULL ? lunit * Game::DEFAULT_W / Game::DEFAULT_W : box->x);
	tgHitboxRect.y = (box == NULL ? lunit * Game::DEFAULT_H / Game::DEFAULT_H : box->y);
	tgHitboxRect.w = Game::DEFAULT_W;
	tgHitboxRect.h = Game::DEFAULT_H;
	tgGFXrect.x = tgHitboxRect.x;
	tgGFXrect.y = tgHitboxRect.y;
	tgGFXrect.w = Game::DEFAULT_W;// +Game::DEFAULT_GFX_OFFSET * 2;
	tgGFXrect.h = Game::DEFAULT_H;// +Game::DEFAULT_GFX_OFFSET * 2;
	tiType = subtype;
}

/*
void Tile::tgResolveCollision(Thing* thing)
{
	SDL_Rect tempRect{ thing->tgHitboxRect.x, thing->tgHitboxRect.y, thing->tgHitboxRect.w, thing->tgHitboxRect.h };
	int xInc = 0, xDec = 0, yInc = 0, yDec = 0;

	while (Game::checkCollisionRects(tgHitboxRect, tempRect))
	{
		tempRect.x--;
		xDec++;
	}
	while (Game::checkCollisionRects(tgHitboxRect, tempRect))
	{
		tempRect.x++;
		xInc++;
	}
	while (Game::checkCollisionRects(tgHitboxRect, tempRect))
	{
		tempRect.y--;
		yDec++;
	}
	while (Game::checkCollisionRects(tgHitboxRect, tempRect))
	{
		tempRect.y++;
		yInc++;
	}

	if (xInc < xDec && xInc < yInc && xInc < yDec)
		thing->tgHitboxRect.x += xInc;
	else if (xDec < xInc && xDec < yInc && xDec < yDec)
		thing->tgHitboxRect.x -= xDec;
	else if (yInc <= xInc && yInc <= xDec && yInc <= yDec)
		thing->tgHitboxRect.y += yInc;
	else if (yDec <= xInc && yDec <= xDec && yDec <= yInc)
		thing->tgHitboxRect.y -= yDec;
}
*/

void Tile::tgResolveCollision(int thingsunit, int dir)
{
	// thing->tgColliding = tgType;
	// thing->tgColDir = dir;

	// tgColliding[dir].thing1 = Game::things[tgLevelUnit];
	// tgColliding[dir] = thing->tgLevelUnit;
	// int direction = invertDir(dir);
	Game::things[thingsunit]->tgColliding[dir] = tgThingsUnit;

	if (tiIsSolid)
	{
		if (Game::things[thingsunit]->tgType == Game::ThingType["player"] && dir == Game::Direction["right"])
		{
			// if (thing->tgSpeed > 0)
				// thing->tgSpeed = 0;
			while (Game::checkCollision(Game::things[thingsunit], this))
				Game::things[thingsunit]->tgHitboxRect.x--;
			// thing->tgHitboxRect.x++;
		}
		else if (Game::things[thingsunit]->tgType == Game::ThingType["player"] && dir == Game::Direction["left"])
		{
			// if (thing->tgSpeed < 0)
				// thing->tgSpeed = 0;
			while (Game::checkCollision(Game::things[thingsunit], this))
				Game::things[thingsunit]->tgHitboxRect.x++;
			// thing->tgHitboxRect.x--;
		}
		else if (Game::things[thingsunit]->tgType == Game::ThingType["enemy"] && dir == Game::Direction["right"])
		{
			while (Game::checkCollision(Game::things[thingsunit], this))
				Game::things[thingsunit]->tgHitboxRect.x--;
			// thing->tgHitboxRect.x++;
			// thing->tgDirection = invertDir(thing->tgDirection);
			Game::things[thingsunit]->tgSpeed = -Game::things[thingsunit]->tgSpeed;
		}
		else if (Game::things[thingsunit]->tgType == Game::ThingType["enemy"] && dir == Game::Direction["left"])
		{
			while (Game::checkCollision(Game::things[thingsunit], this))
				Game::things[thingsunit]->tgHitboxRect.x++;
			// thing->tgHitboxRect.x--;
			// thing->tgDirection = invertDir(thing->tgDirection);
			Game::things[thingsunit]->tgSpeed = -Game::things[thingsunit]->tgSpeed;
		}
		else if (dir == Game::Direction["down"])
		{
			if (Game::things[thingsunit]->tgVerticals > 0)
				Game::things[thingsunit]->tgVerticals = 0;
			while (Game::checkCollision(Game::things[thingsunit], this))
				Game::things[thingsunit]->tgHitboxRect.y--;
			// thing->tgHitboxRect.y++;
		}
		else if (dir == Game::Direction["up"])
		{
			if (Game::things[thingsunit]->tgVerticals < 0)
				Game::things[thingsunit]->tgVerticals = 0;
			while (Game::checkCollision(Game::things[thingsunit], this))
				Game::things[thingsunit]->tgHitboxRect.y++;
			// thing->tgHitboxRect.y--;
		}
		else
			std::cout << "Collision error: " << tgLevelUnit << " " << Game::things[thingsunit]->tgLevelUnit << std::endl;
	}
}

void Tile::tgRender(void)
{
	tgSyncTexture();

	/*
	if (tiIsAnimated)
	{
		++tgFrameWaitCounter;
		if (tgFrameWaitCounter > 4)
		{
			tgFrame++;
			tgFrameWaitCounter = 0;
		}
		if (tgFrame >= TOTAL_OBJECT_FRAME_TYPES)
		{
			tgFrame = 0;
		}
	}
	else
	{
		tgFrame = 0;
	}
	*/

	Graphics::tileTextures[tiType - 1][tgFrame]->txRect = tgGFXrect;
	Graphics::tileTextures[tiType - 1][tgFrame]->txRender();
}

void Tile::tgApplyAI(void)
{
	if (!Game::checkCollision(this, NULL, tgLevelUnit, false))
		for (int i = 0; i < tgColliding.size(); i++)
			tgColliding[i] = -1;
}

int Tile::tgGetSubtype(void)
{
	return tiType;
}

void Tile::tgSetSubtype(int newst)
{
	tiType = newst;
}