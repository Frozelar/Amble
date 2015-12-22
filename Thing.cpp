#include "Thing.h"

Thing::Thing(SDL_Rect* box, int type, int unit)
{
	if (box != NULL)
		tgHitboxRect = *box;
	tgType = type;
	tgLevelUnit = unit;
	tgHealth = Game::DEFAULT_HEALTH;
	tgSpeed = 0;
	tgFrame = 0;
	tgFrameWaitCounter = 0;
	tgVerticals = 0;
	tgSelected = 0;
}

void Thing::tgSyncTexture(void)
{
	if (tgType != TILE)
	{
		tgGFXrect.x = tgHitboxRect.x - Game::DEFAULT_OFFSET;
		tgGFXrect.y = tgHitboxRect.y - Game::DEFAULT_OFFSET;
		tgGFXrect.w = tgHitboxRect.w + Game::DEFAULT_OFFSET * 2;
		tgGFXrect.h = tgHitboxRect.h + Game::DEFAULT_OFFSET * 2;
	}
	else if (tgType == TILE)
	{
		tgGFXrect.x = tgHitboxRect.x;
		tgGFXrect.y = tgHitboxRect.y;
		tgGFXrect.w = tgHitboxRect.w;
		tgGFXrect.h = tgHitboxRect.h;
	}
}

void Thing::tgHandleVerticals(void)
{
	if (tgVerticals >= (int)Game::gravityArray.size() - 1)
		tgVerticals = Game::gravityArray.size() - 2;
	else if (-tgVerticals >= (int)Game::gravityArray.size())
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

void Thing::tgResolveCollision(Thing* thing, Direction dir)
{
	if (dir == LEFT || dir == RIGHT)
		tgDirection = invertDir(tgDirection);
	else if (dir == UP)
	{
		if (thing->tgVerticals < 0)
			thing->tgVerticals = 1;
	}
	else if (dir == DOWN)
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