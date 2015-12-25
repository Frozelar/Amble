#include "LuaBridge.h"
#include <sstream>

// const int LuaBridge::MAX_ARGS = 64;
// void* LuaBridge::labStack[MAX_ARGS];

lua_State* LuaBridge::L;
LuaBridge LuaBridge::gLuaBridge;

LuaBridge::LuaBridge()
{
	L = luaL_newstate();
	// for (int i = 0; i < MAX_ARGS; i++)
	//	labStack[i] = NULL;
}

LuaBridge::~LuaBridge()
{
	
}

void LuaBridge::labInitValues(void)
{
	int max = 0;
	int framenum = 0;
	std::string identifier;
	// std::vector<std::string> frames;
	std::stringstream frame;

	luaL_openlibs(L);
	int error = luaL_dofile(L, "luacode.lua");
	std::cout << "0=no errors, 1=errors: " << error /* << " " << lua_tostring(L, -1) */ << std::endl;
	// lua_pcall(L, 0, 0, 0);
	lua_register(L, "checkCollision", labCheckCollision);

	lua_getglobal(L, "WINDOW_W");
	Game::WINDOW_W = lua_tonumber(L, -1);
	lua_getglobal(L, "WINDOW_H");
	Game::WINDOW_H = lua_tonumber(L, -1);
	lua_getglobal(L, "DEFAULT_W");
	Game::DEFAULT_W = lua_tonumber(L, -1);
	lua_getglobal(L, "DEFAULT_H");
	Game::DEFAULT_H = lua_tonumber(L, -1);
	lua_getglobal(L, "PLAYER_W");
	Game::PLAYER_W = lua_tonumber(L, -1);
	lua_getglobal(L, "PLAYER_H");
	Game::PLAYER_H = lua_tonumber(L, -1);
	lua_getglobal(L, "DEFAULT_OFFSET");
	Game::DEFAULT_OFFSET = lua_tonumber(L, -1);
	lua_getglobal(L, "DEFAULT_ENEMY_W");
	Game::DEFAULT_ENEMY_W = lua_tonumber(L, -1);
	lua_getglobal(L, "DEFAULT_ENEMY_H");
	Game::DEFAULT_ENEMY_H = lua_tonumber(L, -1);
	lua_getglobal(L, "DEFAULT_SPEED");
	Game::DEFAULT_SPEED = lua_tonumber(L, -1);
	// Game::TOTAL_GRAVITY_ARRAY_UNITS = lua_getglobal(L, "TOTAL_GRAVITY_ARRAY_UNITS");					<< THIS DOESNT EVEN WORK ANYWAYS
	// Game::TOTAL_JUMP_ARRAY_UNITS = lua_getglobal(L, "TOTAL_JUMP_ARRAY_UNITS");
	// Game::TOTAL_DASH_ARRAY_UNITS = lua_getglobal(L, "TOTAL_DASH_ARRAY_UNITS");
	lua_getglobal(L, "TOTAL_ENEMY_TYPES");
	TOTAL_ENEMY_TYPES = lua_tonumber(L, -1);
	lua_getglobal(L, "TOTAL_TILE_TYPES");
	TOTAL_TILE_TYPES = lua_tonumber(L, -1);
	lua_getglobal(L, "TOTAL_COLLECTIBLE_TYPES");
	TOTAL_COLLECTIBLE_TYPES = lua_tonumber(L, -1);
	lua_getglobal(L, "TOTAL_TILE_SUBTYPES");
	TOTAL_TILE_SUBTYPES = lua_tonumber(L, -1);
	// Player* pl = &Game::gPlayer;
	// lua_pushlightuserdata(L, pl);
	// lua_settable(L, LUA_REGISTRYINDEX);
	lua_pop(L, 14);

	lua_getglobal(L, "frameIdentifiers");												// frameIdentifiers[]
	for (int i = 0; i < TOTAL_FRAME_IDS; i++)
	{
		lua_pushnumber(L, i + 1);														// frameIdentifiers[], i
		lua_gettable(L, -2);															// frameIdentifiers[], frameSet[]
		for (int j = 0;; j++)
		{
			lua_pushnumber(L, j + 1);													// frameIdentifiers[], frameSet[], j
			lua_gettable(L, -2);														// frameIdentifiers[], frameSet[], identifier[]
			if (lua_isnil(L, -1))
			{
				lua_pop(L, 2);														// frameIdentifiers[]
				break;
			}
			lua_pushnumber(L, 1);														// frameIdentifiers[], frameSet[], identifier[], 1
			lua_gettable(L, -2);														// frameIdentifiers[], frameSet[], identifier[], identifier
			lua_pushnumber(L, 2);														// frameIdentifiers[], frameSet[], identifier[], identifier, 2
			lua_gettable(L, -3);														// frameIdentifiers[], frameSet[], identifier[], identifier, # of frames

			/*
			if (lua_isnil(L, -1) == 1 || lua_isnil(L, -2) == 1)
			{
				lua_pop(L, 4);						// frameIdentifiers[]
				break;
			}
			*/
			if (lua_isstring(L, -2) == 1 && lua_isinteger(L, -1) == 1)
			{
				identifier = lua_tostring(L, -2);
				framenum = lua_tointeger(L, -1);
				// frames.resize(lua_tointeger(L, -1));
				for (int k = 0; k < framenum; k++)
				{
					// frames[k] = lua_tostring(L, -2) + (char)(k + 1);
					frame << identifier << (k + 1);
					if (i == ENTITIES)
					{
						Graphics::entityFrameTypeIdentifiers.resize(j + 1);
						Graphics::entityFrameTypeIdentifiers[j].resize(k + 1);
						Graphics::entityFrameTypeIdentifiers[j][k] = frame.str();
						std::cout << frame.str() << std::endl;
					}
					else if (i == OBJECTS)
					{
						Graphics::objectFrameTypeIdentifiers.resize(j + 1);
						Graphics::objectFrameTypeIdentifiers[j].resize(k + 1);
						Graphics::objectFrameTypeIdentifiers[j][k] = frame.str();
					}
					// frames[k] = frame.str();
					frame.str(std::string());
				}
				lua_pop(L, 3);															// frameIdentifiers[], frameSet[]
			}
		}
	}
	lua_pop(L, 1);																		// 

	lua_getglobal(L, "tileSubIdentifiers");												// tileSubIdentifiers[]
	for (int i = 0; i < TOTAL_TILE_SUBTYPES; i++)
	{
		lua_pushnumber(L, i + 1);														// tileSubIdentifiers[], i + 1
		lua_gettable(L, -2);															// tileSubIdentifiers[], tileSubIdentifier
		Graphics::tileSubIdentifiers.resize(i + 1);
		Graphics::tileSubIdentifiers[i] = lua_tostring(L, -1);							// tileSubIdentifiers[], tileSubIdentifier
		lua_pop(L, 1);																	// tileSubIdentifiers[]
	}
	lua_pop(L, 1);																		// 
	
	lua_getglobal(L, "graphicsIdentifiers");											// graphicsIdentifiers[]
	for (int i = 0; i < TOTAL_GRAPHIC_IDS; i++)
	{
		lua_pushnumber(L, i + 1);														// graphicsIdentifiers[], i
		lua_gettable(L, -2);															// graphicsIdentifiers[], graphicSet[]
		for (int j = 0;; j++)
		{
			lua_pushnumber(L, j + 1);													// graphicsIdentifiers[], graphicSet[], j
			lua_gettable(L, -2);														// graphicsIdentifiers[], graphicSet[], identifier[]
			if (lua_isnil(L, -1))
			{
				lua_pop(L, 2);														// graphicsIdentifiers[]
				break;
			}
			lua_pushnumber(L, 1);														// graphicsIdentifiers[], graphicSet[], identifier[], 1
			lua_gettable(L, -2);														// graphicsIdentifiers[], graphicSet[], identifier[], identifier
			lua_pushnumber(L, 2);														// graphicsIdentifiers[], graphicSet[], identifier[], identifier, 2
			lua_gettable(L, -3);														// graphicsIdentifiers[], graphicSet[], identifier[], identifier, # of frames (or frame sets)

			/*
			if (lua_isnil(L, -1) == 1 || lua_isnil(L, -2) == 1)
			{
				lua_pop(L, 4);										// graphicsIdentifiers[]
				break;
			}
			*/
			if (lua_isstring(L, -2) == 1 && lua_isinteger(L, -1) == 1)
			{
				framenum = lua_tointeger(L, -1);
				identifier = lua_tostring(L, -2);
				// frames.resize(lua_tointeger(L, -1));
				// for (int k = 0; k < lua_tointeger(L, -1); k++)
				// 	frames[k] = lua_tostring(L, -2) + (char)(k + 1);
				if (i == BACKGROUNDS)
				{
					Graphics::backgroundTextures.resize(j + 1);
					Graphics::backgroundTextures[j].resize(framenum);
					Graphics::backgroundIdentifiers.resize(j + 1);
					Graphics::backgroundIdentifiers[j].resize(framenum);
					// frames.resize(framenum);
					if (framenum > 1)
					{
						for (int k = 0; k < framenum; k++)
						{
							// frames[k] = identifier + (char)(k + 1);
							frame << identifier << (k + 1);
							Graphics::backgroundIdentifiers[j][k] = frame.str();
							// frames[k] = frame.str();
							frame.str(std::string());
						}
					}
					else
					{
						// frames[0] = identifier;
						Graphics::backgroundIdentifiers[j][0] = frame.str();
					}
					// Graphics::backgroundIdentifiers[j] = frames[j];
				}
				else if (i == TILES)
				{
					int m = 0;
					Graphics::tileTextures.resize(j + 1);
					Graphics::tileTextures[j].resize(framenum * Graphics::tileSubIdentifiers.size());
					Graphics::tileIdentifiers.resize(j + 1);
					Graphics::tileIdentifiers[j].resize(framenum * Graphics::tileSubIdentifiers.size());
					for (int k = 0; k < Graphics::tileSubIdentifiers.size(); k++)
					{
						// for (int l = 0; l < framenum; l++)
						// {
						if (framenum > 1)
						{
							for (int l = 0; l < framenum; l++, m++)
							{
								// frames.resize(k + m + 1);
								// frames[k + m] = identifier + Graphics::tileSubIdentifiers[k] + (char)(l + 1);
								frame << identifier << Graphics::tileSubIdentifiers[k] << (l + 1);
								Graphics::tileIdentifiers[j][k + m] = frame.str();
								// frames[k] = frame.str();
								frame.str(std::string());
							}
						}
						else
						{
							// frames.resize(k + 1);
							// frames[k] = identifier + Graphics::tileSubIdentifiers[k] /* + (char)(l + 1) */;
							frame << identifier << Graphics::tileSubIdentifiers[k];
							Graphics::tileIdentifiers[j][k] = frame.str();
							// frames[k] = frame.str();
							frame.str(std::string());
						}
						// }
					}
				}
				else if (i == PLAYERS)
				{
					int m = 0;
					Graphics::playerTextures.resize(j + 1);
					// Graphics::playerTextures[j].resize(Graphics::entityFrameTypeIdentifiers[j].size());
					Graphics::playerIdentifiers.resize(j + 1);
					// Graphics::playerIdentifiers[j].resize(Graphics::entityFrameTypeIdentifiers[j].size());
					for (int k = 0; k < framenum; k++)
					{
						if (framenum > 1)
						{
							for (int l = 0; l < Graphics::entityFrameTypeIdentifiers[k].size() /* m < framenum */; l++, m++)
							{
								Graphics::playerTextures[j].resize(k + m + 1);
								Graphics::playerIdentifiers[j].resize(k + m + 1);
								// frames.resize(k + l + 1);
								// frames[k + l] = identifier + Graphics::entityFrameTypeIdentifiers[k][l] /* + (char)(m + 1) */;
								frame << identifier << Graphics::entityFrameTypeIdentifiers[k][l];
								Graphics::playerIdentifiers[j][k + m] = frame.str();
								// frames[k] = frame.str();
								frame.str(std::string());
							}
						}
						else
						{
							Graphics::playerTextures[j].resize(k + 1);
							Graphics::playerIdentifiers[j].resize(k + 1);
							// frames.resize(k + 1);
							// frames[k] = identifier;
							Graphics::playerIdentifiers[j][k] = identifier;
						}
					}
				}
				else if (i == ENEMIES)
				{
					int m = 0;
					Graphics::enemyTextures.resize(j + 1);
					// Graphics::enemyTextures[j].resize(Graphics::entityFrameTypeIdentifiers[j].size());
					Graphics::enemyIdentifiers.resize(j + 1);
					// Graphics::enemyIdentifiers[j].resize(Graphics::entityFrameTypeIdentifiers[j].size());
					for (int k = 0; k < framenum; k++)
					{
						if (framenum > 1)
						{
							for (int l = 0; l < Graphics::entityFrameTypeIdentifiers[k].size() /* m < framenum */; l++, m++)
							{
								Graphics::enemyTextures[j].resize(k + m + 1);
								Graphics::enemyIdentifiers[j].resize(k + m + 1);
								// frames.resize(k + l + 1);
								// frames[k + l] = identifier + Graphics::entityFrameTypeIdentifiers[k][l] /* + (char)(m + 1) */;
								frame << identifier << Graphics::entityFrameTypeIdentifiers[k][l];
								Graphics::enemyIdentifiers[j][k + m] = frame.str();
								// frames[k] = frame.str();
								frame.str(std::string());
							}
						}
						else
						{
							Graphics::enemyTextures[j].resize(k + 1);
							Graphics::enemyIdentifiers[j].resize(k + 1);
							// frames.resize(k + 1);
							// frames[k] = identifier;
							Graphics::enemyIdentifiers[j][k] = identifier;
						}
					}
				}
				else if (i == COLLECTIBLES)
				{
					Graphics::collectibleTextures.resize(j + 1);
					Graphics::collectibleTextures[j].resize(framenum);
					Graphics::collectibleIdentifiers.resize(j + 1);
					Graphics::collectibleIdentifiers[j].resize(framenum);
					// frames.resize(framenum);
					if (framenum > 1)
					{
						for (int k = 0; k < framenum; k++)
						{
							// frames[k] = identifier + (char)(k + 1);
							frame << identifier << (k + 1);
							Graphics::collectibleIdentifiers[j][k] = frame.str();
							// frames[k] = frame.str();
							frame.str(std::string());
						}
					}
					else
						Graphics::collectibleIdentifiers[j][0] = identifier;
				}
				else if (i == PARTICLES)
				{
					Graphics::particleTextures.resize(j + 1);
					Graphics::particleTextures[j].resize(framenum);
					Graphics::particleIdentifiers.resize(j + 1);
					Graphics::particleIdentifiers[j].resize(framenum);
					// frames.resize(framenum);
					if (framenum > 1)
					{
						for (int k = 0; k < framenum; k++)
						{
							// frames[k] = identifier + (char)(k + 1);
							frame << identifier << (k + 1);
							Graphics::particleIdentifiers[j][k] = frame.str();
							// frames[k] = frame.str();
							frame.str(std::string());
						}
					}
					else
						Graphics::particleIdentifiers[j][0] = identifier;
				}
				lua_pop(L, 3);															// graphicsIdentifiers[], graphicSet[]
			}
			/*
			else if (lua_isinteger(L, -1) == 1)
			{
				for (int j = 0; j < lua_tointeger(L, -1); j++)
					Graphics::backgroundIdentifiers[i][j] = frames[j];	
				lua_pop(L, 2);											// graphicsIdentifiers[]
			}
			*/
		}
	}
	lua_pop(L, 1);																		// 

	lua_getglobal(L, "audioIdentifiers");												// audioIdentifiers[]
	for (int i = 0; i < TOTAL_AUDIO_IDS; i++)
	{
		lua_pushnumber(L, i + 1);														// audioIdentifiers[], i
		lua_gettable(L, -2);															// audioIdentifiers[], audioSet[]
		for (int j = 0;; j++)
		{
			lua_pushnumber(L, j + 1);													// audioIdentifiers[], audioSet[], j
			lua_gettable(L, -2);														// audioIdentifiers[], audioSet[], identifier
			if (lua_isnil(L, -1) == 1)
			{
				lua_pop(L, 2);															// audioIdentifiers[]
				break;
			}
			else if (lua_isstring(L, -1) == 1)
			{
				if (i == SFX)
				{
					Audio::sfx.resize(j + 1);
					Audio::sfxIdentifiers.resize(j + 1);
					Audio::sfxIdentifiers[j] = lua_tostring(L, -1);
					// Audio::sfxIdentifiers.insert(Audio::sfxIdentifiers.end(), lua_tostring(L, -1));
				}
				else if (i == MUSIC)
				{
					Audio::music.resize(j + 1);
					Audio::musicIdentifiers.resize(j + 1);
					Audio::musicIdentifiers[j] = lua_tostring(L, -1);
					// Audio::musicIdentifiers.insert(Audio::musicIdentifiers.end(), lua_tostring(L, -1));
				}
				lua_pop(L, 1);															// audioIdentifiers[], audioSet[]
			}
		}
	}
	lua_pop(L, 1);																		// 

	/*
	lua_getglobal(L, "graphicsIdentifiers");										// graphicsIdentifiers[]
	lua_pushstring(L, "bgs");														// graphicsIdentifiers[], "bgs"
	lua_gettable(L, -2);															// graphicsIdentifiers[], bgs[]
	for (int i = 0;; i++)
	{
		lua_pushnumber(L, i + 1);													// graphicsIdentifiers[], bgs[], i
		lua_gettable(L, -2);														// graphicsIdentifiers[], bgs[], identifier
		Graphics::backgroundIdentifiers.resize(i + 1);
		Graphics::backgroundIdentifiers[i] = lua_tostring(L, -1);
		lua_pop(L, 1);																// graphicsIdentifiers[], bgs[]
	}
	lua_pop(L, 1);																	// graphicsIdentifiers[]
	*/

	/*
	std::cout << "MADE IT HERE ERERERERE" << std::endl;
	Graphics::enemyIdentifiers = { "Dude" };
	Graphics::tileIdentifiers = { "DirtBlock", "DirtWall" };
	Graphics::tileSubIdentifiers = { "Center", "Top", "TopRight", "Right", "BottomRight", "Bottom", "BottomLeft", "Left", "TopLeft" };
	Graphics::collectibleIdentifiers = { "Bit", "Byte" };
	Graphics::particleIdentifiers = { "Red", "Gray", "Blue", "BigRed", "BigGray", "BigBlue" };
	Graphics::entityFrameTypeIdentifiers = { "Idle", "WalkLeft", "WalkRight", "Jump", "Fall" };
	Graphics::objectFrameTypeIdentifiers = { "Frame1", "Frame2", "Frame3", "Frame4", "Frame5" };
	*/

	lua_pushnumber(L, Game::gScore);			// gScore
	lua_setglobal(L, "points");					// 

	lua_getglobal(L, "TOTAL_JUMP_ARRAY_UNITS");	// TOTAL_JUMP_ARRAY_UNITS
	max = lua_tonumber(L, -1);					// TOTAL_JUMP_ARRAY_UNITS
	Game::jumpArray.resize(max);
	lua_pop(L, 1);								// 
	lua_getglobal(L, "jumpArray");				// jumpArray
	for (int i = 0; i < max; i++)
	{
		lua_pushnumber(L, i + 1);				// jumpArray, i
		lua_gettable(L, -2);					// jumpArray, value at i
		Game::jumpArray[i] = (int)lua_tonumber(L, -1);
		lua_pop(L, 1);							// jumpArray
	}
	lua_pop(L, 1);									//
	lua_getglobal(L, "TOTAL_GRAVITY_ARRAY_UNITS");	// TOTAL_GRAVITY_ARRAY_UNITS
	max = lua_tonumber(L, -1);						// TOTAL_GRAVITY_ARRAY_UNITS
	Game::gravityArray.resize(max);
	lua_pop(L, 1);									// 
	lua_getglobal(L, "gravityArray");				// gravityArray
	for (int i = 0, j = 0; i < max; i++)
	{
		lua_pushnumber(L, i + 1);				// gravityArray, i
		lua_gettable(L, -2);					// gravityArray, value at i
		Game::gravityArray[i] = (int)lua_tonumber(L, -1);
		lua_pop(L, 1);							// gravityArray
	}
	lua_pop(L, 1);								//
	lua_getglobal(L, "TOTAL_DASH_ARRAY_UNITS");	// TOTAL_DASH_ARRAY_UNITS
	max = lua_tonumber(L, -1);					// TOTAL_DASH_ARRAY_UNITS
	Game::dashArray.resize(max);
	lua_pop(L, 1);								// 
	lua_getglobal(L, "dashArray");				// dashArray
	for (int i = 0, j = 0; i < max; i++)
	{
		lua_pushnumber(L, i + 1);				// dashArray, i
		lua_gettable(L, -2);					// dashArray, value at i
		Game::dashArray[i] = (int)lua_tonumber(L, -1);
		lua_pop(L, 1);							// dashArray
	}
	lua_pop(L, 1);
	lua_getglobal(L, "TOTAL_FLOAT_ARRAY_UNITS");	// TOTAL_FLOAT_ARRAY_UNITS
	max = lua_tonumber(L, -1);						// TOTAL_FLOAT_ARRAY_UNITS
	Game::floatArray.resize(max);
	lua_pop(L, 1);									// 
	lua_getglobal(L, "floatArray");					// floatArray
	for (int i = 0, j = 0; i < max; i++)
	{
		lua_pushnumber(L, i + 1);				// floatArray, i
		lua_gettable(L, -2);					// floatArray, value at i
		Game::floatArray[i] = (int)lua_tonumber(L, -1);
		lua_pop(L, 1);							// floatArray
	}
	lua_pop(L, 1);								// 

	/*
	lua_createtable(L, Level::LEVEL_UNITS, 0);											// things
	for (int i = 0; i < Level::LEVEL_UNITS; i++)
	{
		lua_pushnumber(L, (Game::things[i] == NULL ? -1 : Game::things[i]->tgType));	// things, (-1 or i)
		lua_seti(L, -2, i);																// things
		if (Game::things[i] != NULL)
		{
			if (Game::things[i]->tgType == ENEMY)
			{
				lua_pushstring(L, "enSubtype");						// things, specific thing, "enSubtype"
				lua_pushnumber(L, Game::things[i]->tgGetSubtype());	// things, specific thing, "enSubtype", subtype
				lua_settable(L, -3);								// things, specific thing
				lua_pop(L, 1);										// things
			}
		}
	}
	lua_getglobal(L, "things");										// things, lua things
	lua_copy(L, -2, -1);											// things, things
	std::cout << (int)lua_rawlen(L, -1) << " ";
	std::cout << (int)lua_rawlen(L, -2);
	lua_pop(L, 2);													// 
	*/

	/*
	for (int i = 0; i < Level::LEVEL_UNITS; i++)
	{
		// if (Game::things[i] != NULL)
		// {
		lua_getglobal(L, "copythings");
		lua_pushnumber(L, (Game::things[i] == NULL ? -1 : Game::things[i]->tgType));
		lua_call(L, 1, 0);

		// lua_getglobal(L, "things");														// things
		// lua_pushnumber(L, (Game::things[i] == NULL ? -1 : Game::things[i]->tgType));	// things, (-1 or i)
		// lua_seti(L, -2, i);																// things
		// lua_pushnumber(L, i);														// things, i
		// lua_pushnumber(L, (Game::things[i] == NULL ? -1 : Game::things[i]->tgType));	// things, i, (-1 or i)
		// lua_settable(L, -3);															// things
		if (Game::things[i] != NULL)
		{
			if (Game::things[i]->tgType == ENEMY)
			{
				lua_getglobal(L, "things");
				lua_pushstring(L, "enSubtype");						// things table, specific thing, "enSubtype"
				lua_pushnumber(L, Game::things[i]->tgGetSubtype());	// things table, specific thing, "enSubtype", subtype
				lua_settable(L, -3);								// things table, specific thing
				lua_pop(L, 2);										//
			}
		}
		// }
	}
	*/

	//lua_getglobal(L, "handleLevelLoad");												// handleLevelLoad()
	//lua_pushnumber(L, Level::LEVEL_UNITS);												// handleLevelLoad(), LEVEL_UNITS
	//lua_call(L, 1, 0);																	//

	lua_getglobal(L, "things");															// things
	for (int i = 0; i < Level::LEVEL_UNITS; i++)
	{
		// if (Game::things[i] != NULL)
		// {
		lua_pushnumber(L, (Game::things[i] == NULL ? -1 : Game::things[i]->tgType));	// things, (-1 or things[i]->tgType)
		lua_seti(L, -2, i + 1);															// things
		// lua_pushnumber(L, i);
		// lua_pushnumber(L, (Game::things[i] == NULL ? -1 : Game::things[i]->tgType));
		// lua_settable(L, -3);
		// }
	}
	lua_getglobal(L, "init");
	lua_call(L, 0, 0);
	for (int i = 0; i < Level::LEVEL_UNITS; i++)
	{
		if (Game::things[i] != NULL)
		{
			if (Game::things[i]->tgType == ENEMY)
			{
				lua_pushstring(L, "enSubtype");						// things, specific thing, "enSubtype"
				lua_pushnumber(L, Game::things[i]->tgGetSubtype());	// things, specific thing, "enSubtype", subtype
				lua_settable(L, -3);								// things, specific thing
				lua_pop(L, 1);										// things
			}
			else if (Game::things[i]->tgType == TILE)
			{
				lua_pushstring(L, "tiSubtype");						// things, specific thing, "tiSubtype"
				lua_pushnumber(L, Game::things[i]->tgGetSubtype());	// things, specific thing, "tiSubtype", subtype
				lua_settable(L, -3);								// things, specific thing
				lua_pop(L, 1);										// things
			}
			else if (Game::things[i]->tgType == COLLECTIBLE)
			{
				lua_pushstring(L, "clSubtype");						// things, specific thing, "clSubtype"
				lua_pushnumber(L, Game::things[i]->tgGetSubtype());	// things, specific thing, "clSubtype", subtype
				lua_settable(L, -3);								// things, specific thing
				lua_pop(L, 1);										// things
			}
		}
	}

	// lua_getglobal(L, "init");
	// lua_call(L, 0, 0);
}

/*
int LuaBridge::labCall(int funcID)
{


	/*
	switch (funcID)
	{
	case HANDLEVERTICALS:
		// lua_getfield(L, )
		if (labStack[1] == NULL)
		{
			lua_getglobal(L, "plHandleVerticals");
			lua_pushnumber(L, (int)&labStack[0]);
			lua_call(L, 1, 1);
		}
		else
		{
			lua_getglobal(L, "tgHandleVerticals");
			lua_pushnumber(L, (int)&labStack[0]);
			lua_pushlightuserdata(L, labStack[1]);
			lua_call(L, 1, 1);
		}
		break;
	}
	

	return 0;
}
*/

int LuaBridge::labHandleEnvironment(void)
{
	lua_pushnumber(L, Game::gScore);			// gScore
	lua_setglobal(L, "points");					// 

	for (int i = 0; i < Level::LEVEL_UNITS; i++)
	{
		if (Game::things[i] != NULL)
		{
			lua_getglobal(L, "things");							// things table
			lua_pushnumber(L, i + 1);							// things table, i
			lua_gettable(L, -2);								// things table, specific thing
			std::cout << i << ": " << lua_typename(L, lua_type(L, -1)) << std::endl;
			lua_pushstring(L, "tgVerticals");					// things table, specific thing, "tgVerticals"
			lua_pushnumber(L, Game::things[i]->tgVerticals);	// things table, specific thing, "tgVerticals", tgVerticals
			lua_settable(L, -3);								// things table, specific thing
			lua_pushstring(L, "tgSpeed");						// things table, specific thing, "tgSpeed"
			lua_pushnumber(L, Game::things[i]->tgSpeed);		// things table, specific thing, "tgSpeed", tgSpeed
			lua_settable(L, -3);								// things table, specific thing
			// lua_pop(L, 2);

			lua_pushstring(L, "tgHitbox");						// things table, specific thing, "tgHitbox"
			lua_gettable(L, -2);								// things table, specific thing, tgHitbox
			lua_pushstring(L, "x");								// things table, specific thing, tgHitbox, "x"
			lua_pushnumber(L, Game::things[i]->tgHitboxRect.x);	// things table, specific thing, tgHitbox, "x", x
			lua_settable(L, -3);								// things table, specific thing, tgHitbox
			lua_pushstring(L, "y");								// things table, specific thing, tgHitbox, "y"
			lua_pushnumber(L, Game::things[i]->tgHitboxRect.y);	// things table, specific thing, tgHitbox, "y", y
			lua_settable(L, -3);								// things table, specific thing, tgHitbox
			lua_pop(L, 1);										//

			if (Game::things[i]->tgType == ENEMY)
			{
				lua_getfield(L, -1, "enHandleAI");			// things table, specific thing, enHandleAI()
				lua_call(L, 0, 0);							// things table, specific thing
				// lua_pop(L, 1);							// things table, specific thing
			}
			else if (Game::things[i]->tgType == TILE)
			{
				lua_getfield(L, -1, "tiHandleAI");			// things table, specific thing, tiHandleAI()
				lua_call(L, 0, 0);							// things table, specific thing
				// lua_pop(L, 1);
			}
			else if (Game::things[i]->tgType == PLAYER)
			{
				// ?
			}
			else if (Game::things[i]->tgType == COLLECTIBLE)
			{
				lua_getfield(L, -1, "clHandleAI");			// things table, specific thing, clHandleAI()
				lua_call(L, 0, 0);							// things table, specific thing
				if (Game::things[i]->tgHealth == 0)
				{
					lua_getfield(L, -1, "clCollect");		// things table, specific thing, clCollect()
					lua_call(L, 0, 0);						// things table, specific thing
					lua_pop(L, 2);							// 
				}
			}
		}
		std::cout << i << std::endl;
	}
	// lua_getglobal(L, "handleEnvironment");
	// lua_call(L, 0, 0);
	lua_getglobal(L, "points");									// points
	Game::gScore = (int)lua_tonumber(L, -1);					// points
	lua_pop(L, 1);												// 
	for (int i = 0; i < Level::LEVEL_UNITS; i++)
	{
		if (Game::things[i] != NULL)
		{
			lua_getglobal(L, "things");							// things table
			lua_pushnumber(L, i + 1);							// things table, i
			lua_gettable(L, -2);								// things table, specific thing
			lua_pushstring(L, "tgVerticals");					// things table, specific thing, "tgVerticals"
			lua_gettable(L, -2);								// things table, specific thing, tgVerticals
			Game::things[i]->tgVerticals = (int)lua_tonumber(L, -1);
			lua_pop(L, 1);										// things table, specific thing
			lua_pushstring(L, "tgSpeed");						// things table, specific thing, "tgSpeed"
			lua_gettable(L, -2);								// things table, specific thing, tgSpeed
			Game::things[i]->tgSpeed = (int)lua_tonumber(L, -1);
			if (Game::things[i]->tgSpeed > 0)
				Game::things[i]->tgDirection = RIGHT;
			else if (Game::things[i]->tgSpeed < 0)
				Game::things[i]->tgDirection = LEFT;
			lua_pop(L, 1);										// things table, specific thing

			// lua_getglobal(L, "things");		// things table
			// lua_pushnumber(L, i);			// things table, i
			// lua_gettable(L, -2);				// things table, specific thing
			lua_pushstring(L, "tgHitbox");						// things table, specific thing, "tgHitbox"
			lua_gettable(L, -2);								// things table, specific thing, tgHitbox
			lua_pushstring(L, "x");								// things table, specific thing, tgHitbox, "x"
			lua_gettable(L, -2);								// things table, specific thing, tgHitbox, x
			lua_insert(L, 3);									// things table, specific thing, x, tgHitbox
			lua_pushstring(L, "y");								// things table, specific thing, x, tgHitbox, "y"
			lua_gettable(L, -2);								// things table, specific thing, x, tgHitbox, y
			Game::things[i]->tgHitboxRect.x = (int)lua_tonumber(L, -3);
			Game::things[i]->tgHitboxRect.y = (int)lua_tonumber(L, -1);
			lua_pop(L, 5);										//
		}
	}
	std::cout << "PPPOEERR" << std::endl;

	// this should already be handled in things, down below
	/*
	lua_getglobal(L, "gPlayer");				// gPlayer table
	lua_pushstring(L, "tgHitbox");				// gPlayer table, "tgHitbox" 
	lua_gettable(L, -2);						// gPlayer table, tgHitbox
	lua_pushstring(L, "x");						// gPlayer table, tgHitbox, "x"
	lua_gettable(L, -2);						// gPlayer table, tgHitbox, value of x
	lua_insert(L, 2);							// gPlayer table, value of x, tgHitbox
	lua_pushstring(L, "y");						// gPlayer table, value of x, tgHitbox, "y"
	lua_gettable(L, -2);						// gPlayer table, value of x, tgHitbox, value of y
	Game::gPlayer.tgHitboxRect.x = lua_tonumber(L, -3);
	Game::gPlayer.tgHitboxRect.y = lua_tonumber(L, -1);
	lua_pop(L, 4);								//
	*/

	/*
	for (int i = 0; i < Level::LEVEL_UNITS; i++)
	{
		if (Game::things[i] != NULL)
		{
			lua_getglobal(L, "things");			// things table
			lua_pushnumber(L, i);				// things table, i
			lua_gettable(L, -2);				// things table, specific thing
			lua_pushstring(L, "tgHitbox");		// things table, specific thing, "tgHitbox"
			lua_gettable(L, -2);				// things table, specific thing, tgHitbox
			lua_pushstring(L, "x");				// things table, specific thing, tgHitbox, "x"
			lua_gettable(L, -2);				// things table, specific thing, tgHitbox, x
			lua_insert(L, 3);					// things table, specific thing, x, tgHitbox
			lua_pushstring(L, "y");				// things table, specific thing, x, tgHitbox, "y"
			lua_gettable(L, -2);				// things table, specific thing, x, tgHitbox, y
			Game::things[i]->tgHitboxRect.x = lua_tonumber(L, -3);
			Game::things[i]->tgHitboxRect.y = lua_tonumber(L, -1);
			lua_pop(L, 5);						//
		}
	}
	Game::gPlayer.tgHitboxRect.x = Game::things[Game::gPlayer.tgLevelUnit]->tgHitboxRect.x;
	Game::gPlayer.tgHitboxRect.y = Game::things[Game::gPlayer.tgLevelUnit]->tgHitboxRect.y;
	*/
	
	
	
	
	//lua_getglobal(L, "Player");



	/*
	for (int i = 0; i < Level::LEVEL_UNITS; i++)
	{
		if (Game::things[i]->tgType == PLAYER || Game::things[i]->tgType == ENEMY)
			lua_pushnumber(L, Game::things[i]->tgVerticals);
		else
			lua_pushnumber(L, 0);
	}

	if (lua_iscfunction(L, -1))
	{
		lua_tocfunction(L, -1);
		for (int i = 0; i < Level::LEVEL_UNITS; i++)
		{
			lua_call(L, 1, 0);
			lua_pop(L, -1);
		}
	}
	*/

	return 0;
}

int LuaBridge::labCheckCollision(lua_State* L)
{
	SDL_Rect t1 = Game::things[(int)lua_tonumber(L, -2)]->tgHitboxRect;
	SDL_Rect t2 = Game::things[(int)lua_tonumber(L, -1)]->tgHitboxRect;

	if ((t1.x + t1.w > t2.x || t1.x < t2.x + t2.w) && (t1.y + t1.h > t2.y && t1.y < t2.y + t2.h))
		lua_pushboolean(L, true);
	else
		lua_pushboolean(L, false);

	return 1;
}

/*
int LuaBridge::labCheckCollision(lua_State* L)
{
	bool testall = lua_toboolean(L, 3);
	bool iscolliding = false;
	lua_pop(L, 1);
	lua_pushstring(L, "x");						// plHitbox, tgHitbox, "x"
	lua_gettable(L, 1);							// plHitbox, tgHitbox, plHitbox.x
	lua_pushstring(L, "y");						// plHitbox, tgHitbox, plHitbox.x, "y"
	lua_gettable(L, 1);							// plHitbox, tgHitbox, plHitbox.x, plHitbox.y
	lua_pushstring(L, "w");						// plHitbox, tgHitbox, plHitbox.x, plHitbox.y, "w"
	lua_gettable(L, 1);							// plHitbox, tgHitbox, plHitbox.x, plHitbox.y, plHitbox.w
	lua_pushstring(L, "h");						// plHitbox, tgHitbox, plHitbox.x, plHitbox.y, plHitbox.w, "h"
	lua_gettable(L, 1);							// plHitbox, tgHitbox, plHitbox.x, plHitbox.y, plHitbox.w, plHitbox.h
	SDL_Rect tg1r{ lua_tonumber(L, -4), lua_tonumber(L, -3), lua_tonumber(L, -2), lua_tonumber(L, -1) };
	lua_pop(L, 4);
	if (!testall)
	{
		lua_pushstring(L, "x");						// plHitbox, tgHitbox, "x"
		lua_gettable(L, 2);							// plHitbox, tgHitbox, tgHitbox.x
		lua_pushstring(L, "y");						// plHitbox, tgHitbox, tgHitbox.x, "y"
		lua_gettable(L, 2);							// plHitbox, tgHitbox, tgHitbox.x, tgHitbox.y
		lua_pushstring(L, "w");						// plHitbox, tgHitbox, tgHitbox.x, tgHitbox.y, "w"
		lua_gettable(L, 2);							// plHitbox, tgHitbox, tgHitbox.x, tgHitbox.y, tgHitbox.w
		lua_pushstring(L, "h");						// plHitbox, tgHitbox, tgHitbox.x, tgHitbox.y, tgHitbox.w, "h"
		lua_gettable(L, 2);							// plHitbox, tgHitbox, tgHitbox.x, tgHitbox.y, tgHitbox.w, tgHitbox.h
		SDL_Rect tg2r{ lua_tonumber(L, -4), lua_tonumber(L, -3), lua_tonumber(L, -2), lua_tonumber(L, -1) };
		iscolliding = Game::checkCollisionRects(tg1r, tg2r);
	}
	else
	{
		Thing tg1{ tg1r, , };
		Game::checkCollision()
	}
	/*
	int x1 = lua_tonumber(L, -8);
	int y1 = lua_tonumber(L, -7);
	int w1 = lua_tonumber(L, -6);
	int h1 = lua_tonumber(L, -5);
	int x2 = lua_tonumber(L, -4);
	int y2 = lua_tonumber(L, -3);
	int w2 = lua_tonumber(L, -2);
	int h2 = lua_tonumber(L, -1);
	if ((x1 + w1 > x2 && x1 < x2 + w2) && (y1 + h1 > y2 && y1 < y2 + h2))
		lua_pushboolean(L, true);
	else
		lua_pushboolean(L, false);
		
	return 1;
}
*/