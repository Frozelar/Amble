// Include all the external libraries and whatnot that we'll be needing
#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <random>
#include <time.h>
#include <vector>
#include <map>

// #include "Game.h"

class Texture;
class Thing;
class Tile;
class Enemy;
class Collectible;
class Player;
class Graphics;
class Audio;
class Game;
class Particle;
class Level;
class LuaBridge;
class Menu;
class LevelEditor;

struct Collision {
	Thing* thing1 = NULL;
	Thing* thing2 = NULL;
};

struct ControlScheme {
	int up = SDLK_w;
	int left = SDLK_a;
	int down = SDLK_s;
	int right = SDLK_d;
	int jump = SDLK_SPACE;
	int pause = SDLK_p;
};

// enum Direction;

bool init(void);
bool initFont(void);
void close(void);

int invertDir(int);