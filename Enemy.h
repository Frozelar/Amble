#pragma once
#include "Include.h"
#include "Thing.h"

class Enemy : public Thing
{
public:
	// initialize everything
	Enemy(SDL_Rect* = NULL, int = -1, int = -1);

	// apply AI to the enemy
	void tgApplyAI(void);

	// render the enemy
	void tgRender(void);

	// Thing* = what it is colliding with, Direction = which side of the Thing* the collision occurred at
	void tgResolveCollision(Thing*, int);

	int tgGetSubtype(void);
	void tgSetSubtype(int);

	// the type of enemy
	int enType = 0;
	int enPower = 5;
};