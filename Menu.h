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

	static int muInitialized;
	static int muMenu;
	// static int muOldMenu;
	static std::map< std::string, int > MenuID;
	static std::vector< int > NumOptions;
	static Texture* menuTexture;
	static std::vector< std::string > muOptions;
	static std::vector< std::string > ttOptions;
	static std::vector< Texture* > muOptionTextures;
	static std::vector< Texture* > ttOptionTextures;
};

