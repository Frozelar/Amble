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
	static void labInitValues(void);
	// static int labCall(int);
	static int labHandleEnvironment(void);
	static int labCheckCollision(lua_State*);
	static int labClose(void);

	static std::map<std::string, int> GFXIDLocations;
	static std::map<std::string, int> FrameIDLocations;
	static std::map<std::string, int> AudioIDLocations;

	// static const int MAX_ARGS;
	// static void* labStack[];
	static bool labChangedLevel;
	static lua_State* L;
	// static LuaBridge gLuaBridge;
};