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

class Graphics
{
public:
	Graphics();

	~Graphics();

	static bool gxInit(void);

	static void gxRender(bool = true);

	static void gxClose(void);

	static void gxIncScale(bool = true);
	static void gxToggleFullscreen(bool = true);
	static void gxUpdateWindow(void);

	static bool isFullscreen;
	static float GFX_SCALE;
	static float GFX_MULT;
	static std::vector< std::vector<Texture*> > backgroundTextures;
	static int bgState;
	static int bgFrame;
	static std::vector< std::vector<Texture*> > tileTextures;
	static std::vector< std::vector<Texture*> > playerTextures;
	static std::vector< std::vector<Texture*> > collectibleTextures;
	static std::vector< std::vector<Texture*> > enemyTextures;
	static std::vector< std::vector<Texture*> > particleTextures;
	static std::vector< std::vector<std::string> > backgroundIdentifiers;
	static std::vector< std::vector<std::string> > playerIdentifiers;
	static std::vector< std::vector<std::string> > enemyIdentifiers;
	static std::vector< std::vector<std::string> > tileIdentifiers;
	static std::vector<std::string> tileSubIdentifiers;
	static std::vector< std::vector<std::string> > collectibleIdentifiers;
	static std::vector< std::vector<std::string> > particleIdentifiers;
	static std::vector< std::vector<std::string> > entityFrameTypeIdentifiers;
	static std::vector< std::vector<std::string> > objectFrameTypeIdentifiers;

	/*
	static std::vector<std::string> backgroundIdentifiers;
	static std::vector<std::string> enemyIdentifiers;
	static std::vector<std::string> tileIdentifiers;
	static std::vector<std::string> tileSubIdentifiers;
	static std::vector<std::string> collectibleIdentifiers;
	static std::vector<std::string> particleIdentifiers;
	static std::vector<std::string> entityFrameTypeIdentifiers;
	static std::vector<std::string> objectFrameTypeIdentifiers;
	*/
};