#pragma once
#include "Include.h"

class Menu
{
public:
	Menu();
	~Menu();

	static void muCreateMenu(void);
	static void muHandleMenu(SDL_Event*);
	static void muRender(void);

	static int muInitialized;
};

