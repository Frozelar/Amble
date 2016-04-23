#include "Menu.h"
#include "Game.h"
#include "Level.h"
#include "Graphics.h"
#include "Texture.h"

Menu* Game::gMenu;

int Menu::muMenu = 0;
int Menu::muInitialized = false;
std::map<std::string, int> Menu::MenuID;
std::vector< int > Menu::NumOptions;
Texture* Menu::menuTexture;
std::vector<std::string> Menu::muOptions = { "Resume", "Settings", "Return to Title", "Quit", "Graphics", "Audio", "Controls", "Return" };
std::vector<std::string> Menu::ttOptions = { "Play Game", "Menu", "Level Editor", "Quit" };
std::vector< Texture* > Menu::muOptionTextures;
std::vector< Texture* > Menu::ttOptionTextures;

Menu::Menu()
{
	int num = 0;
	MenuID["pause"] = (num = 0);
	MenuID["settings"] = (++num);
	NumOptions.resize(MenuID.size());
	NumOptions[MenuID["pause"]] = 4;
	NumOptions[MenuID["settings"]] = 4;

	menuTexture = new Texture();
	menuTexture->txLoadF(Game::rDir + "Menu" + Game::rExt);
	menuTexture->txRect.w = Game::WINDOW_W / 2;
	menuTexture->txRect.h = Game::WINDOW_H / 2;
	menuTexture->txRect.x = (Game::WINDOW_W - menuTexture->txRect.w) / 2;
	menuTexture->txRect.y = (Game::WINDOW_H - menuTexture->txRect.h) / 2;

	for (int i = 0; i < muOptions.size(); i++)
	{
		muOptionTextures.resize(i + 1);
		muOptionTextures[i] = new Texture(0, 0, 0, 0);
		muOptionTextures[i]->txLoadT(muOptions[i], Game::textColor);
		muOptionTextures[i]->txRect.x = menuTexture->txRect.x + Game::DEFAULT_W;
		if (i == 0 || i == NumOptions[MenuID["pause"]])
			muOptionTextures[i]->txRect.y = menuTexture->txRect.y + Game::DEFAULT_H;
		else
			muOptionTextures[i]->txRect.y = muOptionTextures[i - 1]->txRect.y + muOptionTextures[i - 1]->txRect.h + Game::DEFAULT_H;
	}
	for (int i = 0; i < ttOptions.size(); i++)
	{
		ttOptionTextures.resize(i + 1);
		ttOptionTextures[i] = new Texture(0, 0, 0, 0);
		ttOptionTextures[i]->txLoadT(ttOptions[i], Game::textColor);
		ttOptionTextures[i]->txRect.x = (Game::WINDOW_W - ttOptionTextures[i]->txRect.w) / 2;
		if (i == 0)
			ttOptionTextures[i]->txRect.y = (Game::WINDOW_H - ttOptionTextures[i]->txRect.w) / 2;
		else
			ttOptionTextures[i]->txRect.y = ttOptionTextures[i - 1]->txRect.y + ttOptionTextures[i - 1]->txRect.h + Game::DEFAULT_H;
	}
}

Menu::~Menu()
{
	delete menuTexture;
	for (int i = 0; i < muOptionTextures.size(); i++)
		delete muOptionTextures[i];
	for (int i = 0; i < ttOptionTextures.size(); i++)
		delete ttOptionTextures[i];
}

void Menu::muCreateMenu(void)
{
	muInitialized = true;
	if (Game::gState == Game::GameState["title"])
	{
		Level::generateLevel(0);	// 0 = title screen
		// if (Graphics::bgState != Game::BackgroundType["Title"])
		// 	Graphics::bgState = Game::BackgroundType["Title"];
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
	if (Game::gState == Game::GameState["menu"])
	{
		if (e->type == SDL_KEYUP && e->key.repeat == NULL)
		{
			switch (e->key.keysym.sym)
			{
			case SDLK_p:
				muMenu = MenuID["pause"];
				Game::changeGameState(Game::gOldState);
				muInitialized = false;
				break;
			}
		}
		else if (e->type == SDL_MOUSEMOTION)
		{
			for (int i = 0; i < muOptionTextures.size(); i++)
			{
				if (Game::checkCollisionRects(&mouse, &muOptionTextures[i]->txRect))
					muOptionTextures[i]->txColor(Game::highlightColor.r, Game::highlightColor.g, Game::highlightColor.b);
				else
					muOptionTextures[i]->txColor(255, 255, 255);
			}
		}
		else if (e->type == SDL_MOUSEBUTTONDOWN)
		{
			for (int i = 0; i < muOptionTextures.size(); i++)
				muOptionTextures[i]->txColor(255, 255, 255);
		}
		else if (e->type == SDL_MOUSEBUTTONUP)
		{
			for (int i = menuPos; i < menuPos + NumOptions[muMenu]; i++)
				if (Game::checkCollisionRects(&mouse, &muOptionTextures[i]->txRect))
				{
					if (muOptions[i] == "Resume")
						Game::changeGameState(Game::gOldState);
					else if (muOptions[i] == "Settings")
						muMenu = MenuID["settings"];
					else if (muOptions[i] == "Return to Title")
					{
						Game::changeGameState(Game::GameState["title"]);
						muCreateMenu();
					}
					else if (muOptions[i] == "Quit")
						return false;
					// else if (muOptions[i] == "Graphics") ...
					else if (muOptions[i] == "Return")
						muMenu = MenuID["pause"];
				}
		}
	}
	else if (Game::gState == Game::GameState["title"])
	{
		if (e->type == SDL_MOUSEMOTION)
		{
			for (int i = 0; i < ttOptionTextures.size(); i++)
			{
				if (Game::checkCollisionRects(&mouse, &ttOptionTextures[i]->txRect))
					ttOptionTextures[i]->txColor(Game::highlightColor.r, Game::highlightColor.g, Game::highlightColor.b);
				else
					ttOptionTextures[i]->txColor(255, 255, 255);
			}
		}
		else if (e->type == SDL_MOUSEBUTTONDOWN)
		{
			for (int i = 0; i < ttOptionTextures.size(); i++)
				ttOptionTextures[i]->txColor(255, 255, 255);
		}
		else if (e->type == SDL_MOUSEBUTTONUP)
		{
			for (int i = 0; i < ttOptions.size(); i++)
			{
				if (Game::checkCollisionRects(&mouse, &ttOptionTextures[i]->txRect))
				{
					if (ttOptions[i] == "Play Game")
						Game::changeGameState(Game::GameState["game"]);
					else if (ttOptions[i] == "Menu")
						Game::changeGameState(Game::GameState["menu"]);
					else if (ttOptions[i] == "Level Editor")
						Game::changeGameState(Game::GameState["editor"]);
					else if (ttOptions[i] == "Quit")
						return false;
				}
			}
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
			muOptionTextures[i]->txRender();
	}
	else if (Game::gState == Game::GameState["title"])
	{
		for (int i = 0; i < ttOptionTextures.size(); i++)
			ttOptionTextures[i]->txRender();
	}
	SDL_RenderPresent(Game::gRenderer);
	// if(Game::gState == Game::GameState["menu"])
	//	Graphics::menuTexture->txRender();
}