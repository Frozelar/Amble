#pragma once
#include "Include.h"
#include "Thing.h"

class Player : public Thing
{
public:
	// set properties
	Player(SDL_Rect* = NULL, int = -1);

	// destroy texture
	// ~Player();

	// check to see if the input has to do with the player (returns whether it did or not)
	void plHandleEvent(SDL_Event*);

	// moves the player and checks collision (takes a moveType, or -1 if the direction is built in to the speed)
	void plMove(void);

	void tgRender(void);

	void tgHandleVerticals(void);

	void plHandleDashing(void);

	void tgResolveCollision(Thing*, int);

	int plJumps = 0;
	int plActionCounter = 0;
	int plState = 0;
	int plOldVerticals = 0;
	SDL_Rect plOldHitboxRect;
};