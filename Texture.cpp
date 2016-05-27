#include "Texture.h"

Texture::Texture(int px, int py, int pw, int ph)
{
	// set properties
	// txRect = new SDL_Rect();
	txRect = { px, py, pw, ph };
}

Texture::~Texture()
{
	// free everything
	// delete txRect;
	txFree();
}

void Texture::txColor(Uint8 r, Uint8 g, Uint8 b)
{
	// set colors to the texture
	SDL_SetTextureColorMod(txTexture, r, g, b);
}

void Texture::txBlend(SDL_BlendMode b)
{
	// set the blend mode
	SDL_SetTextureBlendMode(txTexture, b);
}

void Texture::txAlpha(Uint8 a)
{
	// set alpha to the texture
	SDL_SetTextureAlphaMod(txTexture, a);
}

bool Texture::txLoadF(std::string path)
{
	// keeps track of whether a failure occurred
	bool success = true;

	// free the current texture (if it exists)
	txFree();

	// the texture that we will (hopefully) end up with
	SDL_Texture* createdTexture = NULL;

	// the surface that the texture's image will be loaded onto
	SDL_Surface* imageSurface = IMG_Load(path.c_str());

	if (imageSurface == NULL)
	{
		// output a message if the surface failed to load
		std::cout << "Error loading image at the specified path: " << path << std::endl;
		std::cout << "IMG_GetError: " << IMG_GetError() << std::endl;
		success = false;
	}
	else
	{
		// set the color key
		// SDL_SetColorKey(imageSurface, SDL_TRUE, SDL_MapRGB(imageSurface->format, 0, 255, 255));

		// create the texture itself from the surface
		createdTexture = SDL_CreateTextureFromSurface(Game::gRenderer, imageSurface);

		if (createdTexture == NULL)
		{
			// output an error
			std::cout << "Error creating texture from surface.  SDL_GetError: " << SDL_GetError() << std::endl;
		}
		else
		{
			if (txRect.w == NULL || txRect.h == NULL)
			{
				txRect.w = imageSurface->w;
				txRect.h = imageSurface->h;
			}
		}
	}

	// the surface isn't needed anymore
	SDL_FreeSurface(imageSurface);

	// make the texture member point to the newly created texture
	txTexture = createdTexture;

	// return whether everything ran successfully or not
	return success;
}

bool Texture::txLoadT(std::string text, TTF_Font* font, SDL_Color color)
{
	//Get rid of preexisting texture
	txFree();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), color, 1024);
	if (textSurface != NULL)
	{
		//Create texture from surface pixels
		txTexture = SDL_CreateTextureFromSurface(Game::gRenderer, textSurface);
		if (txTexture == NULL)
		{
			std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
		}
		else
		{
			if (txRect.w == NULL || txRect.h == NULL)
			{
				txRect.w = textSurface->w;
				txRect.h = textSurface->h;
			}
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
	else
	{
		std::cout << "Unable to render text surface! SDL_TTF Error: " << TTF_GetError() << std::endl;
	}

	//Return success
	return txTexture != NULL;
}

void Texture::txRender(SDL_Rect *clip, SDL_Point *center, double rotation, SDL_RendererFlip flip)
{
	// rect to render to
	SDL_Rect renderRect{ txRect.x, txRect.y, txRect.w, txRect.h };

	// if clip is given, set properties
	if (clip != NULL)
	{
		renderRect.w = clip->w;
		renderRect.h = clip->h;
	}

	// render to the screen
	SDL_RenderCopyEx(Game::gRenderer, txTexture, clip, &renderRect, rotation, center, flip);
}

void Texture::txFree()
{
	// destroy the texture
	if (txTexture != NULL)
	{
		SDL_DestroyTexture(txTexture);
	}
}