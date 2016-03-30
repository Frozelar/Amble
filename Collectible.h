#pragma once
#include "Include.h"
#include "Thing.h"

class Collectible : public Thing
{
public:

	Collectible(SDL_Rect* = NULL, int = -1, int = -1);

	void tgRender(void);
	// void clCollect(void);
	void tgResolveCollision(Thing*, int);
	void tgApplyAI(void);
	void tgHandleVerticals(void);
	int tgGetSubtype(void);

	int clType = 0;
};