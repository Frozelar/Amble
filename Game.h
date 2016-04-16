#pragma once
#include "Include.h"

/*
std::map<std::string, int> Direction;
std::map<std::string, int> ThingType;
std::map<std::string, int> EntityFrameType;
std::map<std::string, int> ObjectFrameType;
std::map<std::string, int> TileType;
std::map<std::string, int> TileSubType;
std::map<std::string, int> EnemyType;
std::map<std::string, int> CollectibleType;
std::map<std::string, int> BackgroundType;
std::map<std::string, int> MusicType;
std::map<std::string, int> SoundEffectType;
std::map<std::string, int> ParticleType;
*/

/*
enum Direction{
	NO_DIRECTION, UP, DOWN, LEFT, RIGHT, TOT_DIR
};
*/

/*
enum ThingType{
	TEMP, PLAYER, TILE, ENEMY, COLLECTIBLE, TOTAL_THING_TYPES
};
*/

/*
enum EntityFrameType{
	IDLE_1, IDLE_2, IDLE_3, IDLE_4,
	WALK_LEFT_1, WALK_LEFT_2, WALK_LEFT_3, WALK_LEFT_4,
	WALK_RIGHT_1, WALK_RIGHT_2, WALK_RIGHT_3, WALK_RIGHT_4,
	JUMP_1, JUMP_2,
	FALL_1, FALL_2,
	TOTAL_ENTITY_FRAME_TYPES
};
*/

/*
enum ObjectFrameType{
	FRAME_1, FRAME_2, FRAME_3, FRAME_4, TOTAL_OBJECT_FRAME_TYPES
};
const int TOTAL_ANIMATION_FRAMES = 5;
*/

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
// static int TOTAL_TILE_TYPES = 0;
// const int TILE_TYPE_OFFSET = 10 /* + 1 */;

/*
enum TileSubType{
	TCENTER, TTOP, TTOPRIGHT, TRIGHT, TBOTTOMRIGHT, TBOTTOM, TBOTTOMLEFT, TLEFT, TTOPLEFT, TTS
};
static int TOTAL_TILE_SUBTYPES = TTS;
*/

// enum EnemyType{
// 	ENEMY_STARTING_VALUE = 100, DUDE, ENEMY_ENDING_VALUE
// };
// static int TOTAL_ENEMY_TYPES = 0;
// const int ENEMY_TYPE_OFFSET = 100;

// enum CollectibleType{
// 	COLLECTIBLE_STARTING_VALUE = 200, BIT, BYTE, COLLECTIBLE_ENDING_VALUE
// };
// static int TOTAL_COLLECTIBLE_TYPES = 0;
// const int COLLECTIBLE_TYPE_OFFSET = 200;

/*
enum BackgroundType{
	BG_STARTING_VALUE = 300, UNDERGROUND_BG, BG_ENDING_VALUE
};
const int TOTAL_BACKGROUND_TYPES = BG_ENDING_VALUE - BG_STARTING_VALUE - 1;
const int BACKGROUND_TYPE_OFFSET = BG_STARTING_VALUE;
*/

/*
enum MusicType{
	MUSIC_STARTING_VALUE = 400, UNDERGROUND_MUSIC, MUSIC_ENDING_VALUE
};
const int TOTAL_MUSIC_TYPES = MUSIC_ENDING_VALUE - MUSIC_STARTING_VALUE - 1;
const int MUSIC_TYPE_OFFSET = MUSIC_STARTING_VALUE;
*/

/*
enum SoundEffectType{
	SFX_STARTING_VALUE = 500, WHACK, SFX_ENDING_VALUE
};
const int TOTAL_SFX_TYPES = SFX_ENDING_VALUE - SFX_STARTING_VALUE - 1;
const int SFX_TYPE_OFFSET = SFX_STARTING_VALUE;
*/

/*
enum ParticleType{
	NO_PARTICLE_TYPE = -1, PRED, PGRAY, PBLUE, PBIGRED, PBIGGRAY, PBIGBLUE, TOTAL_PARTICLE_TYPES
};
*/

class Game
{
public:
	Game();
	~Game();

	static std::map<std::string, int> Direction;
	static std::map<std::string, int> ThingType;
	static std::map<std::string, int> EntityFrameType;
	static std::map<std::string, int> EntityFrameGroup;
	static std::map<std::string, int> ObjectFrameType;
	static std::map<std::string, int> ObjectFrameGroup;
	static std::map<std::string, int> TileType;
	static std::map<std::string, int> TileSubType;
	static std::map<std::string, int> EnemyType;
	static std::map<std::string, int> CollectibleType;
	static std::map<std::string, int> BackgroundType;
	static std::map<std::string, int> MusicType;
	static std::map<std::string, int> SoundEffectType;
	static std::map<std::string, int> ParticleType;
	static std::map<std::string, int> GameState;
	static const int TILE_TYPE_OFFSET = 1000;
	static const int ENEMY_TYPE_OFFSET = 2000;
	static const int COLLECTIBLE_TYPE_OFFSET = 3000;
	static const int BACKGROUND_TYPE_OFFSET = 4000;
	static const int MUSIC_TYPE_OFFSET = 5000;
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
	// static const int TOTAL_DIRECTIONS;
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
	static SDL_Event* gEvent;
	static Audio* gAudio;
	static std::vector<Collision*> gColliding;
	static Graphics* gGraphics;
	static Player* gPlayer;
	static Game* gGame;
	static Level* gLevel;
	static LuaBridge* gLuaBridge;
	static std::vector<Thing*> things;
	static std::vector<Particle*> particles;
	static int gScore;
	static int gState;

	// static Game gGame;
	// static Level gLevel;

	static void centerCamera(void);
	static bool applyAI(void);
	// int type, int levelunit, int x (optional if levelunit is given), int y (optional if levelunit is given), int thingtype (use ONLY if offset is not used with the type)
	static void newThing(int, int, int, int, int);
	static void newParticle(SDL_Rect*, int, SDL_Point*, int, int);
	static void destroyThing(int);
	static void destroyParticle(int);

	// Thing* = first thing, Thing* = second thing (or NULL to check first thing and everything), int = level unit of first thing,
	// bool = whether to output collision to gColliding; returns whether or not there was a collision
	static bool checkCollision(Thing* = NULL, Thing* = NULL, int = -1, bool = true);

	static bool checkCollisionRects(SDL_Rect*, SDL_Rect*);
};