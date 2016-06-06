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


#include "Level.h"
#include "Game.h"
#include "Player.h"
#include "Thing.h"
#include "LuaBridge.h"
#include "Audio.h"
#include "Particle.h"
#include "Projectile.h"
#include "Graphics.h"

// default units in the level
int Level::LEVEL_UNITS = Level::levelW() * Level::levelH();

// default width of the level (in units)
int Level::LEVEL_W = (int)sqrt(LEVEL_UNITS);

// default height of the level (in units)
int Level::LEVEL_H = (int)sqrt(LEVEL_UNITS);

// default level width in pixels
int Level::LEVEL_W_PIXELS = LEVEL_W * Game::DEFAULT_W;

// default level height in pixels
int Level::LEVEL_H_PIXELS = LEVEL_H * Game::DEFAULT_H;

// default pixels in the level
int Level::LEVEL_PIXELS = LEVEL_UNITS * Game::DEFAULT_W;

int Level::levelTrack = 0;
int Level::levelBG = 0;

int Level::gLevelMovementsX = 0;
int Level::gLevelMovementsY = 0;

int Level::totalLevels = 0;
int Level::currentLevel = 0;

Level* Game::gLevel;

Level::Level()
{
}

Level::~Level()
{
}

bool Level::generateLevel(int whichLevel)
{
	std::string ext = ".map";
	int x = 0, y = 0;
	int unitType = -1;
	int unit = 0;
	std::ifstream levelMap;
	bool blockIsAnimated = false;
	SDL_Rect playerRect{ 0, 0, Game::PLAYER_W, Game::PLAYER_H };
	// bool isCharacter = false;

	closeLevel();
	if (whichLevel > 0)
		levelMap.open(Game::rDir + "level" + std::to_string(whichLevel) + ext);
	else if (whichLevel == 0)
		levelMap.open(Game::rDir + "title" + ext);
	else if (whichLevel == -1)
		levelMap.open(Game::rDir + "output" + ext);
	currentLevel = whichLevel;

	levelMap >> unitType;
	LEVEL_W = unitType;
	LEVEL_W_PIXELS = LEVEL_W * Game::DEFAULT_W;
	levelMap >> unitType;
	LEVEL_H = unitType;
	LEVEL_H_PIXELS = LEVEL_H * Game::DEFAULT_H;
	LEVEL_UNITS = LEVEL_W * LEVEL_H;
	LEVEL_PIXELS = LEVEL_UNITS * Game::DEFAULT_W;
	Game::things.resize(LEVEL_UNITS);
	Game::gColliding.resize(LEVEL_UNITS);

	levelMap >> unitType;
	levelBG = unitType /* - Game::OFFSET["BACKGROUND"] */ - 1;
	levelMap >> unitType;
	levelTrack = unitType /* - Game::OFFSET["MUSIC"] */ - 1;

	for (int i = 0; i < LEVEL_PIXELS;)
	{
		unitType = -1;

		levelMap >> unitType;

		if (levelMap.fail())
		{
			std::cout << "Failed to load level.map!  Darn." << std::endl;
			return false;
		}

		if (unitType != -1)
		{
			if (unitType != 0)
				Game::newThing(unitType, unit, x, y, -1);

			i += Game::DEFAULT_W;

			x += Game::DEFAULT_W;
			if (x >= LEVEL_W_PIXELS)
			{
				x = 0;
				y += Game::DEFAULT_H;
			}
			unit++;
		}
	}

	levelMap.close();
	// Game::centerCamera();
	Game::gScore = 0;
	LuaBridge::labChangeLevel();
	playMusic();
	// LuaBridge::labChangedLevel = true;
	return true;
}

void Level::moveLevel(void)
{
	// if (Game::gPlayer.tgSpeed != 0)
	// 	Level::gLevelMovementsX -= Game::gPlayer.tgSpeed;
	if (Game::gPlayer->tgHitboxRect.x != Game::gPlayer->plOldHitboxRect.x)
		gLevelMovementsX += (Game::gPlayer->plOldHitboxRect.x - Game::gPlayer->tgHitboxRect.x);
	if (Game::gPlayer->tgHitboxRect.y != Game::gPlayer->plOldHitboxRect.y)
		gLevelMovementsY += (Game::gPlayer->plOldHitboxRect.y - Game::gPlayer->tgHitboxRect.y);
	// if (Game::gPlayer.tgVerticals != 0)
	// 	Level::gLevelMovementsY += (Game::gPlayer.plOldHitboxRect.y - (Game::gPlayer.tgHitboxRect.y +
	// 	(Game::gPlayer.tgVerticals > 0 ? Game::gravityArray[Game::gPlayer.tgVerticals] : -Game::jumpArray[-Game::gPlayer.tgVerticals])));
	if (gLevelMovementsX != 0 || gLevelMovementsY != 0)
	{
		for (int i = 0; i < LEVEL_UNITS; i++)
		{
			if (Game::things[i] != NULL && Game::things[i]->tgType != Game::ThingType["player"])
			{
				Game::things[i]->tgHitboxRect.y += gLevelMovementsY;
				Game::things[i]->tgHitboxRect.x += gLevelMovementsX;
			}
		}
		for (int i = 0; i < Game::gParticles.size(); i++)
		{
			if (Game::gParticles[i] != NULL)
			{
				Game::gParticles[i]->ptRect.x += gLevelMovementsX;
				Game::gParticles[i]->ptRect.y += gLevelMovementsY;
			}
		}
		for (int i = 0; i < Game::gProjectiles.size(); i++)
		{
			if (Game::gProjectiles[i] != NULL)
			{
				Game::gProjectiles[i]->pjRect.x += gLevelMovementsX;
				Game::gProjectiles[i]->pjRect.y += gLevelMovementsY;
			}
		}
	}
	Game::gCamera.x += gLevelMovementsX;
	Game::gCamera.y += gLevelMovementsY;
	gLevelMovementsX = 0;
	gLevelMovementsY = 0;
	Game::gPlayer->tgHitboxRect = Game::gPlayer->plOldHitboxRect;

	// if ((Game::gPlayer.tgSpeed > 0 && Game::gPlayer.tgDirection == LEFT) || 
	// 	(Game::gPlayer.tgSpeed < 0 && Game::gPlayer.tgDirection == RIGHT))
	// 	Game::gPlayer.tgSpeed = 0;
}

void Level::playMusic(void)
{
	Audio::auPlay(levelTrack, 'm');
}

/*
void Level::setBG(void)
{
	
}
*/

void Level::closeLevel()
{
	Game::gCamera = { 0, 0, 0, 0 };
	for (int i = 0; i < Game::things.size(); i++)
	{
		if (Game::things[i] != NULL)
		{
			if (Game::things[i]->tgType != Game::ThingType["player"])
				delete Game::things[i];
			Game::things[i] = NULL;
		}
	}
	for (int i = 0; i < Game::gParticles.size(); i++)
		if (Game::gParticles[i] != NULL)
			Game::gParticles[i]->ptLife = 0; // Game::destroyParticle(i);
	for (int i = 0; i < Game::gProjectiles.size(); i++)
		if (Game::gProjectiles[i] != NULL)
			Game::gProjectiles[i]->pjLife = 0; //Game::destroyProjectile(i);
}

const int Level::levelW(void)
{
	std::ifstream levelMap("resources/level000.map");
	int unitType = -1; levelMap >> unitType;
	return unitType;
}
const int Level::levelH(void)
{
	std::ifstream levelMap("resources/level000.map");
	int unitType = -1; levelMap >> unitType; levelMap >> unitType;
	return unitType;
}