#pragma once
#include "Include.h"


class Particle
{
public:
	// ptRect, ptType, ptNum, ptDestination, ptLife, ptSpeed
	Particle(SDL_Rect*, int, int, SDL_Point* = NULL, int = -1, int = -1, int = -1);
	~Particle();

	void ptMove(void);

	void ptRender(void);

	// void ptDestroy(void);

	int ptType = 0;
	// int ptLifeSpan = 0;
	int ptSpeedX = 0;
	int ptSpeedY = 0;
	int ptAlpha = 255;
	int ptFrame = 0;
	int ptFrameWaitCounter = 1;
	int ptNumber = -1;
	SDL_Point ptDestination;
	SDL_Rect ptRect;
};

