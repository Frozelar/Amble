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
	bool txLoadT(std::string textureText, SDL_Color textColor);

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