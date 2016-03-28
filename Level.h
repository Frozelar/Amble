#pragma once
#include "Include.h"

class Level
{
public:
	Level();
	~Level();

	static const int levelW(void);
	static const int levelH(void);
	static bool generateLevel(void);
	static void moveLevel(void);
	static void playMusic(void);

	static int levelTrack;
	static int levelBG;
	static int LEVEL_UNITS;
	static int LEVEL_W;
	static int LEVEL_H;
	static int LEVEL_W_PIXELS;
	static int LEVEL_H_PIXELS;
	static int LEVEL_PIXELS;
	static int gLevelMovementsX;
	static int gLevelMovementsY;
};