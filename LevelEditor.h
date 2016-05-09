#pragma once
#include "Include.h"

class LevelEditor
{
public:
	LevelEditor();
	~LevelEditor();

	// static ControlScheme leControls;
	static std::map< std::string, int > leControls;
	static Thing* mouseThing;

	static bool leHandleEnvironment(SDL_Event*);
	static void leRender(void);
	static bool leSave(void);
	static bool leOpen(void);
};

