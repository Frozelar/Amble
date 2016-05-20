#include "Game.h"
#include "Texture.h"
#include "Graphics.h"
#include "Audio.h"
#include "Collectible.h"
#include "Enemy.h"
#include "Level.h"
#include "Particle.h"
#include "Player.h"
#include "Thing.h"
#include "Tile.h"
#include "Menu.h"
#include "LevelEditor.h"
// #include "Particle.h"

std::map<std::string, int> Game::Direction;
std::map<std::string, int> Game::ThingType;
std::map<std::string, int> Game::EntityFrameType;
std::map<std::string, int> Game::EntityFrameGroup;
std::map<std::string, int> Game::ObjectFrameType;
std::map<std::string, int> Game::ObjectFrameGroup;
std::map<std::string, int> Game::TileType;
std::map<std::string, int> Game::TileSubType;
std::map<std::string, int> Game::EnemyType;
std::map<std::string, int> Game::CollectibleType;
std::map<std::string, int> Game::BackgroundType;
std::map<std::string, int> Game::MusicType;
std::map<std::string, int> Game::SoundEffectType;
std::map<std::string, int> Game::ParticleType;
std::map<std::string, int> Game::GameState;

// window width (in pixels)
int Game::WINDOW_W = 800;

// window height (in pixels)
int Game::WINDOW_H = 600;

// default unit width (in pixels)
int Game::DEFAULT_W = 8;

// default unit height (in pixels)
int Game::DEFAULT_H = 8;

// maximum level size
// const int Game::LEVEL_MAX = 16384;

// player hitbox width
int Game::PLAYER_W = 8;

// player hitbox height
int Game::PLAYER_H = 16;

// default enemy hitbox width
int Game::DEFAULT_ENEMY_W = 8;

// default enemy hitbox height
int Game::DEFAULT_ENEMY_H = 8;

// default collectible hitbox width
int Game::DEFAULT_COLLECTIBLE_W = 8;

// default collectible hitbox height
int Game::DEFAULT_COLLECTIBLE_H = 8;

// default offset value for graphics
int Game::DEFAULT_GFX_OFFSET = 2;

// the default speed for movement
int Game::DEFAULT_SPEED = 2;

// total number of possible collisions for one object
// const int Game::TOTAL_COLLISIONS = 16;

// total number of directions
// const int Game::TOTAL_DIRECTIONS = TOT_DIR - 1;

// default value for health (more or less just for debugging)
const int Game::DEFAULT_HEALTH = 100;

std::map< std::string, int > Game::OFFSET;

/*
// const int Game::TOTAL_JUMP_ARRAY_UNITS = 32;
int Game::jumpArray[Game::TOTAL_JUMP_ARRAY_UNITS] = { 8, 8, 8, 7, 7, 6, 6, 5, 5, 5, 4, 4, 4, 4, 4,
3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 0 };
*/

// const int Game::TOTAL_GRAVITY_ARRAY_UNITS = 64;
// int Game::gravityArray[TOTAL_GRAVITY_ARRAY_UNITS] = { 0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3,
// 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8 };

// const int Game::TOTAL_DASH_ARRAY_UNITS = 8;
// int Game::dashArray[TOTAL_DASH_ARRAY_UNITS] = { 16, 12, 8, 6, 4, 2, 2, 1 };

std::vector<int> Game::jumpArray;
std::vector<int> Game::gravityArray;
std::vector<int> Game::dashArray;
std::vector<int> Game::floatArray;

std::string Game::gameTitle = "";

Font Game::gHeadingFont;
Font Game::gBodyFont;
Font Game::gTitleFont;
// Font Game::gFont = NULL;
// std::string Game::gFontName = "";
// int Game::gFontSize = 0;

bool Game::initialized = init();
SDL_Window* Game::gWindow = SDL_CreateWindow("Hey There Guy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Game::WINDOW_W, Game::WINDOW_H, SDL_WINDOW_SHOWN);
SDL_Renderer* Game::gRenderer = SDL_CreateRenderer(Game::gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
SDL_Event* Game::gEvent;
std::vector<Collision*> Game::gColliding;

// Player Game::gPlayer;
std::vector<Thing*> Game::things;
int Game::gScore = 0;

int Game::gState = 0;
int Game::gOldState = 0;
std::string Game::rDir = "resources/";
std::string Game::rExt = ".png";
// SDL_Color Game::textColor = { 255, 255, 255, 255 };
SDL_Color Game::highlightColor = { 255, 255, 255, 255 };

// const int Game::MAX_PARTICLES = 4096;
std::vector<Particle*> Game::particles;

SDL_Rect Game::gCamera = { 0, 0, 0, 0 };

Game* Game::gGame;

Game::Game()
{
	int num = 0;
	//Direction["none"] = num;
	Direction["left"] = (/*++*/num);
	Direction["right"] = (++num);
	Direction["up"] = (++num);
	Direction["down"] = (++num);
	Direction["total"] = (++num);

	ThingType["temp"] = (num = 0) /* -1 */;
	ThingType["player"] = (++num);
	ThingType["tile"] = (++num);
	ThingType["enemy"] = (++num);
	ThingType["collectible"] = (++num);

	GameState["game"] = (num = 0);
	GameState["menu"] = (++num);
	GameState["title"] = (++num);
	GameState["editor"] = (++num);

	OFFSET["TILE"] = 1000;
	OFFSET["ENEMY"] = 2000;
	OFFSET["COLLECTIBLE"] = 3000;
	// OFFSET["BACKGROUND"] = 4000;
	// OFFSET["MUSIC"] = 5000;
}

Game::~Game()
{
	delete gEvent;
	gEvent = NULL;
	delete gAudio;
	gAudio = NULL;
	delete gGraphics;
	gGraphics = NULL;
	delete gPlayer;
	gPlayer = NULL;
	delete gLevel;
	gLevel = NULL;
	delete gLuaBridge;
	gLuaBridge = NULL;
	delete gMenu;
	gMenu = NULL;
	for (int i = 0; i < gColliding.size(); i++)
		if (gColliding[i] != NULL)
		{
			delete gColliding[i];
			gColliding[i] = NULL;
		}
}

bool Game::checkCollision(Thing* thingOne, Thing* thingTwo, int levelunit, bool outputCollision)
{
	// check collision between the first and second things
	// (checks collision with everything in the level if thingTwo is NULL)

	bool isColliding = false;

	// clear out gColliding so that results are not reused
	if (outputCollision)
	{
		for (int i = 0; i < Level::LEVEL_UNITS; i++)
		{
			if (gColliding[i] != NULL)
			{
				delete gColliding[i];
				gColliding[i] = NULL;
			}
		}
	}

	// if we need to check for collision with everything in the level
	if (thingTwo == NULL)
	{
		for (int i = 0; i < Level::LEVEL_UNITS; i++)
		{
			if (things[i] != NULL && i != levelunit)
			{
				if ((thingOne->tgHitboxRect.x + thingOne->tgHitboxRect.w > things[i]->tgHitboxRect.x &&
					thingOne->tgHitboxRect.x < things[i]->tgHitboxRect.x + things[i]->tgHitboxRect.w) &&
					(thingOne->tgHitboxRect.y + thingOne->tgHitboxRect.h > things[i]->tgHitboxRect.y &&
					thingOne->tgHitboxRect.y < things[i]->tgHitboxRect.y + things[i]->tgHitboxRect.h))
				{
					if (outputCollision)
					{
						gColliding[i] = new Collision();
						gColliding[i]->thing1 = thingOne;
						gColliding[i]->thing2 = things[i];
					}
					isColliding = true;
				}
			}
		}
	}
	else if (thingOne != NULL && thingTwo != NULL)
	{
		if ((thingOne->tgHitboxRect.x + thingOne->tgHitboxRect.w > thingTwo->tgHitboxRect.x &&
			thingOne->tgHitboxRect.x < thingTwo->tgHitboxRect.x + thingTwo->tgHitboxRect.w) &&
			(thingOne->tgHitboxRect.y + thingOne->tgHitboxRect.h > thingTwo->tgHitboxRect.y &&
			thingOne->tgHitboxRect.y < thingTwo->tgHitboxRect.y + thingTwo->tgHitboxRect.h))
		{
			isColliding = true;
		}
	}
	return isColliding;
}

bool Game::checkCollisionRects(SDL_Rect* rect1, SDL_Rect* rect2)
{
	if ((rect1->x + rect1->w > rect2->x && rect1->x < rect2->x + rect2->w) &&
		(rect1->y + rect1->h > rect2->y && rect1->y < rect2->y + rect2->h))
		return true;
	else
		return false;
}

bool Game::applyAI(void)
{
	for (int i = 0; i < things.size(); i++)
	{
		if (things[i] != NULL && things[i]->tgType != ThingType["temp"])
		{
			if (things[i]->tgType != ThingType["player"])
				things[i]->tgApplyAI();
			if (things[i]->tgHealth == 0)
			{
				if (things[i]->tgType == ThingType["player"])
					return false;
				else
					destroyThing(i);
			}
		}
	}
	return true;
}

void Game::centerCamera(void)
{
	int differenceX = -1;
	int differenceY = -1;
	if (gPlayer->tgHitboxRect.x + gPlayer->tgHitboxRect.w / 2 != WINDOW_W / 2 ||
		gPlayer->tgHitboxRect.y + gPlayer->tgHitboxRect.h / 2 != WINDOW_H / 2)
	{
		differenceX = WINDOW_W / 2 - (gPlayer->tgHitboxRect.x + gPlayer->tgHitboxRect.w / 2);
		differenceY = WINDOW_H / 2 - (gPlayer->tgHitboxRect.y + gPlayer->tgHitboxRect.h / 2);
		for (int i = 0; i < (int)things.size(); i++)
		{
			if (things[i] != NULL)
			{
				things[i]->tgHitboxRect.x += differenceX;
				things[i]->tgHitboxRect.y += differenceY;
			}
		}
	}
	gCamera = { differenceX, differenceY, Level::LEVEL_W_PIXELS, Level::LEVEL_H_PIXELS };
}

// if type has the offset built-in, then do NOT give the final thingtype argument; otherwise, give a thingtype
void Game::newThing(int type, int levelunit, int x, int y, int thingtype)
{
	SDL_Rect trect = { 0, 0, 0, 0 };
	if (x == -1 || y == -1)
	{
		x = (levelunit - ((levelunit / Level::LEVEL_W) * Level::LEVEL_W)) * DEFAULT_W;
		y = (levelunit / Level::LEVEL_W) * DEFAULT_H;
	}
	if (levelunit > -1 && type > -1 && thingtype == -1)
	{
		if (type == ThingType["player"])
		{
			gPlayer->tgHitboxRect.x = x;
			gPlayer->tgHitboxRect.y = y;
			gPlayer->tgLevelUnit = levelunit;
			things[levelunit] = gPlayer;
		}
		else if (type > OFFSET["TILE"] && type < OFFSET["ENEMY"])
			things[levelunit] = new Tile(NULL, type - OFFSET["TILE"], levelunit);
		else if (type > OFFSET["ENEMY"] && type < OFFSET["COLLECTIBLE"])
			things[levelunit] = new Enemy(NULL, type - OFFSET["ENEMY"], levelunit);
		else if (type > OFFSET["COLLECTIBLE"])
			things[levelunit] = new Collectible(NULL, type - OFFSET["COLLECTIBLE"], levelunit);
		/*
		else if (type > TILE_TYPE_OFFSET && type < TILE_TYPE_OFFSET + TileType["total"])
			things[levelunit] = new Tile(NULL, type - TILE_TYPE_OFFSET, levelunit);
		else if (type > ENEMY_TYPE_OFFSET && type < ENEMY_TYPE_OFFSET + EnemyType["total"])
			things[levelunit] = new Enemy(NULL, type - ENEMY_TYPE_OFFSET, levelunit);
		else if (type > COLLECTIBLE_TYPE_OFFSET && type < COLLECTIBLE_TYPE_OFFSET + CollectibleType["total"])
			things[levelunit] = new Collectible(NULL, type - COLLECTIBLE_TYPE_OFFSET, levelunit);
		*/
	}
	else if (levelunit > -1 && type > -1 && thingtype > -1)
	{
		trect = { x / DEFAULT_W * DEFAULT_W, y / DEFAULT_H * DEFAULT_H, 0, 0 };
		if (gCamera.x != 0 || gCamera.y != 0)
		{
			trect.x += gCamera.x;
			trect.y += gCamera.y;
		}
		if (type == ThingType["player"])
		{
			gPlayer->tgHitboxRect.x = x;
			gPlayer->tgHitboxRect.y = y;
			gPlayer->tgLevelUnit = levelunit;
			things[levelunit] = gPlayer;
		}
		else if (type == ThingType["tile"])
			things[levelunit] = new Tile(&trect, thingtype, levelunit);
		else if (type == ThingType["enemy"])
			things[levelunit] = new Enemy(&trect, thingtype, levelunit);
		else if (type == ThingType["collectible"])
			things[levelunit] = new Collectible(&trect, thingtype, levelunit);
	}
}

void Game::newParticle(SDL_Rect* location, int type, SDL_Point* destination, int life = -1, int speed = -1)
{
	int pos = particles.size();
	particles.resize(pos + 1);
	//particles[pos] = new Particle(location, type, pos, destination, life, speed);
}

void Game::destroyThing(int num)
{
	delete things[num];
	things[num] = NULL;
}

void Game::destroyParticle(int num)
{
	delete particles[num];
	particles[num] = NULL;
}

void Game::changeGameState(int newState)
{
	gOldState = gState;
	if (Game::gState != newState && newState >= 0 && newState < GameState.size())
		Game::gState = newState;
}

void Game::readCFG(bool update)
{
	int controlMenuPos = Menu::muGetMenuPos("gmcontrols");
	std::ifstream cfg;
	cfg.open(Game::rDir + "config.cfg");
	cfg >> Graphics::GFX_SCALE;
	cfg >> Graphics::isFullscreen;
	cfg >> Audio::volume[0];
	cfg >> Audio::volume[1];

	for (int i = controlMenuPos; i < controlMenuPos + Menu::NumOptions[Menu::MenuID["gmcontrols"]] - 1; i++)
		cfg >> Game::gPlayer->plControls[Menu::muOptions[i]];
	controlMenuPos = Menu::muGetMenuPos("lecontrols");
	for (int i = controlMenuPos; i < controlMenuPos + Menu::NumOptions[Menu::MenuID["lecontrols"]] - 1; i++)
		cfg >> LevelEditor::leControls[Menu::muOptions[i]];


	if (update)
	{
		if (Graphics::GFX_SCALE != 1.0)
		{
			if (Graphics::GFX_SCALE == 2.0)
				Graphics::GFX_MULT = 2.0;
			Graphics::gxIncScale(false);
		}
		if (Graphics::isFullscreen)
			Graphics::gxToggleFullscreen(false);
		Audio::auIncVolume(Audio::MUSIC_VOL_INDEX, false);
		Audio::auIncVolume(Audio::SFX_VOL_INDEX, false);
	}
	Menu::muUpdateButtons();
}

void Game::writeCFG()
{
	int controlMenuPos = Menu::muGetMenuPos("gmcontrols");
	std::ofstream cfg;
	cfg.open(Game::rDir + "config.cfg");
	cfg.clear();
	cfg << Graphics::GFX_SCALE << " ";
	cfg << Graphics::isFullscreen << " ";
	cfg << Audio::volume[0] << " ";
	cfg << Audio::volume[1] << " ";

	for (int i = controlMenuPos; i < controlMenuPos + Menu::NumOptions[Menu::MenuID["gmcontrols"]] - 1; i++)
		cfg << Game::gPlayer->plControls[Menu::muOptions[i]] << " ";
	controlMenuPos = Menu::muGetMenuPos("lecontrols");
	for (int i = controlMenuPos; i < controlMenuPos + Menu::NumOptions[Menu::MenuID["lecontrols"]] - 1; i++)
		cfg << LevelEditor::leControls[Menu::muOptions[i]] << " ";
}