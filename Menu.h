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

class Menu
{
public:
	Menu();
	~Menu();

	static void muCreateMenu(void);
	static bool muHandleMenu(SDL_Event*);
	static void muRender(void);
	static void muMapButton(int, std::string, int);
	static void muUpdateButtons(void);
	static int muGetMenuPos(std::string);

	static int muInitialized;
	static int muMenu;
	static bool muIsMapping;
	// static int muOldMenu;
	static std::map< std::string, int > MenuID;
	static std::vector< int > NumOptions;
	static Texture* ttTitleTexture;
	static Texture* menuTexture;
	static std::vector< std::string > muOptions;
	static std::vector< std::string > ttOptions;
	static std::vector< Texture* > muOptionTextures;
	static std::vector< Texture* > ttOptionTextures;
	static std::vector< Texture* > muMiscTextures;
	static std::vector< Texture* > muGmControlTextures;
	static std::vector< Texture* > muLeControlTextures;
	static const int MISC_BUTTON_INDEX;
};

