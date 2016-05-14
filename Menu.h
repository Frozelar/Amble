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

