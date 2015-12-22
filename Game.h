#pragma once
#include "Include.h"

enum Direction{
	NO_DIRECTION, UP, DOWN, LEFT, RIGHT, TOT_DIR
};

enum ThingType{
	TEMP, PLAYER, TILE, ENEMY, COLLECTIBLE, TOTAL_THING_TYPES
};

enum EntityFrameType{
	IDLE_1, IDLE_2, IDLE_3, IDLE_4, IDLE_5,
	WALK_LEFT_1, WALK_LEFT_2, WALK_LEFT_3, WALK_LEFT_4, WALK_LEFT_5,
	WALK_RIGHT_1, WALK_RIGHT_2, WALK_RIGHT_3, WALK_RIGHT_4, WALK_RIGHT_5,
	JUMP_1, JUMP_2, JUMP_3, JUMP_4, JUMP_5,
	FALL_1, FALL_2, FALL_3, FALL_4, FALL_5,
	TOTAL_ENTITY_FRAME_TYPES
};

enum ObjectFrameType{
	FRAME_1, FRAME_2, FRAME_3, FRAME_4, FRAME_5, TOTAL_OBJECT_FRAME_TYPES
};
const int TOTAL_ANIMATION_FRAMES = 5;

/*
enum TileVariety{
	DIRT_BLOCK, DIRT_WALL, TOTAL_TILE_VARIETIES
};
*/

/*
enum TileType{
TILE_STARTING_VALUE = 10,
DIRT_BLOCK_CENTER, DIRT_BLOCK_TOP, DIRT_BLOCK_TOPRIGHT, DIRT_BLOCK_RIGHT, DIRT_BLOCK_BOTTOMRIGHT, DIRT_BLOCK_BOTTOM,
DIRT_BLOCK_BOTTOMLEFT, DIRT_BLOCK_LEFT, DIRT_BLOCK_TOPLEFT,
DIRT_WALL_CENTER, DIRT_WALL_TOP, DIRT_WALL_TOPRIGHT, DIRT_WALL_RIGHT, DIRT_WALL_BOTTOMRIGHT, DIRT_WALL_BOTTOM, DIRT_WALL_BOTTOMLEFT,
DIRT_WALL_LEFT, DIRT_WALL_TOPLEFT,
TILE_ENDING_VALUE
};
*/
static int TOTAL_TILE_TYPES = 0;
const int TILE_TYPE_OFFSET = 10 /* + 1 */;

enum TileSubType{
	TCENTER, TTOP, TTOPRIGHT, TRIGHT, TBOTTOMRIGHT, TBOTTOM, TBOTTOMLEFT, TLEFT, TTOPLEFT, TTS
};
static int TOTAL_TILE_SUBTYPES = TTS;

// enum EnemyType{
// 	ENEMY_STARTING_VALUE = 100, DUDE, ENEMY_ENDING_VALUE
// };
static int TOTAL_ENEMY_TYPES = 0;
const int ENEMY_TYPE_OFFSET = 100;

// enum CollectibleType{
// 	COLLECTIBLE_STARTING_VALUE = 200, BIT, BYTE, COLLECTIBLE_ENDING_VALUE
// };
static int TOTAL_COLLECTIBLE_TYPES = 0;
const int COLLECTIBLE_TYPE_OFFSET = 200;

enum BackgroundType{
	BG_STARTING_VALUE = 300, UNDERGROUND_BG, BG_ENDING_VALUE
};
const int TOTAL_BACKGROUND_TYPES = BG_ENDING_VALUE - BG_STARTING_VALUE - 1;
const int BACKGROUND_TYPE_OFFSET = BG_STARTING_VALUE;

enum MusicType{
	MUSIC_STARTING_VALUE = 400, UNDERGROUND_MUSIC, MUSIC_ENDING_VALUE
};
const int TOTAL_MUSIC_TYPES = MUSIC_ENDING_VALUE - MUSIC_STARTING_VALUE - 1;
const int MUSIC_TYPE_OFFSET = MUSIC_STARTING_VALUE;

enum SoundEffectType{
	SFX_STARTING_VALUE = 500, WHACK, SFX_ENDING_VALUE
};
const int TOTAL_SFX_TYPES = SFX_ENDING_VALUE - SFX_STARTING_VALUE - 1;
const int SFX_TYPE_OFFSET = SFX_STARTING_VALUE;

enum ParticleType{
	NO_PARTICLE_TYPE = -1, PRED, PGRAY, PBLUE, PBIGRED, PBIGGRAY, PBIGBLUE, TOTAL_PARTICLE_TYPES
};

class Game
{
public:
	Game();
	~Game();

	static int WINDOW_W;
	static int WINDOW_H;
	static int DEFAULT_W;
	static int DEFAULT_H;
	// static const int LEVEL_MAX;
	static int PLAYER_W;
	static int PLAYER_H;
	static int DEFAULT_ENEMY_W;
	static int DEFAULT_ENEMY_H;
	static int DEFAULT_OFFSET;
	static int DEFAULT_SPEED;
	// static const int TOTAL_COLLISIONS;
	static const int TOTAL_DIRECTIONS;
	// static const int MAX_PARTICLES;
	static const int DEFAULT_HEALTH;
	// static int TOTAL_JUMP_ARRAY_UNITS;
	static std::vector<int> jumpArray;
	// static int TOTAL_GRAVITY_ARRAY_UNITS;
	static std::vector<int> gravityArray;
	// static int TOTAL_DASH_ARRAY_UNITS;
	static std::vector<int> dashArray;
	// static int TOTAL_FLOAT_ARRAY_UNITS;
	static std::vector<int> floatArray;
	static bool initialized;
	static TTF_Font* gFont;
	static SDL_Window* gWindow;
	static SDL_Renderer* gRenderer;
	static SDL_Event gEvent;
	static Audio gAudio;
	static std::vector<Collision*> gColliding;
	static Graphics gGraphics;
	static Player gPlayer;
	// static LuaBridge gLuaBridge;
	static std::vector<Thing*> things;
	static std::vector<Particle*> particles;
	static int gScore;

	static Game gGame;
	static Level gLevel;

	static void centerCamera(void);
	static void applyAI(void);
	static void newThing(int, int, int, int);

	// Thing* = first thing, Thing* = second thing (or NULL to check first thing and everything), int = level unit of first thing,
	// bool = whether to output collision to gColliding; returns whether or not there was a collision
	static bool checkCollision(Thing* = NULL, Thing* = NULL, int = -1, bool = true);

	static bool checkCollisionRects(SDL_Rect, SDL_Rect);
};