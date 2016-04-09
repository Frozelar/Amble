#include "Tile.h"
#include "Game.h"
#include "Graphics.h"
#include "Level.h"
#include "Texture.h"

Tile::Tile(SDL_Rect* box, int subtype, int unit) : Thing(box, Game::ThingType["tile"], unit)
{
	tgHitboxRect.x = (box == NULL ? ((unit - ((unit / Level::LEVEL_W) * Level::LEVEL_W)) * Game::DEFAULT_W) : box->x);
	tgHitboxRect.y = (box == NULL ? ((unit / Level::LEVEL_W) * Game::DEFAULT_H) : box->y);
	tgHitboxRect.w = Game::DEFAULT_W;
	tgHitboxRect.h = Game::DEFAULT_H;
	tgGFXrect.x = tgHitboxRect.x;
	tgGFXrect.y = tgHitboxRect.y;
	tgGFXrect.w = Game::DEFAULT_W;
	tgGFXrect.h = Game::DEFAULT_H;
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

void Tile::tgResolveCollision(Thing* thing, int dir)
{
	// thing->tgColliding = tgType;
	// thing->tgColDir = dir;

	// tgColliding[dir].thing1 = Game::things[tgLevelUnit];
	tgColliding[dir] = thing->tgLevelUnit;

	if (tiIsSolid)
	{
		if (thing->tgType == Game::ThingType["player"] && dir == Game::Direction["right"])
		{
			// if (thing->tgSpeed > 0)
				// thing->tgSpeed = 0;
			while (Game::checkCollision(thing, Game::things[tgLevelUnit]))
				thing->tgHitboxRect.x--;
			// thing->tgHitboxRect.x++;
		}
		else if (thing->tgType == Game::ThingType["player"] && dir == Game::Direction["left"])
		{
			// if (thing->tgSpeed < 0)
				// thing->tgSpeed = 0;
			while (Game::checkCollision(thing, Game::things[tgLevelUnit]))
				thing->tgHitboxRect.x++;
			// thing->tgHitboxRect.x--;
		}
		else if (thing->tgType == Game::ThingType["enemy"] && dir == Game::Direction["right"])
		{
			while (Game::checkCollision(thing, Game::things[tgLevelUnit]))
				thing->tgHitboxRect.x--;
			// thing->tgHitboxRect.x++;
			// thing->tgDirection = invertDir(thing->tgDirection);
			thing->tgSpeed = -thing->tgSpeed;
		}
		else if (thing->tgType == Game::ThingType["enemy"] && dir == Game::Direction["left"])
		{
			while (Game::checkCollision(thing, Game::things[tgLevelUnit]))
				thing->tgHitboxRect.x++;
			// thing->tgHitboxRect.x--;
			// thing->tgDirection = invertDir(thing->tgDirection);
			thing->tgSpeed = -thing->tgSpeed;
		}
		else if (dir == Game::Direction["down"])
		{
			if (thing->tgVerticals > 0)
				thing->tgVerticals = 0;
			while (Game::checkCollision(thing, Game::things[tgLevelUnit]))
				thing->tgHitboxRect.y--;
			// thing->tgHitboxRect.y++;
		}
		else if (dir == Game::Direction["up"])
		{
			if (thing->tgVerticals < 0)
				thing->tgVerticals = 0;
			while (Game::checkCollision(thing, Game::things[tgLevelUnit]))
				thing->tgHitboxRect.y++;
			// thing->tgHitboxRect.y--;
		}
		else
			std::cout << "Collision error: " << tgLevelUnit << " " << thing->tgLevelUnit << std::endl;
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
	if (!Game::checkCollision(Game::things[tgLevelUnit], NULL, tgLevelUnit, false))
		for (int i = 0; i < tgColliding.size(); i++)
			tgColliding[i] = -1;
}

int Tile::tgGetSubtype(void)
{
	return tiType;
}