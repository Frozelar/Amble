#include "luasource/lua.hpp"
#include "Include.h"
#include "Player.h"
#include "Game.h"

int main(int argc, char** argv)
{
	bool quit = false;

	// lua_State* S = luaL_newstate();
	// luaL_openlibs(S);

	// init();
	Level::generateLevel();
	LuaBridge::labInitValues();
	while (!quit)
	{
		// labHandleEnvironment needs to be called before applyAI 
		// since otherwise collectibles would be deleted before clCollect is called
		LuaBridge::labHandleEnvironment();
		while (SDL_PollEvent(&Game::gEvent) != NULL)
		{
			if (Game::gEvent.type == SDL_QUIT)
			{
				quit = true;
			}
			else
			{
				Game::gPlayer.plHandleEvent(&Game::gEvent);
			}
		}
		Game::gPlayer.plMove();
		Game::applyAI();
		Level::moveLevel();
		Graphics::gxRender();
	}

	close();

	return 0;
}