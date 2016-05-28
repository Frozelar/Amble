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


#include "LuaBridge.h"
#include "Game.h"
#include "Audio.h"
#include "Graphics.h"
#include "Level.h"
#include "Thing.h"
#include "Particle.h"
#include "Player.h"
#include <sstream>

// const int LuaBridge::MAX_ARGS = 64;
// void* LuaBridge::labStack[MAX_ARGS];

// bool LuaBridge::labChangedLevel = false;
lua_State* LuaBridge::L;
LuaBridge* Game::gLuaBridge;

std::map<std::string, int> LuaBridge::GFXIDLocations;
std::map<std::string, int> LuaBridge::FrameIDLocations;
std::map<std::string, int> LuaBridge::AudioIDLocations;

LuaBridge::LuaBridge()
{
	int num = 0;
	L = luaL_newstate();
	GFXIDLocations["backgrounds"] = num;
	GFXIDLocations["tiles"] = (++num);
	GFXIDLocations["players"] = (++num);
	GFXIDLocations["enemies"] = (++num);
	GFXIDLocations["collectibles"] = (++num);
	GFXIDLocations["particles"] = (++num);
	GFXIDLocations["total"] = (++num);
	FrameIDLocations["entities"] = (num = 0);
	FrameIDLocations["objects"] = (++num);
	FrameIDLocations["total"] = (++num);
	AudioIDLocations["sfx"] = (num = 0);
	AudioIDLocations["music"] = (++num);
	AudioIDLocations["total"] = (++num);

	// for (int i = 0; i < MAX_ARGS; i++)
	//	labStack[i] = NULL;
}

LuaBridge::~LuaBridge()
{
	
}

int LuaBridge::labInitValues(void)
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

	lua_getglobal(L, "game_title");
	Game::gameTitle = lua_tostring(L, -1);
	lua_pop(L, 1);

	lua_getglobal(L, "totalLevels");
	Level::totalLevels = lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_getglobal(L, "resourceDirectory");
	Game::rDir = lua_tostring(L, -1);
	lua_getglobal(L, "resourceExtension");
	Game::rExt = lua_tostring(L, -1);
	lua_pop(L, 2);
	lua_getglobal(L, "heading_font");		// font table
	lua_pushnumber(L, 1);					// font table, 1
	lua_gettable(L, -2);					// font table, name
	lua_pushnumber(L, 2);					// font table, name, 2
	lua_gettable(L, -3);					// font table, name, size
	for (int i = 2; i < 6; i++)
	{
		lua_pushnumber(L, i + 1);
		lua_gettable(L, -(i + 2));
	}										// font table, name, size, r, g, b, a
	Game::gHeadingFont.name = lua_tostring(L, -6);
	Game::gHeadingFont.size = lua_tonumber(L, -5);
	Game::gHeadingFont.color.r = lua_tonumber(L, -4);
	Game::gHeadingFont.color.g = lua_tonumber(L, -3);
	Game::gHeadingFont.color.b = lua_tonumber(L, -2);
	Game::gHeadingFont.color.a = lua_tonumber(L, -1);
	lua_pop(L, 7);
	lua_getglobal(L, "body_font");			// font table
	lua_pushnumber(L, 1);					// font table, 1
	lua_gettable(L, -2);					// font table, name
	lua_pushnumber(L, 2);					// font table, name, 2
	lua_gettable(L, -3);					// font table, name, size
	for (int i = 2; i < 6; i++)
	{
		lua_pushnumber(L, i + 1);
		lua_gettable(L, -(i + 2));
	}										// font table, name, size, r, g, b, a
	Game::gBodyFont.name = lua_tostring(L, -6);
	Game::gBodyFont.size = lua_tonumber(L, -5);
	Game::gBodyFont.color.r = lua_tonumber(L, -4);
	Game::gBodyFont.color.g = lua_tonumber(L, -3);
	Game::gBodyFont.color.b = lua_tonumber(L, -2);
	Game::gBodyFont.color.a = lua_tonumber(L, -1);
	lua_pop(L, 7);
	lua_getglobal(L, "title_font");			// font table
	lua_pushnumber(L, 1);					// font table, 1
	lua_gettable(L, -2);					// font table, name
	lua_pushnumber(L, 2);					// font table, name, 2
	lua_gettable(L, -3);					// font table, name, size
	for (int i = 2; i < 6; i++)
	{
		lua_pushnumber(L, i + 1);
		lua_gettable(L, -(i + 2));
	}										// font table, name, size, r, g, b, a
	Game::gTitleFont.name = lua_tostring(L, -6);
	Game::gTitleFont.size = lua_tonumber(L, -5);
	Game::gTitleFont.color.r = lua_tonumber(L, -4);
	Game::gTitleFont.color.g = lua_tonumber(L, -3);
	Game::gTitleFont.color.b = lua_tonumber(L, -2);
	Game::gTitleFont.color.a = lua_tonumber(L, -1);
	lua_pop(L, 7);

	lua_getglobal(L, "WINDOW_W");
	lua_getglobal(L, "WINDOW_H");
	if (lua_tonumber(L, -2) != Game::WINDOW_W || lua_tonumber(L, -1) != Game::WINDOW_H)
	{
		Game::WINDOW_W = lua_tonumber(L, -2);
		Game::WINDOW_H = lua_tonumber(L, -1);
		Graphics::gxUpdateWindow();
	}
	lua_pop(L, 2);

	lua_getglobal(L, "DEFAULT_W");
	Game::DEFAULT_W = lua_tonumber(L, -1);
	lua_getglobal(L, "DEFAULT_H");
	Game::DEFAULT_H = lua_tonumber(L, -1);
	lua_getglobal(L, "PLAYER_W");
	Game::PLAYER_W = lua_tonumber(L, -1);
	lua_getglobal(L, "PLAYER_H");
	Game::PLAYER_H = lua_tonumber(L, -1);
	lua_getglobal(L, "DEFAULT_GFX_OFFSET");
	Game::DEFAULT_GFX_OFFSET = lua_tonumber(L, -1);
	lua_getglobal(L, "DEFAULT_ENEMY_W");
	Game::DEFAULT_ENEMY_W = lua_tonumber(L, -1);
	lua_getglobal(L, "DEFAULT_ENEMY_H");
	Game::DEFAULT_ENEMY_H = lua_tonumber(L, -1);
	lua_getglobal(L, "DEFAULT_COLLECTIBLE_W");
	Game::DEFAULT_COLLECTIBLE_W = lua_tonumber(L, -1);
	lua_getglobal(L, "DEFAULT_COLLECTIBLE_H");
	Game::DEFAULT_COLLECTIBLE_H = lua_tonumber(L, -1);
	lua_getglobal(L, "DEFAULT_SPEED");
	Game::DEFAULT_SPEED = lua_tonumber(L, -1);
	// Game::TOTAL_GRAVITY_ARRAY_UNITS = lua_getglobal(L, "TOTAL_GRAVITY_ARRAY_UNITS");					<< THIS DOESNT EVEN WORK ANYWAYS
	// Game::TOTAL_JUMP_ARRAY_UNITS = lua_getglobal(L, "TOTAL_JUMP_ARRAY_UNITS");
	// Game::TOTAL_DASH_ARRAY_UNITS = lua_getglobal(L, "TOTAL_DASH_ARRAY_UNITS");
	lua_getglobal(L, "TOTAL_ENEMY_TYPES");
	Game::EnemyType["total"] = lua_tonumber(L, -1);
	// TOTAL_ENEMY_TYPES = lua_tonumber(L, -1);
	lua_getglobal(L, "TOTAL_TILE_TYPES");
	Game::TileType["total"] = lua_tonumber(L, -1);
	// TOTAL_TILE_TYPES = lua_tonumber(L, -1)
	lua_getglobal(L, "TOTAL_COLLECTIBLE_TYPES");
	Game::CollectibleType["total"] = lua_tonumber(L, -1);
	// TOTAL_COLLECTIBLE_TYPES = lua_tonumber(L, -1);
	lua_getglobal(L, "TOTAL_TILE_SUBTYPES");
	Game::TileSubType["total"] = lua_tonumber(L, -1);
	// TOTAL_TILE_SUBTYPES = lua_tonumber(L, -1);
	// Player* pl = &Game::gPlayer;
	// lua_pushlightuserdata(L, pl);
	// lua_settable(L, LUA_REGISTRYINDEX);
	lua_pop(L, 14);

	/*
	lua_getglobal(L, "textColor");						// textColor
	for (int i = 0; i < 4; i++)
	{
		lua_pushnumber(L, i + 1);
		lua_gettable(L, -(i + 2));
	}													// textColor, r, g, b, a
	Game::textColor.r = lua_tonumber(L, -4);
	Game::textColor.g = lua_tonumber(L, -3);
	Game::textColor.b = lua_tonumber(L, -2);
	Game::textColor.a = lua_tonumber(L, -1);
	lua_pop(L, 5);										// 
	*/

	lua_getglobal(L, "highlightColor");					// highlightColor
	for (int i = 0; i < 4; i++)
	{
		lua_pushnumber(L, i + 1);
		lua_gettable(L, -(i + 2));
	}													// highlightColor, r, g, b, a
	Game::highlightColor.r = lua_tonumber(L, -4);
	Game::highlightColor.g = lua_tonumber(L, -3);
	Game::highlightColor.b = lua_tonumber(L, -2);
	Game::highlightColor.a = lua_tonumber(L, -1);
	lua_pop(L, 5);										// 

	lua_getglobal(L, "frameIdentifiers");												// frameIdentifiers[]
	for (int i = 0; i < FrameIDLocations["total"]; i++)
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
				if (i == FrameIDLocations["entities"])
				{
					Graphics::entityFrameTypeIdentifiers.resize(j + 1);
					Game::EntityFrameGroup[identifier] = framenum;
				}
				else if (i == FrameIDLocations["objects"])
				{
					Graphics::objectFrameTypeIdentifiers.resize(j + 1);
					Game::ObjectFrameGroup[identifier] = framenum;
				}
				for (int k = 0; k < framenum; k++)
				{
					// frames[k] = lua_tostring(L, -2) + (char)(k + 1);
					frame << identifier << (k + 1);
					if (i == FrameIDLocations["entities"])
					{
						Graphics::entityFrameTypeIdentifiers[j].resize(k + 1);
						Graphics::entityFrameTypeIdentifiers[j][k] = frame.str();
						Game::EntityFrameType[frame.str()] = j;
					}
					else if (i == FrameIDLocations["objects"])
					{
						Graphics::objectFrameTypeIdentifiers[j].resize(k + 1);
						Graphics::objectFrameTypeIdentifiers[j][k] = frame.str();
						Game::ObjectFrameType[frame.str()] = j;
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
	for (int i = 0; i < Game::TileSubType["total"]; i++)
	{
		lua_pushnumber(L, i + 1);														// tileSubIdentifiers[], i + 1
		lua_gettable(L, -2);															// tileSubIdentifiers[], tileSubIdentifier
		identifier = lua_tostring(L, -1);
		Graphics::tileSubIdentifiers.resize(i + 1);
		Graphics::tileSubIdentifiers[i] = identifier;									// tileSubIdentifiers[], tileSubIdentifier
		Game::TileSubType[identifier] = i;
		lua_pop(L, 1);																	// tileSubIdentifiers[]
	}
	lua_pop(L, 1);																		// 
	
	lua_getglobal(L, "graphicsIdentifiers");											// graphicsIdentifiers[]
	for (int i = 0; i < GFXIDLocations["total"]; i++)
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
				if (i == GFXIDLocations["backgrounds"])
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
						Graphics::backgroundIdentifiers[j][0] = identifier /* frame.str() */;
					}
					// Graphics::backgroundIdentifiers[j] = frames[j];
					Game::BackgroundType[identifier] = j;
				}
				else if (i == GFXIDLocations["tiles"])
				{
					int m = 0;
					Graphics::tileTextures.resize((j + 1) * Graphics::tileSubIdentifiers.size());
					// Graphics::tileTextures[j].resize(framenum);
					Graphics::tileIdentifiers.resize((j + 1) * Graphics::tileSubIdentifiers.size());
					// Graphics::tileIdentifiers[j].resize(framenum);
					for (int k = 0; k < Graphics::tileSubIdentifiers.size(); k++)
					{
						m = j * Game::TileSubType.size() - j;
						// for (int l = 0; l < framenum; l++)
						// {
						if (framenum > 1)
						{
							for (int l = 0; l < framenum; l++ /* , m++ */)
							{
								Graphics::tileTextures[/*j*/m + k].resize(framenum);
								Graphics::tileIdentifiers[m + k].resize(framenum);
								// frames.resize(k + m + 1);
								// frames[k + m] = identifier + Graphics::tileSubIdentifiers[k] + (char)(l + 1);
								frame << identifier << Graphics::tileSubIdentifiers[k] << (l + 1);
								Graphics::tileIdentifiers[m + k][l] = frame.str();
								// frames[k] = frame.str();
								frame.str(std::string());
							}
						}
						else
						{
							Graphics::tileTextures[/*j*/m + k].resize(framenum);
							Graphics::tileIdentifiers[m + k].resize(framenum);
							// frames.resize(k + 1);
							// frames[k] = identifier + Graphics::tileSubIdentifiers[k] /* + (char)(l + 1) */;
							frame << identifier << Graphics::tileSubIdentifiers[k];
							Graphics::tileIdentifiers[m + k][0] = frame.str();
							// frames[k] = frame.str();
							frame.str(std::string());
						}
						// }
					}
					Game::TileType[identifier] = j;
				}
				else if (i == GFXIDLocations["players"])
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
							m--;
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
					// Game::PlayerType[identifier] = j;
				}
				else if (i == GFXIDLocations["enemies"])
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
							m--;
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
					Game::EnemyType[identifier] = j;
				}
				else if (i == GFXIDLocations["collectibles"])
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
					Game::CollectibleType[identifier] = j;
				}
				else if (i == GFXIDLocations["particles"])
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
					Game::ParticleType[identifier] = j;
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
	for (int i = 0; i < AudioIDLocations["total"]; i++)
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
				identifier = lua_tostring(L, -1);
				if (i == AudioIDLocations["sfx"])
				{
					Audio::sfx.resize(j + 1);
					Audio::sfxIdentifiers.resize(j + 1);
					Audio::sfxIdentifiers[j] = identifier;
					// Audio::sfxIdentifiers.insert(Audio::sfxIdentifiers.end(), lua_tostring(L, -1));
					Game::SoundEffectType[identifier] = j;
				}
				else if (i == AudioIDLocations["music"])
				{
					Audio::music.resize(j + 1);
					Audio::musicIdentifiers.resize(j + 1);
					Audio::musicIdentifiers[j] = identifier;
					// Audio::musicIdentifiers.insert(Audio::musicIdentifiers.end(), lua_tostring(L, -1));
					Game::MusicType[identifier] = j;
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
	// lua_getglobal(L, "gBackground");	// gBackground
	// lua_pushstring(L, "bgSetType");	// "bgSetType"
	// lua_gettable(L, -2);				// bgSetType()
	return 0;
}

int LuaBridge::labChangeLevel()
{
	lua_getglobal(L, "bgSetType");				// bgSetType()
	lua_pushnumber(L, Graphics::bgState + 1);	// bgSetType(), bgState
	lua_call(L, 1, 0);							// 

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
	//lua_pushnumber(L, Level::LEVEL_UNITS);											// handleLevelLoad(), LEVEL_UNITS
	//lua_call(L, 1, 0);																//

	lua_getglobal(L, "things");															// things
	for (int i = 0; i < Game::things.size() /* Level::LEVEL_UNITS */; i++)
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
	lua_pop(L, 1);

	lua_pushnumber(L, (int)Level::LEVEL_W);		// levelW
	lua_setglobal(L, "LEVEL_W");				//
	lua_pushnumber(L, (int)Level::LEVEL_H);		// levelH
	lua_setglobal(L, "LEVEL_H");				//
	lua_pushnumber(L, (int)Level::LEVEL_UNITS); // levelUnits
	lua_setglobal(L, "LEVEL_UNITS");			//

	lua_getglobal(L, "init");
	lua_call(L, 0, 0);

	lua_getglobal(L, "things");										// things
	for (int i = 0; i < Game::things.size() /* Level::LEVEL_UNITS */; i++)
	{
		if (Game::things[i] != NULL && Game::things[i]->tgType != Game::ThingType["player"])
		{
			lua_pushnumber(L, i + 1);								// things, i
			lua_gettable(L, -2);									// things, specific thing

			lua_pushstring(L, "tgSubtype");						// things, specific thing, "tgSubtype"
			lua_pushnumber(L, Game::things[i]->tgGetSubtype());	// things, specific thing, "tgSubtype", subtype
			lua_settable(L, -3);								// things, specific thing
			lua_pop(L, 1);										// things
			/*
			if (Game::things[i]->tgType == Game::ThingType["enemy"])
			{
				lua_pushstring(L, "enSubtype");						// things, specific thing, "enSubtype"
				lua_pushnumber(L, Game::things[i]->tgGetSubtype());	// things, specific thing, "enSubtype", subtype
				lua_settable(L, -3);								// things, specific thing
				lua_pop(L, 1);										// things
			}
			else if (Game::things[i]->tgType == Game::ThingType["tile"])
			{
				lua_pushstring(L, "tiSubtype");						// things, specific thing, "tiSubtype"
				lua_pushnumber(L, Game::things[i]->tgGetSubtype());	// things, specific thing, "tiSubtype", subtype
				lua_settable(L, -3);								// things, specific thing
				lua_pop(L, 1);										// things
			}
			else if (Game::things[i]->tgType == Game::ThingType["collectible"])
			{
				lua_pushstring(L, "clSubtype");						// things, specific thing, "clSubtype"
				lua_pushnumber(L, Game::things[i]->tgGetSubtype());	// things, specific thing, "clSubtype", subtype
				lua_settable(L, -3);								// things, specific thing
				lua_pop(L, 1);										// things
			}
			*/
		}
	}
	lua_pop(L, 1);
	labInitThings();

	return 0;
}

int LuaBridge::labInitThings(void)
{
	//Game::centerCamera();
	lua_getglobal(L, "initThings");
	lua_call(L, 0, 0);
	labPullThings(false);

	for (int i = 0; i < Level::LEVEL_UNITS; i++)
	{
		if (Game::things[i] != NULL)
		{
			lua_getglobal(L, "things");							// things table
			lua_pushnumber(L, i + 1);							// things table, i
			lua_gettable(L, -2);								// things table, specific thing
			lua_getfield(L, -1, "tgHitbox");					// things table, specific thing, tgHitbox
			lua_getfield(L, -1, "w");							// things table, specific thing, tgHitbox, w
			lua_getfield(L, -2, "h");							// things table, specific thing, tgHitbox, w, h
			Game::things[i]->tgHitboxRect.w = (int)lua_tonumber(L, -2);
			Game::things[i]->tgHitboxRect.h = (int)lua_tonumber(L, -1);
			lua_pop(L, 5);										//
		}
	}
	lua_getglobal(L, "GFX_SCALE");					// GFX_SCALE
	if (lua_tonumber(L, -1) != Graphics::GFX_SCALE)
	{
		lua_pushnumber(L, Graphics::GFX_SCALE);		// GFX_SCALE (lua), GFX_SCALE (c++)
		lua_setglobal(L, "GFX_SCALE");				// GFX_SCALE (lua)
		lua_pushnumber(L, Graphics::GFX_MULT);		// GFX_SCALE (lua), GFX_MULT (c++)
		lua_setglobal(L, "GFX_MULT");				// GFX_SCALE (lua)
		lua_getglobal(L, "incGFXscale");			// GFX_SCALE (lua), incGFXscale()
		lua_call(L, 0, 0);							// GFX_SCALE (lua)
	}
	lua_pop(L, 1);									// 
	return 0;
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
	bool neednewpart = false;
	lua_pushnumber(L, Game::gScore);				// gScore
	lua_setglobal(L, "points");						// 
	lua_getglobal(L, "GFX_SCALE");					// GFX_SCALE
	if (lua_tonumber(L, -1) != Graphics::GFX_SCALE)
	{
		lua_pushnumber(L, Graphics::GFX_SCALE);		// GFX_SCALE (lua), GFX_SCALE (c++)
		lua_setglobal(L, "GFX_SCALE");				// GFX_SCALE (lua)
		lua_pushnumber(L, Graphics::GFX_MULT);		// GFX_SCALE (lua), GFX_MULT (c++)
		lua_setglobal(L, "GFX_MULT");				// GFX_SCALE (lua)
		lua_getglobal(L, "incGFXscale");			// GFX_SCALE (lua), incGFXscale()
		lua_call(L, 0, 0);							// GFX_SCALE (lua)
	}
	lua_pop(L, 1);									// 

	labPushThings();

	lua_getglobal(L, "handleEnvironment");
	lua_call(L, 0, 0);

	lua_getglobal(L, "points");									// points
	Game::gScore = (int)lua_tonumber(L, -1);					
	lua_pop(L, 1);												// 
	lua_getglobal(L, "GFX_SCALE");								// GFX_SCALE
	Graphics::GFX_SCALE = lua_tonumber(L, -1);
	lua_pop(L, 1);												//

	labPullThings();

	lua_getglobal(L, "gBackground");							// gBackground
	lua_getfield(L, -1, "bgType");								// gBackground, bgType
	if (Graphics::bgState != (int)lua_tonumber(L, -1))
		Graphics::bgState = (int)lua_tonumber(L, -1);
	lua_pop(L, 2);												// 

	lua_getglobal(L, "audioBuffer");								// audioBuffer
	for (int i = 1; i <= 2; i++)	// only sfx and music
	{
		lua_pushnumber(L, i);										// audioBuffer, i
		lua_gettable(L, -2);										// audioBuffer, (sfx or music)
		for (int j = 1;; j++)
		{
			lua_pushnumber(L, j);									// audioBuffer, (sfx or music), j
			lua_gettable(L, -2);									// audioBuffer, (sfx or music), (sfx track or music track or nil)
			if (lua_isnil(L, -1))
			{
				lua_pop(L, 1);										// audioBuffer, (sfx or music)
				break;
			}
			else
			{
				if (i == 1)
					Audio::auPlay(Game::SoundEffectType[lua_tostring(L, -1)], 's');
				else if (i == 2)
					Audio::auPlay(Game::MusicType[lua_tostring(L, -1)], 'm');
				lua_pop(L, 1);										// audioBuffer, (sfx or music)
				lua_pushnumber(L, j);								// audioBuffer, (sfx or music), j
				lua_pushnil(L);										// audioBuffer, (sfx or music), j, nil
				lua_settable(L, -3);								// audioBuffer, (sfx or music)
			}
		}
		lua_pop(L, 1);												// audioBuffer
	}
	lua_pop(L, 1);													//

	lua_getglobal(L, "totalParticles");							// totalParticles
	for (int i = 0; i < lua_tonumber(L, -1); i++)
	{
		lua_getglobal(L, "gParticles");							// totalParticles, gParticles
		lua_pushnumber(L, i + 1);								// totalParticles, gParticles, i
		lua_gettable(L, -2);									// totalParticles, gParticles, specific particle
		if (i >= Game::gParticles.size() && lua_isnil(L, -1) <= 0)
		{
			SDL_Rect pRect;
			int pType;
			SDL_Point pDestination;
			// int pLife;
			int pSpeedX, pSpeedY;

			lua_getfield(L, -1, "ptRect");							// totalParticles, gParticles, specific particle, ptRect
			lua_getfield(L, -1, "x");								// totalParticles, gParticles, specific particle, ptRect, x
			lua_getfield(L, -2, "y");								// totalParticles, gParticles, specific particle, ptRect, x, y
			lua_getfield(L, -3, "w");								// totalParticles, gParticles, specific particle, ptRect, x, y, w
			lua_getfield(L, -4, "h");								// totalParticles, gParticles, specific particle, ptRect, x, y, w, h
			pRect.x = lua_tonumber(L, -4);
			pRect.y = lua_tonumber(L, -3);
			pRect.w = lua_tonumber(L, -2);
			pRect.h = lua_tonumber(L, -1);
			//std::cout << pRect.x << " " << pRect.y << std::endl;
			lua_pop(L, 5);											// totalParticles, gParticles, specific particle
			lua_getfield(L, -1, "ptDestination");					// totalParticles, gParticles, specific particle, ptDestination
			lua_getfield(L, -1, "x");								// totalParticles, gParticles, specific particle, ptDestination, x
			lua_getfield(L, -2, "y");								// totalParticles, gParticles, specific particle, ptDestination, x, y
			pDestination.x = lua_tonumber(L, -2);
			pDestination.y = lua_tonumber(L, -1);
			lua_pop(L, 3);											// totalParticles, gParticles, specific particle
			lua_getfield(L, -1, "ptType");							// totalParticles, gParticles, specific particle, ptType
			// lua_getfield(L, -2, "ptLife");		// gParticles, specific particle, ptType, ptLife
			lua_getfield(L, -2, "ptSpeedX");						// totalParticles, gParticles, specific particle, ptType, ptSpeedX
			lua_getfield(L, -3, "ptSpeedY");						// totalParticles, gParticles, specific particle, ptType, ptSpeedX, ptSpeedY
			pType = lua_tonumber(L, -3);
			// pLife = lua_tonumber(L, -2);
			pSpeedX = lua_tonumber(L, -2);
			pSpeedY = lua_tonumber(L, -1);
			lua_pop(L, 3);											// totalParticles, gParticles, specific particle

			// Game::particles.resize(pNum + 1);
			/* Game::particles[pNum] = */
			Game::newParticle(&pRect, pType, /* pNum, */ &pDestination, pSpeedX, pSpeedY, i);
		}
		else if (i < Game::gParticles.size())
		{
			if (Game::gParticles[i] != NULL && Game::gParticles[i]->ptLife <= 0)
			{
				lua_pop(L, 1);											// totalParticles, gParticles
				lua_getglobal(L, "deleteParticle");						// totalParticles, gParticles, deleteParticle()
				lua_pushnumber(L, i + 1);								// totalParticles, gParticles, deleteParticle(), i
				lua_call(L, 1, 0);										// totalParticles, gParticles
				lua_pushnil(L);											// totalParticles, gParticles, nil (just so that lua_pop(L, 3) works)
				Game::destroyParticle(i);
			}
		}
		lua_pop(L, 2);													// totalParticles
	}

	/*
	for (int i = 0; i < lua_tonumber(L, -1); i++)
	{
		if (i < Game::particles.size() /* lua_tonumber(L, -1) - 1  && Game::particles[i] == NULL)
		{
			lua_getglobal(L, "gParticles");	// totalParticles, gParticles
			lua_pushnumber(L, i + 1);		// totalParticles, gParticles, i
			lua_gettable(L, -2);			// totalParticles, gParticles, (particle or nil)
			if (lua_isnil(L, -1) == 0)
			{
				neednewpart = true;
				lua_pop(L, 3);				// 
				lua_pushnumber(L, i + 1);	// i
			}
			else
				lua_pop(L, 2);				// totalParticles
		}
		if(i >= Game::particles.size() || neednewpart)
		{
			SDL_Rect pRect;
			int pType;
			int pNum = (int)lua_tonumber(L, -1) - (i > Game::particles.size() ? 0 : 1);
			SDL_Point pDestination;
			// int pLife;
			int pSpeedX, pSpeedY;

			lua_getglobal(L, "gParticles");							// (totalParticles or i), gParticles
			lua_insert(L, -2);										// gParticles, (totalParticles or i)
			lua_gettable(L, -2);									// gParticles, specific particle
			lua_pushstring(L, "ptRect");							// gParticles, specific particle, "ptRect"
			lua_gettable(L, -2);									// gParticles, specific particle, ptRect
			lua_pushstring(L, "x");									// gParticles, specific particle, ptRect, "x"
			lua_gettable(L, -2);									// gParticles, specific particle, ptRect, x
			lua_insert(L, -2);										// gParticles, specific particle, x, ptRect
			lua_pushstring(L, "y");									// gParticles, specific particle, x, ptRect, "y"
			lua_gettable(L, -2);									// gParticles, specific particle, x, ptRect, y
			lua_insert(L, -2);										// gParticles, specific particle, x, y, ptRect
			lua_pushstring(L, "w");									// gParticles, specific particle, x, y, ptRect, "w"
			lua_gettable(L, -2);									// gParticles, specific particle, x, y, ptRect, w
			lua_insert(L, -2);										// gParticles, specific particle, x, y, w, ptRect
			lua_pushstring(L, "h");									// gParticles, specific particle, x, y, w, ptRect, "h"
			lua_gettable(L, -2);									// gParticles, specific particle, x, y, w, ptRect, h
			lua_insert(L, -2);										// gParticles, specific particle, x, y, w, h, ptRect
			pRect.x = lua_tonumber(L, -5);
			pRect.y = lua_tonumber(L, -4);
			pRect.w = lua_tonumber(L, -3);
			pRect.h = lua_tonumber(L, -2);
			lua_pop(L, 5);											// gParticles, specific particle
			lua_pushstring(L, "ptDestination");						// gParticles, specific particle, "ptDestination"
			lua_gettable(L, -2);									// gParticles, specific particle, ptDestination
			lua_pushstring(L, "x");									// gParticles, specific particle, ptDestination, "x"
			lua_gettable(L, -2);									// gParticles, specific particle, ptDestination, x
			lua_insert(L, -2);										// gParticles, specific particle, x, ptDestination
			lua_pushstring(L, "y");									// gParticles, specific particle, x, ptDestination, "y"
			lua_gettable(L, -2);									// gParticles, specific particle, x, ptDestination, y
			lua_insert(L, -2);										// gParticles, specific particle, x, y, ptDestination
			pDestination.x = lua_tonumber(L, -3);
			pDestination.y = lua_tonumber(L, -2);
			lua_pop(L, 3);											// gParticles, specific particle
			lua_pushstring(L, "ptType");							// gParticles, specific particle, "ptType"
			lua_gettable(L, -2);									// gParticles, specific particle, ptType
			/*
			lua_pushstring(L, "ptLife");					// gParticles, specific particle, ptType, "ptLife"
			lua_gettable(L, -3);							// gParticles, specific particle, ptType, ptLife
			
			lua_pushstring(L, "ptSpeedX");							// gParticles, specific particle, ptType, "ptSpeedX"
			lua_gettable(L, -3);									// gParticles, specific particle, ptType, ptSpeedX
			lua_pushstring(L, "ptSpeedY");							// gParticles, specific particle, ptType, ptSpeedX, "ptSpeedY"
			lua_gettable(L, -4);									// gParticles, specific particle, ptType, ptSpeedX, ptSpeedY
			pType = lua_tonumber(L, -3);
			// pLife = lua_tonumber(L, -2);
			pSpeedX = lua_tonumber(L, -2);
			pSpeedY = lua_tonumber(L, -1);
			lua_pop(L, 5);											// 

			// Game::particles.resize(pNum + 1);
			/* Game::particles[pNum] = 
			Game::newParticle(&pRect, pType, /* pNum,  &pDestination, pSpeedX, pSpeedY, pNum);
		}
		lua_getglobal(L, "totalParticles");							// totalParticles
		neednewpart = false;
	}
	lua_pop(L, 1);												// 
	// }
	// else if (Game::particles.size() > lua_tonumber(L, -1))
	// {
	for (int i = 0; i < Game::particles.size(); i++)
		if (Game::particles[i] != NULL)
			if (Game::particles[i]->ptLife <= 0)
			{
				lua_pushnumber(L, Game::particles[i]->ptNumber + 1);	// [all particle numbers that need to be deleted]
				Game::destroyParticle(Game::particles[i]->ptNumber);
			}
	while (lua_isnumber(L, -1) > 0)
	{
		lua_getglobal(L, "deleteParticle");		// [particle #s to be deleted], deleteParticle()
		lua_insert(L, -2);						// [particle #s to be deleted], deleteParticle(), first particle # to be deleted
		lua_call(L, 1, 0);						// [particle #s to be deleted - 1, or nothing if all particles are deleted]
	}
	*/

		// while (Game::particles.size() > lua_tonumber(L, -1))
			// Game::destroyParticle(Game::particles.size() - 1);
	// 	lua_pop(L, 1);
	// }

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

int LuaBridge::labPushThings(void)
{
	for (int i = 0; i < Level::LEVEL_UNITS; i++)
	{
		if (Game::things[i] != NULL)
		{
			lua_getglobal(L, "things");							// things table
			lua_pushnumber(L, i + 1);							// things table, i
			lua_gettable(L, -2);								// things table, specific thing
			lua_pushnumber(L, Game::things[i]->tgVerticals);	// things table, specific thing, tgVerticals
			lua_setfield(L, -2, "tgVerticals");					// things table, specific thing
			lua_pushnumber(L, Game::things[i]->tgSpeed);		// things table, specific thing, tgSpeed
			lua_setfield(L, -2, "tgSpeed");						// things table, specific thing
			lua_pushnumber(L, Game::things[i]->tgHealth);		// things table, specific thing, tgHealth
			lua_setfield(L, -2, "tgHealth");					// things table, specific thing
			lua_pushnumber(L, Game::things[i]->tgDashing);		// things table, specific thing, tgDashing
			lua_setfield(L, -2, "tgDashing");					// things table, specific thing
			lua_getfield(L, -1, "tgHitbox");					// things table, specific thing, tgHitbox
			lua_pushnumber(L, Game::things[i]->tgHitboxRect.x);	// things table, specific thing, tgHitbox, x
			lua_setfield(L, -2, "x");							// things table, specific thing, tgHitbox
			lua_pushnumber(L, Game::things[i]->tgHitboxRect.y);	// things table, specific thing, tgHitbox, y
			lua_setfield(L, -2, "y");							// things table, specific thing, tgHitbox
			/*
			lua_pushstring(L, "w");								// things table, specific thing, tgHitbox, "w"
			lua_pushnumber(L, Game::things[i]->tgHitboxRect.w); // things table, specific thing, tgHitbox, "w", w
			lua_settable(L, -3);								// things table, specific thing, tgHitbox
			lua_pushstring(L, "h");								// things table, specific thing, tgHitbox, "h"
			lua_pushnumber(L, Game::things[i]->tgHitboxRect.h); // things table, specific thing, tgHitbox, "h", h
			lua_settable(L, -3);								// things table, specific thing, tgHitbox
			*/
			lua_pop(L, 1);										// things table, specific thing

			// if (Game::things[i]->tgType == Game::ThingType["tile"] || Game::things[i]->tgType == Game::ThingType["collectible"] || Game::things[i]->tgType == Game::ThingType["enemy"])
			if (Game::things[i]->tgType != Game::ThingType["temp"])
			{
				for (int j = 0; j < Game::things[i]->tgColliding.size(); j++)
				{
					//if (Game::things[i]->tgColliding[j] != -1)
					//{
					lua_getfield(L, -1, "tgColliding");						// things, specific thing, tgColliding
					lua_pushnumber(L, j + 1);								// things, specific thing, tgColliding, direction
					lua_pushnumber(L, (Game::things[i]->tgColliding[j] == -1 ? -1 : Game::things[i]->tgColliding[j] + 1));	// things, specific thing, tgColliding, direction, thing2->tgLevelUnit
					lua_settable(L, -3);									// things, specific thing, tgColliding
					lua_pop(L, 1);											// things, specific thing
					/*
					lua_pushstring(L, "tgColDir");					// things, specific thing, "tgColDir"
					lua_pushnumber(L, j + 1);						// things, specific thing, "tgColDir", dir
					lua_settable(L, -3);							// things, specific thing
					*/
					//}
				}
			}
			if (Game::things[i]->tgType == Game::ThingType["player"])
			{
				lua_pushnumber(L, Game::gPlayer->plActionFrames);	// things, specific thing, plActionFrames
				lua_setfield(L, -2, "plActionFrames");				// things, specific thing
			}

			/*
			if (Game::things[i]->tgType == Game::ThingType["enemy"])
			{
			lua_getfield(L, -1, "enHandleAI");			// things table, specific thing, enHandleAI()
			lua_call(L, 0, 0);							// things table, specific thing
			// lua_pop(L, 1);							// things table, specific thing
			}
			else if (Game::things[i]->tgType == Game::ThingType["tile"])
			{
			lua_getfield(L, -1, "tiHandleAI");			// things table, specific thing, tiHandleAI()
			lua_call(L, 0, 0);							// things table, specific thing
			// lua_pop(L, 1);
			}
			else if (Game::things[i]->tgType == Game::ThingType["player"])
			{
			// ?
			}
			else if (Game::things[i]->tgType == Game::ThingType["collectible"])
			{
			// lua_getfield(L, -1, "clHandleAI");	// things table, specific thing, clHandleAI()
			// lua_call(L, 0, 0);					// things table, specific thing
			if (Game::things[i]->tgHealth == 0)
			{
			lua_getfield(L, -1, "clCollect");		// things table, specific thing, clCollect()
			lua_call(L, 0, 0);						// things table, specific thing
			}
			}
			*/
			lua_pop(L, 2);										// 
		}
		else if (Game::things[i] == NULL)
		{
			lua_getglobal(L, "things");							// things table
			lua_pushnumber(L, i + 1);							// things table, i
			lua_gettable(L, -2);								// things table, specific thing
			if (lua_type(L, -1) != LUA_TNUMBER)
			{
				lua_pop(L, 1);								// things table
				lua_pushnumber(L, i + 1);					// things table, i
				lua_pushnumber(L, -1);						// things table, i, -1
				lua_settable(L, -3);						// things table
				lua_pop(L, 1);								// 
			}
			else
				lua_pop(L, 2);								// 
		}
	}
	return 0;
}

int LuaBridge::labPullThings(bool searchForNewThings)
{
	for (int i = 0; i < Game::things.size(); i++)
	{
		if (Game::things[i] != NULL)
		{
			lua_getglobal(L, "things");							// things table
			lua_pushnumber(L, i + 1);							// things table, i
			lua_gettable(L, -2);								// things table, specific thing
			lua_getfield(L, -1, "tgVerticals");					// things table, specific thing, tgVerticals
			Game::things[i]->tgVerticals = (int)lua_tonumber(L, -1);
			lua_pop(L, 1);										// things table, specific thing
			lua_getfield(L, -1, "tgSpeed");						// things table, specific thing, tgSpeed
			Game::things[i]->tgSpeed = (int)lua_tonumber(L, -1);
			/*
			if (Game::things[i]->tgSpeed > 0)
			Game::things[i]->tgDirection = RIGHT;
			else if (Game::things[i]->tgSpeed < 0)
			Game::things[i]->tgDirection = LEFT;
			*/
			lua_pop(L, 1);										// things table, specific thing
			lua_getfield(L, -1, "tgHealth");					// things table, specific thing, tgHealth
			Game::things[i]->tgHealth = (int)lua_tonumber(L, -1);
			lua_pop(L, 1);										// things table, specific thing
			lua_getfield(L, -1, "tgFrame");						// things table, specific thing, tgFrame
			Game::things[i]->tgFrame = (int)lua_tonumber(L, -1) - 1;
			lua_pop(L, 1);										// things table, specific thing

			if (Game::things[i]->tgType == Game::ThingType["player"])
			{
				lua_getfield(L, -1, "plActionFrames");					// things table, specific thing, plActionFrames
				Game::gPlayer->plActionFrames = lua_tonumber(L, -1);	// things table, specific thing, plActionFrames
				lua_pop(L, 1);											// things table, specific thing
			}

			// lua_getglobal(L, "things");		// things table
			// lua_pushnumber(L, i);			// things table, i
			// lua_gettable(L, -2);				// things table, specific thing
			lua_getfield(L, -1, "tgHitbox");					// things table, specific thing, tgHitbox
			lua_getfield(L, -1, "x");							// things table, specific thing, tgHitbox, x
			lua_getfield(L, -2, "y");							// things table, specific thing, tgHitbox, x, y
			/*
			lua_pushstring(L, "w");						// things table, specific thing, x, y, tgHitbox, "w"
			lua_gettable(L, -2);						// things table, specific thing, x, y, tgHitbox, w
			lua_insert(L, -2);							// things table, specific thing, x, y, w, tgHitbox
			lua_pushstring(L, "h");						// things table, specific thing, x, y, w, tgHitbox, "h"
			lua_gettable(L, -2);						// things table, specific thing, x, y, w, tgHitbox, h
			lua_insert(L, -2);							// things table, specific thing, x, y, w, h, tgHitbox
			*/
			Game::things[i]->tgHitboxRect.x = (int)lua_tonumber(L, -2);
			Game::things[i]->tgHitboxRect.y = (int)lua_tonumber(L, -1);
			// Game::things[i]->tgHitboxRect.x = (int)lua_tonumber(L, -5);
			// Game::things[i]->tgHitboxRect.y = (int)lua_tonumber(L, -4);
			// Game::things[i]->tgHitboxRect.w = (int)lua_tonumber(L, -3);
			// Game::things[i]->tgHitboxRect.h = (int)lua_tonumber(L, -2);
			lua_pop(L, 5);										//
		}
		else if (searchForNewThings && Game::things[i] == NULL)
		{
			lua_getglobal(L, "things");							// things table
			lua_pushnumber(L, i + 1);							// things table, i
			lua_gettable(L, -2);								// things table, specific thing
			if (lua_type(L, -1) != LUA_TNUMBER)
			{
				lua_getfield(L, -1, "tgType");						// things table, specific thing, tgType
				lua_getfield(L, -2, "tgSubtype");					// things table, specific thing, tgSubtype
				/*
				if(Game::ThingType[lua_tostring(L, -1)] == Game::ThingType["tile"])
					lua_getfield(L, -2, "tiSubtype");				// things table, specific thing, tgType, tgSubtype
				if(Game::ThingType[lua_tostring(L, -1)] == Game::ThingType["enemy"])
					lua_getfield(L, -2, "enSubtype");				// things table, specific thing, tgType, tgSubtype
				if(Game::ThingType[lua_tostring(L, -1)] == Game::ThingType["collectible"])
					lua_getfield(L, -2, "clSubtype");				// things table, specific thing, tgType, tgSubtype
					*/
				Game::newThing(Game::ThingType[lua_tostring(L, -2)], i, -1, -1, lua_tonumber(L, -1));
				lua_pop(L, 2);										// things table, specific thing
				lua_getfield(L, -1, "tgHitbox");					// things table, specific thing, tgHitbox
				lua_pushnumber(L, Game::things[i]->tgHitboxRect.x);	// things table, specific thing, tgHitbox, x
				lua_setfield(L, -2, "x");							// things table, specific thing, tgHitbox
				lua_pushnumber(L, Game::things[i]->tgHitboxRect.y);	// things table, specific thing, tgHitbox, y
				lua_setfield(L, -2, "y");							// things table, specific thing, tgHitbox
				lua_pop(L, 3);									// 
				labInitThings();
			}
			else
				lua_pop(L, 2);								// 
		}
	}
	return 0;
}

int LuaBridge::labCheckCollision(lua_State* L)
{
	SDL_Rect t1;
	SDL_Rect t2;
	bool collided = false;
	t1.x = lua_getfield(L, 1, "x"); t1.y = lua_getfield(L, 1, "y");
	t1.w = lua_getfield(L, 1, "w"); t1.h = lua_getfield(L, 1, "h");
	if (lua_type(L, 2) != LUA_TNIL) {
		t2.x = lua_getfield(L, 2, "x"); t2.y = lua_getfield(L, 2, "y");
		t2.w = lua_getfield(L, 2, "w"); t2.h = lua_getfield(L, 2, "h");
		if ((t1.x + t1.w > t2.x || t1.x < t2.x + t2.w) && (t1.y + t1.h > t2.y && t1.y < t2.y + t2.h))
			collided = true;
	}
	else {
		for (int i = 0; i < Game::things.size(); i++) {
			if (Game::things[i] != NULL && Game::things[i]->tgType != Game::ThingType["temp"]) {
				t2.x = Game::things[i]->tgHitboxRect.x; t2.y = Game::things[i]->tgHitboxRect.y;
				t2.w = Game::things[i]->tgHitboxRect.w; t2.h = Game::things[i]->tgHitboxRect.h;
				if ((t1.x + t1.w > t2.x || t1.x < t2.x + t2.w) && (t1.y + t1.h > t2.y && t1.y < t2.y + t2.h))
					collided = true;
			}
		}
	}
	lua_pushboolean(L, collided);
	return 1;
}

int LuaBridge::labClose(void)
{
	lua_close(L);
	return 0;
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