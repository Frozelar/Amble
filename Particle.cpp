#include "Particle.h"

Particle::Particle(SDL_Rect* placement, ParticleType type, int num, SDL_Point* destination, int life)
{
	ptNumber = num;
	ptType = type;
	ptRect = *placement;
	if (destination != NULL && life != -1)
	{
		ptDirection = *destination;
		ptLifeSpan = life;
	}
	else
	{
		ptDirection.x = rand() % 18 + (-9);
		ptDirection.y = rand() % 18 + (-9);
		ptLifeSpan = rand() % 100 + 1;
	}
}

Particle::~Particle()
{

}

void Particle::ptMove()
{
	ptRect.x += ptDirection.x;
	ptRect.y += ptDirection.y;
	if (--ptLifeSpan <= 0)
		ptDestroy();
	else if (ptLifeSpan <= 15)
		ptAlpha -= 17;
}

void Particle::ptRender()
{
	if (ptFrameWaitCounter <= 0)
		if ((++ptFrame) >= TOTAL_OBJECT_FRAME_TYPES)
			ptFrame = 0;

	Game::gGraphics.particleTextures[ptType][ptFrame].txRect = ptRect;
	Graphics::particleTextures[ptType][ptFrame].txAlpha(ptAlpha);
	Graphics::particleTextures[ptType][ptFrame].txRender();
}

void Particle::ptDestroy()
{
	delete Game::particles[ptNumber];
	Game::particles[ptNumber] = NULL;
}