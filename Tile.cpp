#include "Tile.h"

Tile::Tile(SDL_Rect* box, int subtype, int unit) : Thing(box, TILE, unit)
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

void Tile::tgResolveCollision(Thing* thing, Direction dir)
{
	if (tiIsSolid)
	{
		if (thing->tgType == PLAYER && dir == RIGHT)
		{
			if (thing->tgSpeed > 0)
				thing->tgSpeed = 0;
			while (Game::checkCollision(thing, Game::things[tgLevelUnit]))
				thing->tgHitboxRect.x--;
			thing->tgHitboxRect.x++;
		}
		else if (thing->tgType == PLAYER && dir == LEFT)
		{
			if (thing->tgSpeed < 0)
				thing->tgSpeed = 0;
			while (Game::checkCollision(thing, Game::things[tgLevelUnit]))
				thing->tgHitboxRect.x++;
			thing->tgHitboxRect.x--;
		}
		else if (thing->tgType == ENEMY && dir == RIGHT)
		{
			while (Game::checkCollision(thing, Game::things[tgLevelUnit]))
				thing->tgHitboxRect.x--;
			thing->tgHitboxRect.x++;
			thing->tgDirection = invertDir(thing->tgDirection);
			thing->tgSpeed = -thing->tgSpeed;
		}
		else if (thing->tgType == ENEMY && dir == LEFT)
		{
			while (Game::checkCollision(thing, Game::things[tgLevelUnit]))
				thing->tgHitboxRect.x++;
			thing->tgHitboxRect.x--;
			thing->tgDirection = invertDir(thing->tgDirection);
			thing->tgSpeed = -thing->tgSpeed;
		}
		else if (dir == DOWN)
		{
			if (thing->tgVerticals > 0)
				thing->tgVerticals = 0;
			while (Game::checkCollision(thing, Game::things[tgLevelUnit]))
				thing->tgHitboxRect.y--;
			thing->tgHitboxRect.y++;
		}
		else if (dir == UP)
		{
			if (thing->tgVerticals < 0)
				thing->tgVerticals = 0;
			while (Game::checkCollision(thing, Game::things[tgLevelUnit]))
				thing->tgHitboxRect.y++;
			thing->tgHitboxRect.y--;
		}
		else
			std::cout << "Collision error: " << tgLevelUnit << " " << thing->tgLevelUnit << std::endl;
	}
}

void Tile::tgRender(void)
{
	tgSyncTexture();

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

	Game::gGraphics.tileTextures[tiType - TILE_TYPE_OFFSET][tgFrame].txRect = tgGFXrect;
	Game::gGraphics.tileTextures[tiType - TILE_TYPE_OFFSET][tgFrame].txRender();
}

/*
void Tile::tgApplyAI(void)
{

}
*/

int Tile::tgGetSubtype(void)
{
	return tiType;
}