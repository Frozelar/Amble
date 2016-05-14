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

