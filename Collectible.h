#pragma once
#include "Graphics.h"

class Collectible : public Thing
{
public:

	Collectible(SDL_Rect* = NULL, int = -1, int = -1);

	void tgRender(void);
	void clCollect(void);
	void tgResolveCollision(Thing*, Direction);
	void tgApplyAI(void);
	void tgHandleVerticals(void);

	int clType = COLLECTIBLE_TYPE_OFFSET;
};