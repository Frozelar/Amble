/*
Amble: a platformer game being made as a hobby.
Copyright (C) 2015, 2016 Frozelar

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "Particle.h"
#include "Game.h"
#include "Graphics.h"
#include "Texture.h"

Particle::Particle(SDL_Rect* placement, int type, int num, SDL_Point* destination, int life, int speedX, int speedY)
{
	ptNumber = num;
	ptType = type;
	ptRect = *placement;
	if (life != -1)
		ptLife = life;
	else
		ptLife = 1;
	if (destination != NULL)
		ptDestination = *destination;
	else
		ptDestination = { placement->x + rand() % 18 + (-9), placement->y + rand() % 18 + (-9) };		// rectToPolar({ placement->x + rand() % 18 + (-9), placement->y + rand() % 18 + (-9) });
	/*
	if(life != -1)
		ptLifeSpan = life;
	else
		ptLifeSpan = rand() % 100 + 1;
		*/
	// if (speedX != -1 && speedY != -1)
	// {
		ptSpeedX = speedX;
		ptSpeedY = speedY;
	// }
	// else
	// {
	// 	ptSpeedX = (ptRect.x > ptDestination.x ? rand() % Game::DEFAULT_SPEED : -(rand() % Game::DEFAULT_SPEED));
	// 	ptSpeedY = (ptRect.y > ptDestination.y ? rand() % Game::DEFAULT_SPEED : -(rand() % Game::DEFAULT_SPEED));
		// ptSpeedX = ptDestination.x - ptRect.x - (rand() % ptDestination.x - ptRect.x - 1);
		// ptSpeedY = ptDestination.y - ptRect.y - (rand() % ptDestination.y - ptRect.y - 1);
		// ptSpeedX = (ptDestination.x - ptRect.x) - rand() % std::abs((int)(ptDestination.x - ptRect.x)) + 1;
		// ptSpeedY = (ptDestination.y - ptRect.y) - rand() % std::abs((int)(ptDestination.y - ptRect.y)) + 1;
		// ptSpeedX = (polarToRect(ptDestination).x - ptRect.x) - (rand() % ((polarToRect(ptDestination).x - ptRect.x - 1) == 0 ? 1 : (polarToRect(ptDestination).x - ptRect.x - 1)));
		// ptSpeedY = (polarToRect(ptDestination).y - ptRect.y) - (rand() % ((polarToRect(ptDestination).y - ptRect.y - 1) == 0 ? 1 : (polarToRect(ptDestination).y - ptRect.y - 1)));
	// }
}

Particle::~Particle()
{

}

void Particle::ptMove()
{
	ptRect.x += ptSpeedX;
	ptRect.y += ptSpeedY;
	// std::cout << ptNumber << " | " << ptRect.x << " " << ptDestination.x << " | " << ptRect.y << " " << ptDestination.y << std::endl;
	if ((((ptSpeedX > 0 && ptRect.x > ptDestination.x) || (ptSpeedX < 0 && ptRect.x < ptDestination.x)) && 
		((ptSpeedY > 0 && ptRect.y > ptDestination.y) || (ptSpeedY < 0 && ptRect.y < ptDestination.y))) || 
		((ptSpeedX == 0 && ptSpeedY > 0 && ptRect.y > ptDestination.y) || (ptSpeedX == 0 && ptSpeedY < 0 && ptRect.y < ptDestination.y) || 
		(ptSpeedY == 0 && ptSpeedX > 0 && ptRect.x > ptDestination.x) || (ptSpeedY == 0 && ptSpeedX < 0 && ptRect.x < ptDestination.x)))
		ptLife = 0; // Game::destroyParticle(ptNumber);
	// else if (ptRect.x % 15 == 0 || ptRect.y % 15 == 0)
	// 	ptAlpha -= 17;
}

void Particle::ptRender()
{
	/*
	if (ptFrameWaitCounter <= 0)
		if ((++ptFrame) >= TOTAL_OBJECT_FRAME_TYPES)
			ptFrame = 0;
	*/
	Graphics::particleTextures[ptType][ptFrame]->txRect = ptRect;
	Graphics::particleTextures[ptType][ptFrame]->txAlpha(ptAlpha);
	Graphics::particleTextures[ptType][ptFrame]->txRender();
}

/*
void Particle::ptDestroy()
{
	delete Game::particles[ptNumber];
	Game::particles[ptNumber] = NULL;
}
*/