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
#pragma once
#define Lua_Register( _s, _funcname) \
    lua_register( s, #_funcname, LUA ## _funcname)

/*
enum GraphicIDLocations {
	BACKGROUNDS, TILES, PLAYERS, ENEMIES, COLLECTIBLES, PARTICLES, TOTAL_GRAPHIC_IDS
};
enum frameIDlocations {
	ENTITIES, OBJECTS, TOTAL_FRAME_IDS
};
enum AudioIDLocations {
	SFX, MUSIC, TOTAL_AUDIO_IDS
};
*/

class LuaBridge
{
public:
	LuaBridge();
	~LuaBridge();
	static int labInitValues(void);
	// static int labCall(int);
	static int labHandleEnvironment(void);
	static int labPushThings(void);
	static int labPullThings(bool = true);
	static int labInitThings(void);
	static int labChangeLevel(void);
	static int labCheckCollision(lua_State*);
	static int labClose(void);

	static std::map<std::string, int> GFXIDLocations;
	static std::map<std::string, int> FrameIDLocations;
	static std::map<std::string, int> AudioIDLocations;

	// static const int MAX_ARGS;
	// static void* labStack[];
	//static bool labChangedLevel;
	static lua_State* L;
	// static LuaBridge gLuaBridge;
};