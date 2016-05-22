#include "Include.h"
#include "Game.h"
#include "Luabridge.h"
#include "Thing.h"

const int PI = 3.141592653;

/*
class Collision {
	Thing* thing1 = NULL;
	Thing* thing2 = NULL;
};
*/

bool init(void)
{
	srand(time(NULL));

	// initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < NULL)
	{
		std::cout << "SDL error:" << std::endl;
		std::cout << SDL_GetError() << std::endl;
		return false;
	}

	// initialize SDL_image to load png images
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		std::cout << "SDL_Image error:" << std::endl;
		std::cout << IMG_GetError() << std::endl;
		return false;
	}

	//Game::gAudio.auInit();
	//Game::gGraphics.gxInit();

	// initialize SDL_TTF for text
	if (TTF_Init() == -1)
	{
		std::cout << "SDL_TTF error:" << std::endl;
		std::cout << TTF_GetError() << std::endl;
		return false;
	}

	/*
	Game::gFont = TTF_OpenFont(font.c_str(), Game::gFontSize);
	if (Game::gFont == NULL)
	{
		std::cout << "Error opening the font:" << std::endl;
		std::cout << TTF_GetError() << std::endl;
		return false;
	}
	*/

	for (int i = 0; i < (int)Game::things.size(); i++)
	{
		Game::things[i] = NULL;
		Game::gColliding[i] = NULL;
	}

	return true;
}

bool initFont(void)
{
	std::string hfont = Game::rDir + Game::gHeadingFont.name;
	std::string bfont = Game::rDir + Game::gBodyFont.name;
	std::string tfont = Game::rDir + Game::gTitleFont.name;

	Game::gHeadingFont.font = TTF_OpenFont(hfont.c_str(), Game::gHeadingFont.size);
	Game::gBodyFont.font = TTF_OpenFont(bfont.c_str(), Game::gBodyFont.size);
	Game::gTitleFont.font = TTF_OpenFont(tfont.c_str(), Game::gTitleFont.size);
	if (Game::gHeadingFont.font == NULL || Game::gBodyFont.font == NULL || Game::gTitleFont.font == NULL)
	{
		std::cout << "Error opening font: " << std::endl;
		std::cout << TTF_GetError() << std::endl;
		return false;
	}
}

void close(void)
{
	for (int i = 0; i < (int)Game::things.size(); i++)
	{
		if (Game::things[i] != NULL)
		{
			if (Game::things[i]->tgType != Game::ThingType["player"])
				delete Game::things[i];
			Game::things[i] = NULL;
		}
	}

	delete Game::gGame;
	LuaBridge::labClose();

	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	TTF_CloseFont(Game::gHeadingFont.font);
	TTF_CloseFont(Game::gBodyFont.font);
	TTF_CloseFont(Game::gTitleFont.font);
	Game::gHeadingFont.font = NULL;
	Game::gBodyFont.font = NULL;
	Game::gTitleFont.font = NULL;
	TTF_Quit();
	SDL_Quit();
	SDL_DestroyRenderer(Game::gRenderer);
	SDL_DestroyWindow(Game::gWindow);
	Game::gWindow = NULL;
	Game::gRenderer = NULL;
}

int invertDir(int dir)
{
	if (dir == Game::Direction["up"])
		return Game::Direction["down"];
	else if (dir == Game::Direction["down"])
		return Game::Direction["up"];
	else if (dir == Game::Direction["left"])
		return Game::Direction["right"];
	else if (dir == Game::Direction["right"])
		return Game::Direction["left"];
	return -1;
}

// takes a polar point (with theta expressed in DEGREES) and converts it to a rectangular point
SDL_Point polarToRect(PolarPoint coord)
{
	SDL_Point rect = { coord.r * cos(coord.angle * PI / 180), coord.r * sin(coord.angle * PI / 180) };
	return rect;
}

// takes a rectangular point and converts it to a polar point (with theta expressed in DEGREES)
PolarPoint rectToPolar(SDL_Point coord)
{
	PolarPoint polar = { (int)sqrt(coord.x * coord.x + coord.y * coord.y), (int)atan(coord.y / coord.x) * 180 / PI };
	if (polar.angle < 0)
		polar.angle += 360;
	return polar;
}