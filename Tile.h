#pragma once
#include "Include.h"
#include "Thing.h"

class Tile : public Thing
{
public:
	// initialize everything
	Tile(SDL_Rect* = NULL, int = -1, int = -1);

	// destroy texture
	// ~Tile();

	void tgResolveCollision(Thing*, int);
	void tgRender(void);
	void tgApplyAI(void);
	int tgGetSubtype(void);

	int tiType = 0;
	bool tiIsSolid = true;
	bool tiIsAnimated = false;
};