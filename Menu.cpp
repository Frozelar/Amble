#include "Menu.h"
#include "Game.h"
#include "Level.h"
#include "Graphics.h"
#include "Audio.h"
#include "Texture.h"
#include "Player.h"

Menu* Game::gMenu;

int Menu::muMenu = 0;
// int Menu::muOldMenu = 0;
int Menu::muInitialized = false;
bool Menu::muIsMapping = false;
std::map<std::string, int> Menu::MenuID;
std::vector< int > Menu::NumOptions;
Texture* Menu::ttTitleTexture;
Texture* Menu::menuTexture;
std::vector<std::string> Menu::muOptions = { "Resume", "Settings", "Return to Title", "Quit", "",
	"Graphics", "Audio", "Controls", "Return", "",
	"Window Size", "Toggle Fullscreen", "Return", "",
	"Music", "Sound Effects", "Return", "",
	"Left", "Right", "Up", "Down", "Jump", "Pause", "Return", "" };
std::vector<std::string> Menu::ttOptions = { "Play Game", "Menu", "Level Editor", "Quit" };
std::vector< Texture* > Menu::muOptionTextures;
std::vector< Texture* > Menu::ttOptionTextures;
std::vector< Texture* > Menu::muMiscTextures;	// sfx, volume
const int Menu::MISC_BUTTON_INDEX = 2;

Menu::Menu()
{
	int num = 0;
	int controlMenuPos = 0;
	MenuID["pause"] = (num = 0);
	MenuID["settings"] = (++num);
	MenuID["graphics"] = (++num);
	MenuID["audio"] = (++num);
	MenuID["controls"] = (++num);
	NumOptions.resize(MenuID.size());
	NumOptions[MenuID["pause"]] = 4;
	NumOptions[MenuID["settings"]] = 4;
	NumOptions[MenuID["graphics"]] = 3;
	NumOptions[MenuID["audio"]] = 3;
	NumOptions[MenuID["controls"]] = 7;

	for (int i = 0; i < MenuID["controls"]; i++)
		controlMenuPos += NumOptions[i] + 1;

	ttTitleTexture = new Texture(0, 0, 0, 0);
	ttTitleTexture->txLoadT(Game::gameTitle, Game::gTitleFont.font, Game::gTitleFont.color);
	ttTitleTexture->txRect.x = (Game::WINDOW_W - ttTitleTexture->txRect.w) / 2;
	ttTitleTexture->txRect.y = Game::DEFAULT_H;

	menuTexture = new Texture();
	menuTexture->txLoadF(Game::rDir + "Menu" + Game::rExt);
	menuTexture->txRect.w = Game::WINDOW_W / 2;
	menuTexture->txRect.h = Game::WINDOW_H / 2;
	menuTexture->txRect.x = (Game::WINDOW_W - menuTexture->txRect.w) / 2;
	menuTexture->txRect.y = (Game::WINDOW_H - menuTexture->txRect.h) / 2;

	muMiscTextures.resize(3);
	for (int i = 0; i < muMiscTextures.size(); i++)
		muMiscTextures[i] = new Texture(0, 0, 0, 0);
	muMiscTextures[MISC_BUTTON_INDEX]->txLoadT("Press Button", Game::gHeadingFont.font, Game::gHeadingFont.color);
	muMiscTextures[MISC_BUTTON_INDEX]->txRect.x = (Game::WINDOW_W - muMiscTextures[MISC_BUTTON_INDEX]->txRect.w) / 2;
	muMiscTextures[MISC_BUTTON_INDEX]->txRect.y = (Game::WINDOW_H - muMiscTextures[MISC_BUTTON_INDEX]->txRect.h) / 2;

	for (int i = 0; i < muOptions.size(); i++)
	{
		muOptionTextures.resize(i + 1);
		if (muOptions[i] != "")
		{
			muOptionTextures[i] = new Texture(0, 0, 0, 0);
			if (muOptions[i] == "Music" || muOptions[i] == "Sound Effects")
				muOptionTextures[i]->txLoadT(muOptions[i] + ": ", Game::gHeadingFont.font, Game::gHeadingFont.color);
			else if (i >= controlMenuPos && i < controlMenuPos + NumOptions[MenuID["controls"]])
				muOptionTextures[i]->txLoadT(muOptions[i], Game::gBodyFont.font, Game::gBodyFont.color);
			else
				muOptionTextures[i]->txLoadT(muOptions[i], Game::gHeadingFont.font, Game::gHeadingFont.color);
			muOptionTextures[i]->txRect.x = menuTexture->txRect.x + Game::DEFAULT_W;
			if (i == 0 || muOptions[i - 1] == "")
				muOptionTextures[i]->txRect.y = menuTexture->txRect.y + Game::DEFAULT_H;
			else
				muOptionTextures[i]->txRect.y = muOptionTextures[i - 1]->txRect.y + muOptionTextures[i - 1]->txRect.h + Game::DEFAULT_H;
			if (muOptions[i] == "Music" || muOptions[i] == "Sound Effects")
			{
				muMiscTextures[(muOptions[i] == "Sound Effects" ? Audio::SFX_VOL_INDEX : Audio::MUSIC_VOL_INDEX)]->txRect.x = muOptionTextures[i]->txRect.x + muOptionTextures[i]->txRect.w + Game::DEFAULT_W;
				muMiscTextures[(muOptions[i] == "Sound Effects" ? Audio::SFX_VOL_INDEX : Audio::MUSIC_VOL_INDEX)]->txRect.y = muOptionTextures[i]->txRect.y;
			}
		}
		else
			muOptionTextures[i] = NULL;
	}
	for (int i = 0; i < ttOptions.size(); i++)
	{
		ttOptionTextures.resize(i + 1);
		ttOptionTextures[i] = new Texture(0, 0, 0, 0);
		ttOptionTextures[i]->txLoadT(ttOptions[i], Game::gHeadingFont.font, Game::gHeadingFont.color);
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
		if(muOptionTextures[i] != NULL)
			delete muOptionTextures[i];
	for (int i = 0; i < ttOptionTextures.size(); i++)
		delete ttOptionTextures[i];
	for (int i = 0; i < muMiscTextures.size(); i++)
		delete muMiscTextures[i];
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
		menuPos += NumOptions[i] + 1;

	if (muIsMapping)
	{
		if (e->type == SDL_KEYUP && e->key.repeat == NULL)
			muMapButton("", e->key.keysym.sym);
		return true;
	}
	if (Game::gState == Game::GameState["menu"])
	{
		if (e->type == SDL_KEYUP && e->key.repeat == NULL)
		{
			if (e->key.keysym.sym == Game::gPlayer->plControls.pause)
			{
				muMenu = MenuID["pause"];
				Game::changeGameState(Game::gOldState);
				muInitialized = false;
				Game::writeCFG();
			}
		}
		else if (e->type == SDL_MOUSEMOTION)
		{
			for (int i = 0; i < muOptionTextures.size(); i++)
			{
				if (muOptionTextures[i] != NULL)
				{
					if (Game::checkCollisionRects(&mouse, &muOptionTextures[i]->txRect))
						muOptionTextures[i]->txColor(Game::highlightColor.r, Game::highlightColor.g, Game::highlightColor.b);
					else
						muOptionTextures[i]->txColor(255, 255, 255);
				}
			}
		}
		else if (e->type == SDL_MOUSEBUTTONDOWN)
		{
			for (int i = 0; i < muOptionTextures.size(); i++)
				if (muOptionTextures[i] != NULL)
				{
					if (Game::checkCollisionRects(&mouse, &muOptionTextures[i]->txRect))
						muOptionTextures[i]->txColor(Game::highlightColor.r, Game::highlightColor.g, Game::highlightColor.b);
					else
						muOptionTextures[i]->txColor(255, 255, 255);
				}
		}
		else if (e->type == SDL_MOUSEBUTTONUP)
		{
			for (int i = menuPos; i < menuPos + NumOptions[muMenu]; i++)
				if (muOptionTextures[i] != NULL)
				{
					if (Game::checkCollisionRects(&mouse, &muOptionTextures[i]->txRect))
					{
						muOptionTextures[i]->txColor(255, 255, 255);
						if (muMenu == MenuID["pause"])
						{
							if (muOptions[i] == "Resume")
							{
								Game::writeCFG();
								Game::changeGameState(Game::gOldState);
							}
							else if (muOptions[i] == "Settings")
							{
								// muOldMenu = muMenu;
								muMenu = MenuID["settings"];
							}
							else if (muOptions[i] == "Return to Title")
							{
								Game::writeCFG();
								Game::changeGameState(Game::GameState["title"]);
								muCreateMenu();
							}
							else if (muOptions[i] == "Quit")
							{
								Game::writeCFG();
								return false;
							}
							// else if (muOptions[i] == "Graphics") ...
						}
						else if (muMenu == MenuID["settings"])
						{
							if (muOptions[i] == "Graphics")
							{
								// muOldMenu = muMenu;
								muMenu = MenuID["graphics"];
							}
							else if (muOptions[i] == "Audio")
							{
								muMiscTextures[Audio::SFX_VOL_INDEX]->txRect.w = 0; muMiscTextures[Audio::SFX_VOL_INDEX]->txRect.h = 0;
								muMiscTextures[Audio::MUSIC_VOL_INDEX]->txRect.w = 0; muMiscTextures[Audio::MUSIC_VOL_INDEX]->txRect.h = 0;
								muMiscTextures[Audio::SFX_VOL_INDEX]->txLoadT(std::to_string(Audio::volume[Audio::SFX_VOL_INDEX]), Game::gHeadingFont.font, Game::gHeadingFont.color);
								muMiscTextures[Audio::MUSIC_VOL_INDEX]->txLoadT(std::to_string(Audio::volume[Audio::MUSIC_VOL_INDEX]), Game::gHeadingFont.font, Game::gHeadingFont.color);
								Menu::muMiscTextures[Audio::SFX_VOL_INDEX]->txRect.w *= Graphics::GFX_MULT; Menu::muMiscTextures[Audio::SFX_VOL_INDEX]->txRect.h *= Graphics::GFX_MULT;
								Menu::muMiscTextures[Audio::MUSIC_VOL_INDEX]->txRect.w *= Graphics::GFX_MULT; Menu::muMiscTextures[Audio::MUSIC_VOL_INDEX]->txRect.h *= Graphics::GFX_MULT;
								muMenu = MenuID["audio"];
							}
							else if (muOptions[i] == "Controls")
							{
								muMenu = MenuID["controls"];
							}
							else if (muOptions[i] == "Return")
							{
								muMenu--; // = muOldMenu;
							}
						}
						else if (muMenu == MenuID["graphics"])
						{
							if (muOptions[i] == "Window Size")
							{
								Graphics::gxIncScale();
							}
							else if (muOptions[i] == "Toggle Fullscreen")
							{
								Graphics::gxToggleFullscreen();
							}
							else if (muOptions[i] == "Return")
								muMenu--; // = muOldMenu;
						}
						else if (muMenu == MenuID["audio"])
						{
							if (muOptions[i] == "Music")
							{
								Audio::auIncVolume(Audio::MUSIC_VOL_INDEX);
								muMiscTextures[Audio::MUSIC_VOL_INDEX]->txRect.w = 0; muMiscTextures[Audio::MUSIC_VOL_INDEX]->txRect.h = 0;
								muMiscTextures[Audio::MUSIC_VOL_INDEX]->txLoadT(std::to_string(Audio::volume[Audio::MUSIC_VOL_INDEX]), Game::gHeadingFont.font, Game::gHeadingFont.color);
								Menu::muMiscTextures[Audio::MUSIC_VOL_INDEX]->txRect.w *= Graphics::GFX_MULT; Menu::muMiscTextures[Audio::MUSIC_VOL_INDEX]->txRect.h *= Graphics::GFX_MULT;
							}
							else if (muOptions[i] == "Sound Effects")
							{
								Audio::auIncVolume(Audio::SFX_VOL_INDEX);
								muMiscTextures[Audio::SFX_VOL_INDEX]->txRect.w = 0; muMiscTextures[Audio::SFX_VOL_INDEX]->txRect.h = 0;
								muMiscTextures[Audio::SFX_VOL_INDEX]->txLoadT(std::to_string(Audio::volume[Audio::SFX_VOL_INDEX]), Game::gHeadingFont.font, Game::gHeadingFont.color);
								muMiscTextures[Audio::SFX_VOL_INDEX]->txRect.w *= Graphics::GFX_MULT; muMiscTextures[Audio::SFX_VOL_INDEX]->txRect.h *= Graphics::GFX_MULT;
							}
							else if (muOptions[i] == "Return")
								muMenu -= 2;
						}
						else if (muMenu == MenuID["controls"])
						{
							if (muOptions[i] == "Return")
							{
								muMenu -= 3;
							}
							else
							{
								if(!muIsMapping)
									muIsMapping = true;
								muMapButton(muOptions[i], e->key.keysym.sym);
							}
						}
					}
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
			{
				if (Game::checkCollisionRects(&mouse, &ttOptionTextures[i]->txRect))
					ttOptionTextures[i]->txColor(Game::highlightColor.r, Game::highlightColor.g, Game::highlightColor.b);
				else
					ttOptionTextures[i]->txColor(255, 255, 255);
			}
		}
		else if (e->type == SDL_MOUSEBUTTONUP)
		{
			for (int i = 0; i < ttOptions.size(); i++)
			{
				if (Game::checkCollisionRects(&mouse, &ttOptionTextures[i]->txRect))
				{
					muOptionTextures[i]->txColor(255, 255, 255);
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
			menuPos += NumOptions[i] + 1;
		for (int i = menuPos; i < menuPos + NumOptions[muMenu]; i++)
			if (muOptionTextures[i] != NULL)
				muOptionTextures[i]->txRender();
		if (muMenu == MenuID["audio"])
		{
			muMiscTextures[Audio::SFX_VOL_INDEX]->txRender();
			muMiscTextures[Audio::MUSIC_VOL_INDEX]->txRender();
		}
		if (muIsMapping)
			muMiscTextures[MISC_BUTTON_INDEX]->txRender();
	}
	else if (Game::gState == Game::GameState["title"])
	{
		for (int i = 0; i < ttOptionTextures.size(); i++)
			ttOptionTextures[i]->txRender();
		ttTitleTexture->txRender();
	}
	SDL_RenderPresent(Game::gRenderer);
	// if(Game::gState == Game::GameState["menu"])
	//	Graphics::menuTexture->txRender();
}

void Menu::muMapButton(std::string button, int newmap)
{
	static std::string b = "";
	static int m = -1;
	if(button != "")
		b = button;
	m = newmap;

	if (b == "Left")
		Game::gPlayer->plControls.left = m;
	else if (b == "Right")
		Game::gPlayer->plControls.right = m;
	else if (b == "Up")
		Game::gPlayer->plControls.up = m;
	else if (b == "Down")
		Game::gPlayer->plControls.down = m;
	else if (b == "Jump")
		Game::gPlayer->plControls.jump = m;
	else if (b == "Pause")
		Game::gPlayer->plControls.pause = m;
	if(button == "")
		muIsMapping = false;
}