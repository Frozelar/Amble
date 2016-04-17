#include "Menu.h"
#include "Game.h"
#include "Graphics.h"
#include "Texture.h"

Menu* Game::gMenu;

int Menu::muMenu = 0;
int Menu::muInitialized = false;
std::map<std::string, int> Menu::MenuID;
std::vector< int > Menu::NumOptions;
Texture* Menu::menuTexture;
std::vector<std::string> Menu::muOptions = { "Resume", "Settings", "Quit", "Graphics", "Audio", "Controls", "Return" };
std::vector< Texture* > Menu::optionTextures;

Menu::Menu()
{
	int num = 0;
	MenuID["pause"] = (num = 0);
	MenuID["settings"] = (++num);
	NumOptions.resize(MenuID.size());
	NumOptions[MenuID["pause"]] = 3;
	NumOptions[MenuID["settings"]] = 4;

	menuTexture = new Texture();
	menuTexture->txLoadF(Game::rDir + "Menu" + Game::rExt);
	menuTexture->txRect.w = Game::WINDOW_W / 2;
	menuTexture->txRect.h = Game::WINDOW_H / 2;
	menuTexture->txRect.x = (Game::WINDOW_W - menuTexture->txRect.w) / 2;
	menuTexture->txRect.y = (Game::WINDOW_H - menuTexture->txRect.h) / 2;

	for (int i = 0; i < muOptions.size(); i++)
	{
		optionTextures.resize(i + 1);
		optionTextures[i] = new Texture(0, 0, 0, 0);
		optionTextures[i]->txLoadT(muOptions[i], Game::textColor);
		optionTextures[i]->txRect.x = menuTexture->txRect.x + Game::DEFAULT_W;
		if (i == 0 || i == NumOptions[MenuID["pause"]])
			optionTextures[i]->txRect.y = menuTexture->txRect.y + Game::DEFAULT_H;
		else
			optionTextures[i]->txRect.y = optionTextures[i - 1]->txRect.y + optionTextures[i - 1]->txRect.h + Game::DEFAULT_H;
	}
}

Menu::~Menu()
{
	delete menuTexture;
	for (int i = 0; i < optionTextures.size(); i++)
		delete optionTextures[i];
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
	}

}

bool Menu::muHandleMenu(SDL_Event* e)
{
	int menuPos = 0;
	SDL_Rect mouse{ 0, 0, 1, 1 };
	SDL_GetMouseState(&mouse.x, &mouse.y);
	for (int i = 0; i < muMenu; i++)
		menuPos += NumOptions[i];
	if (e->type == SDL_KEYUP && e->key.repeat == NULL)
	{
		switch (e->key.keysym.sym)
		{
		case SDLK_p:
			if (Game::gState == Game::GameState["menu"])
			{
				muMenu = MenuID["pause"];
				Game::gState = Game::GameState["game"];
				muInitialized = false;
			}
			break;
		}
	}
	else if (e->type == SDL_MOUSEMOTION)
	{
		for (int i = 0; i < optionTextures.size(); i++)
		{
			if (Game::checkCollisionRects(&mouse, &optionTextures[i]->txRect))
				optionTextures[i]->txColor(Game::highlightColor.r, Game::highlightColor.g, Game::highlightColor.b);
			else
				optionTextures[i]->txColor(255, 255, 255);
		}
	}
	else if (e->type == SDL_MOUSEBUTTONDOWN)
	{
		for (int i = 0; i < optionTextures.size(); i++)
				optionTextures[i]->txColor(255, 255, 255);
	}
	else if (e->type == SDL_MOUSEBUTTONUP)
	{
		for (int i = menuPos; i < menuPos + NumOptions[muMenu]; i++)
			if (Game::checkCollisionRects(&mouse, &optionTextures[i]->txRect))
			{
				if (muOptions[i] == "Resume")
					Game::gState = Game::GameState["game"];
				else if (muOptions[i] == "Settings")
					muMenu = MenuID["settings"];
				else if (muOptions[i] == "Quit")
					return false;
				// else if (muOptions[i] == "Graphics") ...
				else if (muOptions[i] == "Return")
					muMenu = MenuID["pause"];
			}
	}
	return true;
}

void Menu::muRender(void)
{
	int menuPos = 0;
	SDL_SetRenderDrawColor(Game::gRenderer, 105, 105, 245, 255);
	SDL_RenderClear(Game::gRenderer);
	Graphics::gxRender(false);
	if (Game::gState == Game::GameState["menu"])
	{
		menuTexture->txRender();
		for (int i = 0; i < muMenu; i++)
			menuPos += NumOptions[i];
		for (int i = menuPos; i < menuPos + NumOptions[muMenu]; i++)
			optionTextures[i]->txRender();
	}
	SDL_RenderPresent(Game::gRenderer);
	// if(Game::gState == Game::GameState["menu"])
	//	Graphics::menuTexture->txRender();
}