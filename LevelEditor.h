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


#pragma once
#include "Include.h"

class LevelEditor
{
public:
	LevelEditor();
	~LevelEditor();

	// static ControlScheme leControls;
	static std::vector< Texture* > leMsgTextures;
	static std::vector< int > leMsgTimers;
	static const int MSG_DISPLAY_TIME;
	static std::map< std::string, int > leMsgs;
	static std::map< std::string, int > leControls;
	static Thing* mouseThing;
	static SDL_Rect level;
	static int leLvlMoveX, leLvlMoveY;
	static int leTotMoveX, leTotMoveY;
	static int DEFAULT_LVL_MOVE;
	static int leTakingInput;
	static Texture* leInputTexture;

	// static bool leInitLevel(void);
	static void leLoadMsgs(void);
	static bool leEnter(void);
	static bool leExit(void);
	static bool leHandleEnvironment(SDL_Event*);
	static void leMoveLevel(void);
	static void leRender(void);
	static bool leSave(void);
	static bool leOpen(void);
	static void leAcceptInput(SDL_Event*);
	static void leChangeDimensions(int, int);
};

