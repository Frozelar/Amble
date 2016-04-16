#pragma once
#include "Include.h"

class Graphics
{
public:
	Graphics();

	~Graphics();

	static bool gxInit(void);

	static void gxRender(void);

	static void gxClose(void);

	static std::vector< std::vector<Texture*> > backgroundTextures;
	static int bgState;
	static int bgFrame;
	static Texture* menuTexture;
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