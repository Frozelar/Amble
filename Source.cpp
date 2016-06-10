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
	Uint32 curfps = 0;
	
	Game::gEvent = new SDL_Event();
	Game::gGame = new Game();
	Game::gPlayer = new Player();
	Game::gLevel = new Level();
	Game::gLuaBridge = new LuaBridge();
	// Level::generateLevel();
	// Game::gLuaBridge = new LuaBridge();
	LuaBridge::labInitValues();
	// LuaBridge::labChangeLevel();
	initFont();
	Game::gAudio = new Audio();
	Game::gGraphics = new Graphics();
	Game::gLevelEditor = new LevelEditor();
	Game::gMenu = new Menu();
	// Level::playMusic();
	// lua_State* S = luaL_newstate();
	// luaL_openlibs(S);
	// init();
	// Level::generateLevel();
	// Graphics::gxInit();

	Game::readCFG();
	Game::gState = Game::GameState["title"];
	while (!quit)
	{
		curfps = SDL_GetTicks();
		if (Game::gState == Game::GameState["game"])
		{
			if (needtoinitlevel)
			{
				Level::generateLevel(Level::currentLevel + 1);	// may need to change later?
				if (Graphics::updatedGFX)
				{
					Graphics::gxUpdateThingScale();
					Graphics::updatedGFX = false;
				}
				Game::centerCamera();
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
			if (needtoinitlevel)
			{
				LevelEditor::leEnter();
				needtoinitlevel = false;
			}
			while (SDL_PollEvent(Game::gEvent) != NULL)
			{
				if (Game::gEvent->type == SDL_QUIT)
					quit = true;
				else
				{
					if (!quit)
						quit = !LevelEditor::leHandleEnvironment(Game::gEvent);
					else
						LevelEditor::leHandleEnvironment(Game::gEvent);
				}
			}
			LevelEditor::leMoveLevel();
			LevelEditor::leRender();
		}

		if (1000 / Game::FPS > SDL_GetTicks() - curfps)
			SDL_Delay((1000 / Game::FPS) - (SDL_GetTicks() - curfps));

		if (!needtoinitlevel && (/* Game::gState == Game::GameState["editor"] || */ Game::gState == Game::GameState["title"]))
			needtoinitlevel = true;
			// if (Game::gOldState == Game::GameState["editor"])
			// 	LevelEditor::leExit();
	}
	close();

	return 0;
}