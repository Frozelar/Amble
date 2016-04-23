#include "luasource/lua.hpp"
#include "Include.h"
#include "Game.h"
#include "Graphics.h"
#include "Level.h"
#include "LuaBridge.h"
#include "Player.h"
#include "Audio.h"
#include "Menu.h"
#include "LevelEditor.h"

int main(int argc, char** argv)
{
	bool quit = false;
	bool needtoinitlevel = true;
	
	Game::gEvent = new SDL_Event();
	Game::gGame = new Game();
	Game::gPlayer = new Player();
	Game::gLevel = new Level();
	Game::gLuaBridge = new LuaBridge();
	// Level::generateLevel();
	// Game::gLuaBridge = new LuaBridge();
	LuaBridge::labInitValues();
	// LuaBridge::labChangeLevel();
	Game::gAudio = new Audio();
	Game::gGraphics = new Graphics();
	Game::gMenu = new Menu();
	// Level::playMusic();
	// lua_State* S = luaL_newstate();
	// luaL_openlibs(S);

	// init();
	// Level::generateLevel();
	// Graphics::gxInit();

	Game::gState = Game::GameState["title"];
	while (!quit)
	{
		if (Game::gState == Game::GameState["game"])
		{
			if (needtoinitlevel)
			{
				Level::generateLevel(Level::currentLevel + 1);	// may need to change later?
				// LuaBridge::labChangeLevel();  <--- moved to generateLevel
				// Level::playMusic();
				needtoinitlevel = false;
			}
			// labHandleEnvironment needs to be called before applyAI 
			// since otherwise collectibles would be deleted before clCollect is called
			LuaBridge::labHandleEnvironment();
			while (SDL_PollEvent(Game::gEvent) != NULL)
			{
				if (Game::gEvent->type == SDL_QUIT)
					quit = true;
				else
					Game::gPlayer->plHandleEvent(Game::gEvent);
			}
			if (!quit)
				quit = !Game::applyAI();
			else
				Game::applyAI();
			Game::gPlayer->plMove();
			Level::moveLevel();
			Graphics::gxRender();
		}
		else if (Game::gState == Game::GameState["title"] || Game::gState == Game::GameState["menu"])
		{
			// Level::generateLevel(0);		<--- moved to muCreateMenu
			if(!Menu::muInitialized)
				Menu::muCreateMenu();
			while (SDL_PollEvent(Game::gEvent) != NULL)
			{
				if (Game::gEvent->type == SDL_QUIT)
					quit = true;
				else
				{
					if (!quit)
						quit = !Menu::muHandleMenu(Game::gEvent);
					else
						Menu::muHandleMenu(Game::gEvent);
				}
			}
			Menu::muRender();
		}
		else if (Game::gState == Game::GameState["editor"])
		{

		}

		if (!needtoinitlevel && (Game::gState == Game::GameState["editor"] || Game::gState == Game::GameState["title"]))
			needtoinitlevel = true;
	}

	close();

	return 0;
}