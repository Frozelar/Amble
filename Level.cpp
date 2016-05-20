#include "Level.h"
#include "Game.h"
#include "Player.h"
#include "Thing.h"
#include "LuaBridge.h"
#include "Audio.h"

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
	int unit = -1;
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
	Level::LEVEL_W = unitType;
	Level::LEVEL_W_PIXELS = Level::LEVEL_W * Game::DEFAULT_W;
	levelMap >> unitType;
	Level::LEVEL_H = unitType;
	Level::LEVEL_H_PIXELS = Level::LEVEL_H * Game::DEFAULT_H;
	Level::LEVEL_UNITS = Level::LEVEL_W * Level::LEVEL_H;
	Level::LEVEL_PIXELS = Level::LEVEL_UNITS * Game::DEFAULT_W;
	Game::things.resize(Level::LEVEL_UNITS);
	Game::gColliding.resize(Level::LEVEL_UNITS);

	levelMap >> unitType;
	levelBG = unitType /* - Game::OFFSET["BACKGROUND"] */ - 1;
	levelMap >> unitType;
	levelTrack = unitType /* - Game::OFFSET["MUSIC"] */ - 1;

	for (int i = 0, j = 0; i < Level::LEVEL_PIXELS;)
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
			unit++;
			if(unitType != 0)
				Game::newThing(unitType, unit, x, y, -1);

			i += Game::DEFAULT_W;
			j++;

			if (x + Game::DEFAULT_W >= Level::LEVEL_W * Game::DEFAULT_W)
			{
				x = 0;
				y += Game::DEFAULT_H;
			}
			else
				x += Game::DEFAULT_W;
		}
	}

	levelMap.close();
	// Game::centerCamera();
	LuaBridge::labChangeLevel();
	Level::playMusic();
	// LuaBridge::labChangedLevel = true;
	return true;
}

void Level::moveLevel(void)
{
	// if (Game::gPlayer.tgSpeed != 0)
	// 	Level::gLevelMovementsX -= Game::gPlayer.tgSpeed;
	if (Game::gPlayer->tgHitboxRect.x != Game::gPlayer->plOldHitboxRect.x)
		Level::gLevelMovementsX += (Game::gPlayer->plOldHitboxRect.x - Game::gPlayer->tgHitboxRect.x);
	if (Game::gPlayer->tgHitboxRect.y != Game::gPlayer->plOldHitboxRect.y)
		Level::gLevelMovementsY += (Game::gPlayer->plOldHitboxRect.y - Game::gPlayer->tgHitboxRect.y);
	// if (Game::gPlayer.tgVerticals != 0)
	// 	Level::gLevelMovementsY += (Game::gPlayer.plOldHitboxRect.y - (Game::gPlayer.tgHitboxRect.y +
	// 	(Game::gPlayer.tgVerticals > 0 ? Game::gravityArray[Game::gPlayer.tgVerticals] : -Game::jumpArray[-Game::gPlayer.tgVerticals])));
	if (Level::gLevelMovementsX != 0 || Level::gLevelMovementsY != 0)
	{
		for (int i = 0; i < Level::LEVEL_UNITS; i++)
		{
			if (Game::things[i] != NULL && Game::things[i]->tgType != Game::ThingType["player"])
			{
				Game::things[i]->tgHitboxRect.y += Level::gLevelMovementsY;
				Game::things[i]->tgHitboxRect.x += Level::gLevelMovementsX;
			}
		}
	}
	Game::gCamera.x += Level::gLevelMovementsX;
	Game::gCamera.y += Level::gLevelMovementsY;
	Level::gLevelMovementsX = 0;
	Level::gLevelMovementsY = 0;
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