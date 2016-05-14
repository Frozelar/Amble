#include "LevelEditor.h"
#include "Game.h"
#include "Level.h"
#include "Graphics.h"
#include "Texture.h"
#include "Thing.h"
#include "Collectible.h"
#include "Enemy.h"
#include "Tile.h"
#include "Player.h"

LevelEditor* Game::gLevelEditor = NULL;

std::map< std::string, int > LevelEditor::leMsgs;
std::vector< Texture* > LevelEditor::leMsgTextures;
const int LevelEditor::MSG_DISPLAY_TIME = 180;
std::vector< int > LevelEditor::leMsgTimers;
std::map< std::string, int > LevelEditor::leControls;
Thing* LevelEditor::mouseThing;
// SDL_Rect LevelEditor::level = { 0, 0, 0, 0 };
int LevelEditor::leLvlMoveX = 0, LevelEditor::leLvlMoveY = 0;
int LevelEditor::leTotMoveX = 0, LevelEditor::leTotMoveY = 0;
int LevelEditor::DEFAULT_LVL_MOVE = Game::DEFAULT_W * Game::DEFAULT_H / 2;

LevelEditor::LevelEditor()
{
	int num = 0;
	mouseThing = new Tile(NULL, 1, -1);
	leControls["Up"] = SDLK_w;
	leControls["Down"] = SDLK_s;
	leControls["Left"] = SDLK_a;
	leControls["Right"] = SDLK_d;
	leControls["Place"] = SDL_BUTTON_LEFT;
	leControls["Delete"] = SDL_BUTTON_RIGHT;
	leControls["Subtype Up"] = SDLK_COLON;
	leControls["Subtype Down"] = SDLK_QUOTE;
	leControls["Type Up"] = SDLK_RIGHTBRACKET;
	leControls["Type Down"] = SDLK_LEFTBRACKET;
	leControls["Save"] = SDLK_q;
	leControls["Open"] = SDLK_e;
	leMsgs["saved"] = num;
	leMsgs["opened"] = ++num;
	leMsgTextures.resize(leMsgs.size());
	leMsgTimers.resize(leMsgTextures.size());
	for (int i = 0; i < leMsgTextures.size(); i++)
	{
		leMsgTextures[i] = new Texture(0, 0, 0, 0);
		leMsgTimers[i] = 0;
	}
	leMsgTextures[leMsgs["saved"]]->txLoadT("Level Saved (output.map)", Game::gBodyFont.font, Game::gBodyFont.color);
	leMsgTextures[leMsgs["saved"]]->txRect.x = Game::WINDOW_W - leMsgTextures[leMsgs["saved"]]->txRect.w - Game::DEFAULT_W;
	leMsgTextures[leMsgs["saved"]]->txRect.y = Game::WINDOW_H - leMsgTextures[leMsgs["saved"]]->txRect.h - Game::DEFAULT_H;
	leMsgTextures[leMsgs["opened"]]->txLoadT("Level Opened (output.map)", Game::gBodyFont.font, Game::gBodyFont.color);
	leMsgTextures[leMsgs["opened"]]->txRect.x = Game::WINDOW_W - leMsgTextures[leMsgs["opened"]]->txRect.w - Game::DEFAULT_W;
	leMsgTextures[leMsgs["opened"]]->txRect.y = Game::WINDOW_H - leMsgTextures[leMsgs["opened"]]->txRect.h - Game::DEFAULT_H;
}

LevelEditor::~LevelEditor()
{
	if (mouseThing != NULL)
	{
		delete mouseThing;
		mouseThing = NULL;
	}
	for (int i = 0; i < leMsgTextures.size(); i++)
		if (leMsgTextures[i] != NULL)
		{
			delete leMsgTextures[i];
			leMsgTextures[i] = NULL;
		}
}

/*
bool LevelEditor::leInitLevel()
{
	level = { 0, 0, Level::LEVEL_W, Level::LEVEL_H };
	return true;
}
*/

bool LevelEditor::leEnter()
{
	leOpen();
	if (mouseThing == NULL)
		mouseThing = new Tile(NULL, 1, -1);
	return true;
}

bool LevelEditor::leExit()
{
	if (mouseThing != NULL)
	{
		delete mouseThing;
		mouseThing = NULL;
	}
	return true;
}

bool LevelEditor::leHandleEnvironment(SDL_Event* e)
{
	int mx = 0, my = 0;
	int newType = 0;
	int i = 0;
	bool changedThing = false;
	static int isDragging = 0;
	SDL_GetMouseState(&mx, &my);
	// i = (mx * my) / (Game::DEFAULT_W * Game::DEFAULT_H);
	i = ((my - leTotMoveY) / Game::DEFAULT_H * Level::LEVEL_W) + ((mx - leTotMoveX) / Game::DEFAULT_W);

	if (e->type == SDL_MOUSEMOTION)
	{
		if (mouseThing != NULL && mx >= 0 + leTotMoveX && my >= 0 + leTotMoveY && mx < Level::LEVEL_W_PIXELS + leTotMoveX && my < Level::LEVEL_H_PIXELS + leTotMoveY)
		{
			mouseThing->tgHitboxRect.x = mx;
			mouseThing->tgHitboxRect.y = my;
		}
	}
	if (e->type == SDL_MOUSEBUTTONDOWN || isDragging)
	{
		if (e->button.button == leControls["Place"] || isDragging == leControls["Place"])
		{
			if (mouseThing != NULL)
			{
				if(Game::things[i] == NULL)
					Game::newThing(mouseThing->tgType, i, mouseThing->tgHitboxRect.x, mouseThing->tgHitboxRect.y, mouseThing->tgGetSubtype());
			}
			else
			{
				Game::gPlayer->tgLevelUnit = i;
				Game::gPlayer->tgHitboxRect.x = mx / Game::DEFAULT_W * Game::DEFAULT_W;
				Game::gPlayer->tgHitboxRect.y = my / Game::DEFAULT_H * Game::DEFAULT_H;
			}
			isDragging = leControls["Place"];
		}
		else if (e->button.button == leControls["Delete"] || isDragging == leControls["Delete"])
		{
			if(Game::things[i] != NULL)
				if (Game::things[i]->tgType != Game::ThingType["player"])
					Game::destroyThing(i);
			isDragging = leControls["Delete"];
		}
	}
	if (e->type == SDL_MOUSEBUTTONUP)
	{
		if (isDragging > 0)
			isDragging = 0;
	}
	if (e->type == SDL_KEYDOWN)
	{
		if (e->key.keysym.sym == leControls["Up"])
			leLvlMoveY = -DEFAULT_LVL_MOVE;
		else if (e->key.keysym.sym == leControls["Down"])
			leLvlMoveY = DEFAULT_LVL_MOVE;
		else if (e->key.keysym.sym == leControls["Left"])
			leLvlMoveX = -DEFAULT_LVL_MOVE;
		else if (e->key.keysym.sym == leControls["Right"])
			leLvlMoveX = DEFAULT_LVL_MOVE;
	}
	if (e->type == SDL_KEYUP)
	{
		if (e->key.keysym.sym == leControls["Up"] && leLvlMoveY < 0)
			leLvlMoveY = 0;
		else if (e->key.keysym.sym == leControls["Down"] && leLvlMoveY > 0)
			leLvlMoveY = 0;
		else if (e->key.keysym.sym == leControls["Left"] && leLvlMoveX < 0)
			leLvlMoveX = 0;
		else if (e->key.keysym.sym == leControls["Right"] && leLvlMoveX > 0)
			leLvlMoveX = 0;
		else if (e->key.keysym.sym == leControls["Type Up"])
		{
			if(mouseThing == NULL)
				newType = Game::ThingType["player"] + 1;
			else if (mouseThing->tgType + 1 < Game::ThingType.size())
				newType = mouseThing->tgType + 1;
			else
				newType = mouseThing->tgType = 1;
			changedThing = true;
		}
		else if (e->key.keysym.sym == leControls["Type Down"])
		{
			if (mouseThing == NULL)
				newType = Game::ThingType.size() - 1;
			else if (mouseThing->tgType > 1)
				newType = mouseThing->tgType - 1;
			else
				newType = Game::ThingType.size() - 1;
			changedThing = true;
		}
		else if (e->key.keysym.sym == leControls["Subtype Up"])
		{
			std::cout << mouseThing->tgGetSubtype();
			if (mouseThing != NULL && 
				((mouseThing->tgType == Game::ThingType["collectible"] && mouseThing->tgGetSubtype() < Game::CollectibleType.size() - 1) ||
				(mouseThing->tgType == Game::ThingType["enemy"] && mouseThing->tgGetSubtype() < Game::EnemyType.size() - 1) ||
				(mouseThing->tgType == Game::ThingType["tile"] && mouseThing->tgGetSubtype() < (Game::TileType.size() - 1) * (Game::TileSubType.size() - 1))))
				mouseThing->tgSetSubtype(mouseThing->tgGetSubtype() + 1);
			else if(mouseThing != NULL)
				mouseThing->tgSetSubtype(1);
		}
		else if (e->key.keysym.sym == leControls["Subtype Down"])
		{
			if(mouseThing != NULL && mouseThing->tgGetSubtype() - 1 > 0)
				mouseThing->tgSetSubtype(mouseThing->tgGetSubtype() - 1);
			else if(mouseThing != NULL)
			{
				if (mouseThing->tgType == Game::ThingType["collectible"])
					mouseThing->tgSetSubtype(Game::CollectibleType.size() - 1);
				else if (mouseThing->tgType == Game::ThingType["enemy"])
					mouseThing->tgSetSubtype(Game::EnemyType.size() - 1);
				else if (mouseThing->tgType == Game::ThingType["tile"])
					mouseThing->tgSetSubtype(Game::TileType.size() - 1);
			}
		}
		else if (e->key.keysym.sym == leControls["Save"])
		{
			leSave();
		}
		else if (e->key.keysym.sym == leControls["Open"])
		{
			leOpen();
		}
		else if (e->key.keysym.sym == Game::gPlayer->plControls["Pause"])
		{
			Game::changeGameState(Game::GameState["menu"]);
		}
	}

	if (changedThing)
	{
		if (mouseThing != NULL)
		{
			delete mouseThing;
			mouseThing = NULL;
		}
		if (newType != Game::ThingType["player"])
		{
			if (newType == Game::ThingType["collectible"])
				mouseThing = new Collectible(NULL, 1, -1);
			else if (newType == Game::ThingType["enemy"])
				mouseThing = new Enemy(NULL, 1, -1);
			else if (newType == Game::ThingType["tile"])
				mouseThing = new Tile(NULL, 1, -1);
			mouseThing->tgHitboxRect.x = mx;
			mouseThing->tgHitboxRect.y = my;
		}
	}

	return true;
}

void LevelEditor::leMoveLevel()
{
	if (leLvlMoveX != 0 || leLvlMoveY != 0)
	{
		for (int i = 0; i < Game::things.size(); i++)
			if (Game::things[i] != NULL)
			{
				Game::things[i]->tgHitboxRect.x += leLvlMoveX;
				Game::things[i]->tgHitboxRect.y += leLvlMoveY;
			}
		leTotMoveX += leLvlMoveX;
		leTotMoveY += leLvlMoveY;
	}
}

void LevelEditor::leRender()
{
	for (int i = 0; i < leMsgTimers.size(); i++)
	{
		if (leMsgTimers[i] > 0 && leMsgTimers[i] < MSG_DISPLAY_TIME)
			leMsgTimers[i]++;
		else if (leMsgTimers[i] > 0)
			leMsgTimers[i] = 0;
	}
	SDL_SetRenderDrawColor(Game::gRenderer, 105, 105, 245, 255);
	SDL_RenderClear(Game::gRenderer);
	Graphics::gxRender(false);
	if(mouseThing != NULL)
		mouseThing->tgRender();
	for (int i = 0; i < leMsgTimers.size(); i++)
		if (leMsgTimers[i] > 0 && leMsgTextures[i] != NULL)
			leMsgTextures[i]->txRender();
	SDL_RenderPresent(Game::gRenderer);
}

bool LevelEditor::leSave()
{
	std::ofstream level;
	std::string ext = ".map";
	level.open(Game::rDir + "output" + ext);
	int value = 0;
	int unit = 0;
	std::string prefix = "";

	level << Level::LEVEL_W << " ";
	level << Level::LEVEL_H << std::endl;
	level << Level::levelBG /* + Game::OFFSET["BACKGROUND"] */ + 1 << " ";
	level << Level::levelTrack /* + Game::OFFSET["MUSIC"] */ + 1 << std::endl;

	for (int i = 0; i < Game::things.size(); i++)
	{
		if (Game::things[i] != NULL)
		{
			if (Game::things[i]->tgType == Game::ThingType["player"])
				value = Game::things[i]->tgType;
			else if (Game::things[i]->tgType == Game::ThingType["tile"])
				value = Game::things[i]->tgGetSubtype() + Game::OFFSET["TILE"];
			else if (Game::things[i]->tgType == Game::ThingType["enemy"])
				value = Game::things[i]->tgGetSubtype() + Game::OFFSET["ENEMY"];
			else if (Game::things[i]->tgType == Game::ThingType["collectible"])
				value = Game::things[i]->tgGetSubtype() + Game::OFFSET["COLLECTIBLE"];
		}
		else
			value = 0;
		if (value >= 0 && value <= 9)
			prefix = "000";
		else if (value >= 10 && value <= 99)
			prefix = "00";
		else if (value >= 100 && value <= 999)
			prefix = "0";
		else if (value >= 1000 && value <= 9999)
			prefix = "";
		else
			prefix = "ERROR";
		if (++unit >= Level::LEVEL_W)
		{
			unit = 0;
			level << prefix << value << std::endl;
		}
		else
			level << prefix << value << " ";
	}
	leMsgTimers[leMsgs["saved"]]++;

	return true;
}

bool LevelEditor::leOpen()
{
	Level::generateLevel(-1);
	leMsgTimers[leMsgs["opened"]]++;
	leLvlMoveX = leLvlMoveY = 0;
	leTotMoveX = leTotMoveY = 0;
	// leInitLevel();
	return true;
}