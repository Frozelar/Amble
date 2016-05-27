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


#pragma once
#include "Include.h"
#include "Game.h"

class Texture
{
public:

	// constructor to initialize properties
	Texture(int = 2, int = 2, int = Game::DEFAULT_W, int = Game::DEFAULT_H);

	// deconstructor to destroy everything
	~Texture();

	// render the texture to the screen
	void txRender(SDL_Rect* clip = NULL, SDL_Point* center = NULL,
		double rotation = 0.0, SDL_RendererFlip flip = SDL_FLIP_NONE);

	// load up a texture from a file
	bool txLoadF(std::string);

	//Creates image from font string
	bool txLoadT(std::string textureText, TTF_Font* textFont, SDL_Color textColor);

	// free everything
	void txFree(void);

	// set color using red, green, blue
	void txColor(Uint8 r, Uint8 g, Uint8 b);

	// set transparency (aka alpha)
	void txAlpha(Uint8 a);

	// set blend mode
	void txBlend(SDL_BlendMode b);

	SDL_Texture* txTexture = NULL;
	SDL_Rect txRect;
};