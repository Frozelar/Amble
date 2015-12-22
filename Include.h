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

struct Collision{
	Thing* thing1 = NULL;
	Thing* thing2 = NULL;
};

enum Direction;

bool init(void);
void close(void);

Direction invertDir(Direction);