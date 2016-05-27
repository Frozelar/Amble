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

struct Font {
	TTF_Font* font;
	SDL_Color color;
	std::string name;
	int size;
};

struct Collision {
	Thing* thing1 = NULL;
	Thing* thing2 = NULL;
};

struct PolarPoint {
	PolarPoint(int pr = NULL, int pang = NULL) { r = pr; angle = pang; }
	int r = 0;
	int angle = 0;
};

/*
struct ControlScheme {
	int up = SDLK_w;
	int left = SDLK_a;
	int down = SDLK_s;
	int right = SDLK_d;
	int jump = SDLK_SPACE;
	int pause = SDLK_p;
};
*/

// enum Direction;

bool init(void);
bool initFont(void);
void close(void);

int invertDir(int);
SDL_Point polarToRect(PolarPoint);
PolarPoint rectToPolar(SDL_Point);
SDL_Rect multDimensions(SDL_Rect, float);