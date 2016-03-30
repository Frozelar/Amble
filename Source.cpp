#include "luasource/lua.hpp"
#include "Include.h"
#include "Game.h"
#include "Graphics.h"
#include "Level.h"
#include "LuaBridge.h"
#include "Player.h"
#include "Audio.h"

int main(int argc, char** argv)
{
	bool quit = false;
	
	Game::gEvent = new SDL_Event();
	Game::gGame = new Game();
	Game::gPlayer = new Player();
	Game::gLevel = new Level();
	Game::gLuaBridge = new LuaBridge();
	Level::generateLevel();
	// Game::gLuaBridge = new LuaBridge();
	LuaBridge::labInitValues();
	Game::gAudio = new Audio();
	Game::gGraphics = new Graphics();
	Level::playMusic();
	// lua_State* S = luaL_newstate();
	// luaL_openlibs(S);

	// init();
	// Level::generateLevel();
	// Graphics::gxInit();
	while (!quit)
	{
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
		Game::applyAI();
		Game::gPlayer->plMove();
		Level::moveLevel();
		Graphics::gxRender();
	}

	close();

	return 0;
}