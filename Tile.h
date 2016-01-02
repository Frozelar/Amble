#pragma once
#include "Graphics.h"

class Tile : public Thing
{
public:
	// initialize everything
	Tile(SDL_Rect* = NULL, int = -1, int = -1);

	// destroy texture
	// ~Tile();

	void tgResolveCollision(Thing*, Direction);
	void tgRender(void);
	// void tgApplyAI(void);
	int tgGetSubtype(void);

	int tiType = 0;
	bool tiIsSolid = true;
	bool tiIsAnimated = false;
};