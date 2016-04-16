#include "Menu.h"
#include "Game.h"
#include "Graphics.h"
#include "Texture.h"

int Menu::muInitialized = false;

Menu::Menu()
{
}

Menu::~Menu()
{
}

void Menu::muCreateMenu(void)
{
	muInitialized = true;
	if (Game::gState == Game::GameState["title"])
	{
		if (Graphics::bgState != Game::BackgroundType["Title"])
			Graphics::bgState = Game::BackgroundType["Title"];
	}
	else if (Game::gState == Game::GameState["menu"])
	{
		//if (Graphics::bgState != Game::BackgroundType["Menu"])
		//	Graphics::bgState = Game::BackgroundType["Menu"];
		Graphics::menuTexture->txRect.w = Game::WINDOW_W / 2;
		Graphics::menuTexture->txRect.h = Game::WINDOW_H / 2;
		Graphics::menuTexture->txRect.x = (Game::WINDOW_W - Graphics::menuTexture->txRect.w) / 2;
		Graphics::menuTexture->txRect.y = (Game::WINDOW_H - Graphics::menuTexture->txRect.h) / 2;
	}

}

void Menu::muHandleMenu(SDL_Event* e)
{
	if (e->type == SDL_KEYUP && e->key.repeat == NULL)
	{
		switch (e->key.keysym.sym)
		{
		case SDLK_p:
			if (Game::gState == Game::GameState["menu"])
			{
				Game::gState = Game::GameState["game"];
				muInitialized = false;
			}
			break;
		}
	}
}

void Menu::muRender(void)
{
	Graphics::gxRender();
	// if(Game::gState == Game::GameState["menu"])
	//	Graphics::menuTexture->txRender();
}