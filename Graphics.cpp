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


#include "Graphics.h"
#include "Game.h"
#include "Level.h"
#include "Texture.h"
#include "Thing.h"
#include "Player.h"
#include "Menu.h"
#include "LevelEditor.h"
#include "Particle.h"

bool Graphics::isFullscreen = false;
float Graphics::GFX_SCALE = 1.0;
float Graphics::GFX_MULT = 1.0;
std::vector< std::vector<Texture*> > Graphics::backgroundTextures(Game::BackgroundType["total"]);
int Graphics::bgState = 0;
int Graphics::bgFrame = 0;
// Texture Graphics::tileTextures[TOTAL_TILE_TYPES][TOTAL_OBJECT_FRAME_TYPES];
std::vector< std::vector<Texture*> > Graphics::tileTextures(Game::TileType["total"]);
std::vector< std::vector<Texture*> > Graphics::playerTextures(Game::EntityFrameType["total"]);
std::vector< std::vector<Texture*> > Graphics::collectibleTextures(Game::CollectibleType["total"]);
std::vector< std::vector<Texture*> > Graphics::enemyTextures(Game::EnemyType["total"]);
std::vector< std::vector<Texture*> > Graphics::particleTextures(Game::ParticleType["total"]);
std::vector< std::vector<std::string> > Graphics::backgroundIdentifiers(Game::BackgroundType["total"]);
std::vector< std::vector<std::string> > Graphics::playerIdentifiers;
std::vector< std::vector<std::string> > Graphics::enemyIdentifiers(Game::EnemyType["total"]);
std::vector< std::vector<std::string> > Graphics::tileIdentifiers(Game::TileType["total"]);
std::vector<std::string> Graphics::tileSubIdentifiers(Game::TileSubType["total"]);
std::vector< std::vector<std::string> > Graphics::collectibleIdentifiers(Game::CollectibleType["total"]);
std::vector< std::vector<std::string> > Graphics::particleIdentifiers(Game::ParticleType["total"]);
std::vector< std::vector<std::string> > Graphics::entityFrameTypeIdentifiers(Game::EntityFrameType["total"]);
std::vector< std::vector<std::string> > Graphics::objectFrameTypeIdentifiers(Game::ObjectFrameType["total"]);

/*
std::vector<std::string> Graphics::backgroundIdentifiers(TOTAL_BACKGROUND_TYPES);
std::vector<std::string> Graphics::enemyIdentifiers(TOTAL_ENEMY_TYPES);
std::vector<std::string> Graphics::tileIdentifiers(TOTAL_TILE_TYPES);
std::vector<std::string> Graphics::tileSubIdentifiers(TOTAL_TILE_SUBTYPES);
std::vector<std::string> Graphics::collectibleIdentifiers(TOTAL_COLLECTIBLE_TYPES);
std::vector<std::string> Graphics::particleIdentifiers(TOTAL_PARTICLE_TYPES);
std::vector<std::string> Graphics::entityFrameTypeIdentifiers(TOTAL_ENTITY_FRAME_TYPES);
std::vector<std::string> Graphics::objectFrameTypeIdentifiers(TOTAL_OBJECT_FRAME_TYPES);
*/

Graphics* Game::gGraphics;

Graphics::Graphics()
{
	if (!gxInit())
		std::cout << "Error initializing graphics!" << std::endl;
}

Graphics::~Graphics()
{
	gxClose();
}

bool Graphics::gxInit(void)
{
	bool isAnimated = false;

	/*
	Graphics::backgroundIdentifiers = { "Underground" };
	Graphics::enemyIdentifiers = { "Dude" };
	Graphics::tileIdentifiers = { "DirtBlock", "DirtWall" };
	Graphics::tileSubIdentifiers = { "Center", "Top", "TopRight", "Right", "BottomRight", "Bottom", "BottomLeft", "Left", "TopLeft" };
	Graphics::collectibleIdentifiers = { "Bit", "Byte" };
	Graphics::particleIdentifiers = { "Red", "Gray", "Blue", "BigRed", "BigGray", "BigBlue" };
	Graphics::entityFrameTypeIdentifiers = { "Idle", "WalkLeft", "WalkRight", "Jump", "Fall" };
	Graphics::objectFrameTypeIdentifiers = { "Frame1", "Frame2", "Frame3", "Frame4", "Frame5" };
	*/

	/*
	// backgroundTextures.resize(TOTAL_BACKGROUND_TYPES);
	for (int i = 0; i < TOTAL_BACKGROUND_TYPES; i++)
		backgroundTextures[i].resize(TOTAL_OBJECT_FRAME_TYPES);
	for (int i = 0; i < TOTAL_TILE_TYPES; i++)
		tileTextures[i].resize(TOTAL_OBJECT_FRAME_TYPES);
	for (int i = 0; i < TOTAL_COLLECTIBLE_TYPES; i++)
		collectibleTextures[i].resize(TOTAL_OBJECT_FRAME_TYPES);
	for (int i = 0; i < TOTAL_ENEMY_TYPES; i++)
		enemyTextures[i].resize(TOTAL_ENTITY_FRAME_TYPES);
	for (int i = 0; i < TOTAL_PARTICLE_TYPES; i++)
		particleTextures[i].resize(TOTAL_OBJECT_FRAME_TYPES);
	*/

	for (int i = 0; i < playerIdentifiers.size(); i++)
	{
		for (int j = 0; j < playerIdentifiers[i].size(); j++)
		{
			// playerTextures[i].resize(j + 1);		// should already be sized correctly from luabridge
			playerTextures[i][j] = new Texture();
			playerTextures[i][j]->txLoadF(Game::rDir + "pl" + playerIdentifiers[i][j] + Game::rExt);
		}
	}
	for (int i = 0; i < enemyIdentifiers.size(); i++)
	{
		for (int j = 0; j < enemyIdentifiers[i].size(); j++)
		{
			// enemyTextures[i].resize(j + 1);
			enemyTextures[i][j] = new Texture();
			enemyTextures[i][j]->txLoadF(Game::rDir + "en" + enemyIdentifiers[i][j] + Game::rExt);
		}
	}
	for (int i = 0; i < tileIdentifiers.size(); i++)
	{
		for (int j = 0; j < tileIdentifiers[i].size(); j++)
		{
			// tileTextures[i].resize(j + 1);
			tileTextures[i][j] = new Texture();
			tileTextures[i][j]->txLoadF(Game::rDir + "ti" + tileIdentifiers[i][j] + Game::rExt);
		}
	}
	for (int i = 0; i < collectibleIdentifiers.size(); i++)
	{
		for (int j = 0; j < collectibleIdentifiers[i].size(); j++)
		{
			// collectibleTextures[i].resize(j + 1);
			collectibleTextures[i][j] = new Texture();
			collectibleTextures[i][j]->txLoadF(Game::rDir + "cl" + collectibleIdentifiers[i][j] + Game::rExt);
		}
	}
	for (int i = 0; i < particleIdentifiers.size(); i++)
	{
		for (int j = 0; j < particleIdentifiers[i].size(); j++)
		{
			// particleTextures[i].resize(j + 1);
			particleTextures[i][j] = new Texture();
			particleTextures[i][j]->txLoadF(Game::rDir + "pt" + particleIdentifiers[i][j] + Game::rExt);
		}
	}
	for (int i = 0; i < backgroundIdentifiers.size(); i++)
	{
		for (int j = 0; j < backgroundIdentifiers[i].size(); j++)
		{
			// backgroundTextures[i].resize(j + 1);
			backgroundTextures[i][j] = new Texture();
			backgroundTextures[i][j]->txLoadF(Game::rDir + "bg" + backgroundIdentifiers[i][j] + Game::rExt);
		}
	}
	bgState = 0;

	/*
	for (int frame = 0; frame < playerTextures.size() && frame < entityFrameTypeIdentifiers.size(); frame++)
		playerTextures[frame].txLoadF(dir + "pl" + entityFrameTypeIdentifiers[frame] + ext);
	for (int type = 0, frame = 0; type < enemyIdentifiers.size();)
	{
		if (frame >= enemyTextures[type].size())
		{
			frame = 0;
			type++;
		}
		enemyTextures[type][frame].txLoadF(dir + "en" + enemyIdentifiers[type] + entityFrameTypeIdentifiers[frame] + ext);
	}
	for (int type = 0, frame = 0; type < collectibleIdentifiers.size(); frame++)
	{
		if (frame >= objectFrameTypeIdentifiers.size())
		{
			frame = 0;
			type++;
		}
		collectibleTextures[type][frame].txLoadF(dir + "cl" + collectibleIdentifiers[type] + objectFrameTypeIdentifiers[frame] + ext);
	}
	for (int type = 0, frame = 0; type < TOTAL_COLLECTIBLE_TYPES || type < TOTAL_PARTICLE_TYPES; frame++)
	{
		if (frame >= TOTAL_OBJECT_FRAME_TYPES)
		{
			frame = 0;
			type++;
		}
		particleTextures[type][frame].txLoadF(dir + "pt" + particleIdentifiers[type] + objectFrameTypeIdentifiers[frame] + ext);
	}

	for (int num = 1, type = 0, frameType = 0, framePos = 0; framePos < TOTAL_ENTITY_FRAME_TYPES; num++, framePos++)
	{
		if (type < TOTAL_ENEMY_TYPES)
			enemyTextures[type][framePos].txLoadF("resources/en" + enemyIdentifiers[type] +
				entityFrameTypeIdentifiers[frameType] + std::to_string(num) + ".png");
		playerTextures[framePos].txLoadF("resources/pl" + entityFrameTypeIdentifiers[frameType] + std::to_string(num) + ".png");
		if (num >= TOTAL_ANIMATION_FRAMES)
		{
			num = 0;
			frameType++;
		}
		if (frameType >= TOTAL_ENTITY_FRAME_TYPES / TOTAL_ANIMATION_FRAMES)
		{
			frameType = 0;
			type++;
		}
	}
	for (int type = 0, frame = 0; type < TOTAL_COLLECTIBLE_TYPES || type < TOTAL_PARTICLE_TYPES; frame++)
	{
		if (frame >= TOTAL_OBJECT_FRAME_TYPES)
		{
			frame = 0;
			type++;
		}
		if (type < TOTAL_COLLECTIBLE_TYPES && frame < TOTAL_OBJECT_FRAME_TYPES)
			collectibleTextures[type][frame].txLoadF("resources/cl" + collectibleIdentifiers[type] +
			objectFrameTypeIdentifiers[frame] + ".png");
		// if (frame < TOTAL_OBJECT_FRAME_TYPES && type < TOTAL_PARTICLE_TYPES)
		// 	particleTextures[type][frame].txLoadF("resources/pt" + particleIdentifiers[type] +
		// 	objectFrameTypeIdentifiers[frame] + ".png");
	}
	for (int type = 0, frame = 0; type < TOTAL_BACKGROUND_TYPES; frame++)
	{
		if (type == -10) // if bg is animated (change this later)
			isAnimated = true;
		else
			isAnimated = false;

		if (isAnimated)
		{
			if (frame >= TOTAL_OBJECT_FRAME_TYPES)
			{
				frame = 0;
				type++;
			}
			backgroundTextures[type][frame].txLoadF("resources/bg" + backgroundIdentifiers[type] + objectFrameTypeIdentifiers[frame] +
				".png");
		}
		else
		{
		// 	backgroundTextures[type][0].txLoadF("resources/bg" + backgroundIdentifiers[type] + ".png");
			type++;
		}
	}
	for (int pos = 0, num = 1, type = 0, subType = 0, frameType = 0; type < TOTAL_TILE_VARIETIES; pos++)
	{
		if (type == -10) // if tile is animated
			isAnimated = true;
		else
			isAnimated = false;

		if (isAnimated)
		{
			if (type < TOTAL_TILE_VARIETIES && frameType < TOTAL_OBJECT_FRAME_TYPES && subType < TOTAL_TILE_SUBTYPES && pos < TOTAL_TILE_TYPES)
				tileTextures[pos][frameType].txLoadF("resources/ti" + tileIdentifiers[type] + tileSubIdentifiers[subType] +
				objectFrameTypeIdentifiers[frameType] + ".png");
			if ((++num) >= TOTAL_ANIMATION_FRAMES)
			{
				num = 1;
				frameType++;
			}
			if (frameType >= TOTAL_OBJECT_FRAME_TYPES - 1)
			{
				frameType = 0;
				subType++;
			}
		}
		else
		{
			if (type < TOTAL_TILE_VARIETIES && subType < TOTAL_TILE_SUBTYPES && pos < TOTAL_TILE_TYPES)
				tileTextures[pos][0].txLoadF("resources/ti" + tileIdentifiers[type] + tileSubIdentifiers[subType] + ".png");
			subType++;
		}
		if (subType >= TOTAL_TILE_SUBTYPES)
		{
			subType = 0;
			type++;
		}
	}
	*/
	return true;
}

void Graphics::gxRender(bool updateRenderer)
{
	SDL_Rect cameraRect{ 0 - Game::DEFAULT_W * Game::DEFAULT_H, 0 - Game::DEFAULT_W * Game::DEFAULT_H,
		Game::WINDOW_W + Game::DEFAULT_W * Game::DEFAULT_H * 2, Game::WINDOW_H + Game::DEFAULT_W * Game::DEFAULT_H * 2 };
	Thing camera{ &cameraRect, Game::ThingType["temp"], -1 };
	
	if (updateRenderer)
	{
		SDL_SetRenderDrawColor(Game::gRenderer, 105, 105, 245, 255);
		SDL_RenderClear(Game::gRenderer);
	}

	bgState = Level::levelBG;
	Graphics::backgroundTextures[bgState][bgFrame]->txRect = { 0, 0, Game::WINDOW_W, Game::WINDOW_H };
	Graphics::backgroundTextures[bgState][bgFrame]->txRender();
	// gPlayer->tgRender();
	for (int i = 0; i < Game::things.size(); i++)
	{
		if (Game::things[i] != NULL)
		{
			if (Game::checkCollision(Game::things[i], &camera, -1, false) && Game::things[i]->tgType != Game::ThingType["temp"])
			{
				// Game::things[i]->tgSyncTexture();
				Game::things[i]->tgRender();
			}
		}
	}

	for (int i = 0; i < Game::gParticles.size(); i++)
	{
		if (Game::gParticles[i] != NULL)
		{
			if(Game::checkCollisionRects(&Game::gParticles[i]->ptRect, &cameraRect))
				Game::gParticles[i]->ptRender();
		}
	}

	if (updateRenderer)
		SDL_RenderPresent(Game::gRenderer);
}

void Graphics::gxClose(void)
{
	for (int i = 0; i < playerIdentifiers.size(); i++)
		for (int j = 0; j < playerIdentifiers[i].size(); j++)
			delete playerTextures[i][j];
	for (int i = 0; i < enemyIdentifiers.size(); i++)
		for (int j = 0; j < enemyIdentifiers[i].size(); j++)
			delete enemyTextures[i][j];
	for (int i = 0; i < tileIdentifiers.size(); i++)
		for (int j = 0; j < tileIdentifiers[i].size(); j++)
			delete tileTextures[i][j];
	for (int i = 0; i < collectibleIdentifiers.size(); i++)
		for (int j = 0; j < collectibleIdentifiers[i].size(); j++)
			delete collectibleTextures[i][j];
	for (int i = 0; i < particleIdentifiers.size(); i++)
		for (int j = 0; j < particleIdentifiers[i].size(); j++)
			delete particleTextures[i][j];
	for (int i = 0; i < backgroundIdentifiers.size(); i++)
		for (int j = 0; j < backgroundIdentifiers[i].size(); j++)
			delete backgroundTextures[i][j];
}

void Graphics::gxIncScale(bool gfxscale)
{
	std::string hfont = Game::rDir + Game::gHeadingFont.name;
	std::string bfont = Game::rDir + Game::gBodyFont.name;
	std::string tfont = Game::rDir + Game::gTitleFont.name;

	if (gfxscale)
	{
		if (GFX_SCALE == 1.0)
		{
			GFX_SCALE = 2.0;
			GFX_MULT = 2.0;
		}
		else if (GFX_SCALE == 2.0)
		{
			GFX_SCALE = 1.0;
			GFX_MULT = 0.5;
		}
	}

	Game::WINDOW_W *= GFX_MULT;
	Game::WINDOW_H *= GFX_MULT;
	Game::DEFAULT_W *= GFX_MULT;
	Game::DEFAULT_H *= GFX_MULT;
	Game::PLAYER_W *= GFX_MULT;
	Game::PLAYER_H *= GFX_MULT;
	Game::DEFAULT_ENEMY_W *= GFX_MULT;
	Game::DEFAULT_ENEMY_H *= GFX_MULT;
	Game::DEFAULT_SPEED *= GFX_MULT;
	Game::DEFAULT_GFX_OFFSET *= GFX_MULT;

	TTF_CloseFont(Game::gHeadingFont.font);
	TTF_CloseFont(Game::gBodyFont.font);
	TTF_CloseFont(Game::gTitleFont.font);
	Game::gHeadingFont.size *= GFX_MULT;
	Game::gBodyFont.size *= GFX_MULT;
	Game::gTitleFont.size *= GFX_MULT;
	Game::gHeadingFont.font = TTF_OpenFont(hfont.c_str(), Game::gHeadingFont.size);
	Game::gBodyFont.font = TTF_OpenFont(bfont.c_str(), Game::gBodyFont.size);
	Game::gTitleFont.font = TTF_OpenFont(tfont.c_str(), Game::gTitleFont.size);

	Game::gCamera = multDimensions(Game::gCamera, GFX_MULT);
	// Game::gCamera.x *= GFX_MULT;
	// Game::gCamera.y *= GFX_MULT;
	// Game::gCamera.w *= GFX_MULT;
	// Game::gCamera.h *= GFX_MULT;

	Game::gPlayer->tgHitboxRect = multDimensions(Game::gPlayer->tgHitboxRect, GFX_MULT);
	Game::gPlayer->tgGFXrect = multDimensions(Game::gPlayer->tgGFXrect, GFX_MULT);
	for (int i = 0; i < Game::things.size(); i++)
	{
		if (Game::things[i] != NULL && Game::things[i]->tgType != Game::ThingType["player"])
		{
			Game::things[i]->tgHitboxRect = multDimensions(Game::things[i]->tgHitboxRect, GFX_MULT);
			Game::things[i]->tgGFXrect = multDimensions(Game::things[i]->tgGFXrect, GFX_MULT);
		}
	}
	for (int i = 0; i < Game::gravityArray.size(); i++)
		Game::gravityArray[i] *= GFX_MULT;
	for (int i = 0; i < Game::jumpArray.size(); i++)
		Game::jumpArray[i] *= GFX_MULT;
	for (int i = 0; i < Game::dashArray.size(); i++)
		Game::dashArray[i] *= GFX_MULT;
	for (int i = 0; i < Game::floatArray.size(); i++)
		Game::floatArray[i] *= GFX_MULT;

	Menu::ttTitleTexture->txRect = multDimensions(Menu::ttTitleTexture->txRect, GFX_MULT);
	Menu::menuTexture->txRect = multDimensions(Menu::menuTexture->txRect, GFX_MULT);
	for (int i = 0; i < Menu::muOptionTextures.size(); i++)
		if (Menu::muOptionTextures[i] != NULL)
			Menu::muOptionTextures[i]->txRect = multDimensions(Menu::muOptionTextures[i]->txRect, GFX_MULT);
	for (int i = 0; i < Menu::ttOptionTextures.size(); i++)
		if (Menu::ttOptionTextures[i] != NULL)
			Menu::ttOptionTextures[i]->txRect = multDimensions(Menu::ttOptionTextures[i]->txRect, GFX_MULT);
	for (int i = 0; i < Menu::muMiscTextures.size(); i++)
		Menu::muMiscTextures[i]->txRect = multDimensions(Menu::muMiscTextures[i]->txRect, GFX_MULT);
	for (int i = 0; i < Menu::muGmControlTextures.size(); i++)
		Menu::muGmControlTextures[i]->txRect = multDimensions(Menu::muGmControlTextures[i]->txRect, GFX_MULT);
	for (int i = 0; i < Menu::muLeControlTextures.size(); i++)
		Menu::muLeControlTextures[i]->txRect = multDimensions(Menu::muLeControlTextures[i]->txRect, GFX_MULT);
	LevelEditor::mouseThing->tgHitboxRect = multDimensions(LevelEditor::mouseThing->tgHitboxRect, GFX_MULT);
	LevelEditor::mouseThing->tgGFXrect = multDimensions(LevelEditor::mouseThing->tgGFXrect, GFX_MULT);
	for (int i = 0; i < LevelEditor::leMsgTextures.size(); i++)
		LevelEditor::leMsgTextures[i]->txRect = multDimensions(LevelEditor::leMsgTextures[i]->txRect, GFX_MULT);
	LevelEditor::DEFAULT_LVL_MOVE *= GFX_MULT;
	LevelEditor::leTotMoveX *= GFX_MULT;
	LevelEditor::leTotMoveY *= GFX_MULT;
	LevelEditor::leInputTexture->txRect = multDimensions(LevelEditor::leInputTexture->txRect, GFX_MULT);
	LevelEditor::level = multDimensions(LevelEditor::level, GFX_MULT);

	Level::LEVEL_W_PIXELS *= GFX_MULT;
	Level::LEVEL_H_PIXELS *= GFX_MULT;
	Level::LEVEL_PIXELS *= GFX_MULT;

	gxUpdateWindow();
}

void Graphics::gxUpdateWindow()
{
	SDL_SetWindowSize(Game::gWindow, Game::WINDOW_W, Game::WINDOW_H);
}

void Graphics::gxToggleFullscreen(bool isfs)
{
	if(isfs)
		isFullscreen = !isFullscreen;
	SDL_SetWindowFullscreen(Game::gWindow, isFullscreen);
}