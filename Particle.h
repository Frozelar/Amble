#pragma once
#include "Graphics.h"


class Particle
{
public:
	Particle(SDL_Rect*, ParticleType, int, SDL_Point* = NULL, int = -1);
	~Particle();

	void ptMove(void);

	void ptRender(void);

	void ptDestroy(void);

	int ptType = PGRAY;
	int ptLifeSpan = 0;
	int ptAlpha = 255;
	int ptFrame = FRAME_1;
	int ptFrameWaitCounter = 1;
	int ptNumber = -1;
	SDL_Point ptDirection;
	SDL_Rect ptRect;
};

