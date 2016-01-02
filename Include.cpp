#include "Include.h"
#include "Thing.h"
#include "LuaBridge.h"

//struct Collision{
//	Thing* thing1 = NULL;
//	Thing* thing2 = NULL;
//};

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

	Game::gFont = TTF_OpenFont("resources/AveriaSans-Regular.ttf", 18);
	if (Game::gFont == NULL)
	{
		std::cout << "Error opening the font:" << std::endl;
		std::cout << TTF_GetError() << std::endl;
		return false;
	}

	for (int i = 0; i < (int)Game::things.size(); i++)
	{
		Game::things[i] = NULL;
		Game::gColliding[i] = NULL;
	}

	return true;
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

	LuaBridge::labClose();

	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
	SDL_DestroyRenderer(Game::gRenderer);
	SDL_DestroyWindow(Game::gWindow);
	Game::gWindow = NULL;
	Game::gRenderer = NULL;
}

/*
Direction invertDir(Direction dir)
{
	switch (dir)
	{
	case LEFT:
		return RIGHT;
	case RIGHT:
		return LEFT;
	case UP:
		return DOWN;
	case DOWN:
		return UP;
	default:
		return dir;
	}
}
*/