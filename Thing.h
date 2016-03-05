#pragma once
#include "Include.h"

class Thing
{
public:
	// SDL_Rect = hitbox rect; int = thing type; int = thing subtype; int = level unit
	Thing(SDL_Rect* = NULL, int = -1, int = -1);
	~Thing();

	virtual void tgRender(void);

	// Thing* = thing being collided with, int = direction of collision relative to this thing (not the passed thing)
	virtual void tgResolveCollision(Thing*, int);

	// uses default gravity and jumping
	virtual void tgHandleVerticals(void);

	virtual void tgApplyAI(void);

	void tgSyncTexture(void);

	virtual int tgGetSubtype(void);

	int tgType;
	int tgHealth;
	int tgSpeed;
	int tgFrame;
	// int tgFrameWaitCounter;
	int tgLevelUnit;
	int tgVerticals;
	bool tgSelected;
	// Direction tgDirection;
	SDL_Rect tgHitboxRect;
	SDL_Rect tgGFXrect;
};