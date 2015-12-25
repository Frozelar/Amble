#include "Collectible.h"

Collectible::Collectible(SDL_Rect* box, int subtype, int unit) : Thing(box, COLLECTIBLE, unit)
{
	tgHitboxRect.w = Game::DEFAULT_ENEMY_W;
	tgHitboxRect.h = Game::DEFAULT_ENEMY_H;
	tgHitboxRect.x = (box == NULL ? ((unit - ((unit / Level::LEVEL_W) * Level::LEVEL_W)) * Game::DEFAULT_W) : box->x);
	tgHitboxRect.y = (box == NULL ? ((unit / Level::LEVEL_W) * Game::DEFAULT_H) : box->y);
	tgGFXrect.x = tgHitboxRect.x - Game::DEFAULT_OFFSET;
	tgGFXrect.y = tgHitboxRect.y - Game::DEFAULT_OFFSET;
	tgGFXrect.w = Game::DEFAULT_ENEMY_W + Game::DEFAULT_OFFSET * 2;
	tgGFXrect.h = Game::DEFAULT_ENEMY_H + Game::DEFAULT_OFFSET * 2;
	clType = subtype;
}

void Collectible::tgRender(void)
{
	tgSyncTexture();

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

	Graphics::collectibleTextures[clType - COLLECTIBLE_TYPE_OFFSET][tgFrame].txRect = tgGFXrect;
	Graphics::collectibleTextures[clType - COLLECTIBLE_TYPE_OFFSET][tgFrame].txRender();
}

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
	*/
	delete Game::things[tgLevelUnit];
	Game::things[tgLevelUnit] = NULL;
}

void Collectible::tgResolveCollision(Thing* thing, Direction dir)
{
	if (thing->tgType == PLAYER)
		tgHealth = 0;
}

void Collectible::tgApplyAI(void)
{
	if (tgHealth == 0)
		clCollect();
}

void Collectible::tgHandleVerticals(void)
{
	tgHitboxRect.y += Game::floatArray[tgVerticals++];
	if (tgVerticals >= (int)Game::floatArray.size())
		tgVerticals = 0;
}