#include "LevelEditor.h"
#include "Game.h"
#include "Level.h"
#include "Graphics.h"
#include "Thing.h"
#include "Collectible.h"
#include "Enemy.h"
#include "Tile.h"
#include "Player.h"

LevelEditor* Game::gLevelEditor = NULL;

std::map< std::string, int > LevelEditor::leControls;
Thing* LevelEditor::mouseThing;

LevelEditor::LevelEditor()
{
	mouseThing = new Tile(NULL, 1, -1);
	leControls["place"] = SDL_BUTTON_LEFT;
	leControls["delete"] = SDL_BUTTON_RIGHT;
	leControls["subtypeu"] = SDLK_r;
	leControls["subtyped"] = SDLK_e;
	leControls["typeu"] = SDLK_w;
	leControls["typed"] = SDLK_q;
	leControls["save"] = SDLK_s;
	leControls["open"] = SDLK_o;
}

LevelEditor::~LevelEditor()
{
	delete mouseThing;
	mouseThing = NULL;
}

bool LevelEditor::leHandleEnvironment(SDL_Event* e)
{
	int mx = 0, my = 0;
	int newType = 0;
	int i = 0;
	bool changedThing = false;
	SDL_GetMouseState(&mx, &my);
	// i = (mx * my) / (Game::DEFAULT_W * Game::DEFAULT_H);
	i = (my / Game::DEFAULT_H * Level::LEVEL_W) + (mx / Game::DEFAULT_W);

	if (e->type == SDL_MOUSEMOTION)
	{
		if (mx >= 0 && my >= 0 && mx < Level::LEVEL_W_PIXELS && my < Level::LEVEL_H_PIXELS)
		{
			mouseThing->tgHitboxRect.x = mx;
			mouseThing->tgHitboxRect.y = my;
		}
	}
	else if (e->type == SDL_MOUSEBUTTONDOWN)
	{

	}
	else if (e->type == SDL_MOUSEBUTTONUP)
	{
		if (e->button.button == leControls["place"])
		{
			if (mouseThing->tgType != Game::ThingType["player"])
				Game::newThing(mouseThing->tgType, i, mouseThing->tgHitboxRect.x, mouseThing->tgHitboxRect.y, mouseThing->tgGetSubtype());
			else
			{
				Game::gPlayer->tgHitboxRect.x = mx;
				Game::gPlayer->tgHitboxRect.y = my;
			}
		}
		else if (e->button.button == leControls["delete"])
		{
			if(Game::things[i]->tgType != Game::ThingType["player"])
				Game::destroyThing(i);
		}
	}
	else if (e->type == SDL_KEYUP)
	{
		if (e->key.keysym.sym == leControls["typeu"])
		{
			if (mouseThing->tgType + 1 < Game::ThingType.size())
				newType = mouseThing->tgType + 1;
			else
				newType = mouseThing->tgType = 1;
			changedThing = true;
		}
		else if (e->key.keysym.sym == leControls["typed"])
		{
			if (mouseThing->tgType > 1)
				newType = mouseThing->tgType - 1;
			else
				newType = Game::ThingType.size() - 1;
			changedThing = true;
		}
		else if (e->key.keysym.sym == leControls["subtypeu"])
		{
			if (mouseThing->tgType != Game::ThingType["player"] && 
				((mouseThing->tgType == Game::ThingType["collectible"] && mouseThing->tgGetSubtype() < Game::CollectibleType.size() - 1) ||
				(mouseThing->tgType == Game::ThingType["enemy"] && mouseThing->tgGetSubtype() < Game::EnemyType.size() - 1) ||
				(mouseThing->tgType == Game::ThingType["tile"] && mouseThing->tgGetSubtype() < (Game::TileType.size() - 1) * (Game::TileSubType.size() - 1))))
				mouseThing->tgSetSubtype(mouseThing->tgGetSubtype() + 1);
			else if(mouseThing->tgType != Game::ThingType["player"])
				mouseThing->tgSetSubtype(1);
		}
		else if (e->key.keysym.sym == leControls["subtyped"])
		{
			if(mouseThing->tgType != Game::ThingType["player"] && mouseThing->tgGetSubtype() - 1 > 0)
				mouseThing->tgSetSubtype(mouseThing->tgGetSubtype() - 1);
			else if(mouseThing->tgType != Game::ThingType["player"])
			{
				if (mouseThing->tgType == Game::ThingType["collectible"])
					mouseThing->tgSetSubtype(Game::CollectibleType.size() - 1);
				else if (mouseThing->tgType == Game::ThingType["enemy"])
					mouseThing->tgSetSubtype(Game::EnemyType.size() - 1);
				else if (mouseThing->tgType == Game::ThingType["tile"])
					mouseThing->tgSetSubtype(Game::TileType.size() - 1);
			}
		}
		else if (e->key.keysym.sym == leControls["save"])
		{
			leSave();
		}
		else if (e->key.keysym.sym == leControls["open"])
		{
			leOpen();
		}
		else if (e->key.keysym.sym == Game::gPlayer->plControls["pause"])
		{
			Game::changeGameState(Game::GameState["menu"]);
		}
	}

	if (changedThing)
	{
		delete mouseThing;
		if (newType == Game::ThingType["collectible"])
			mouseThing = new Collectible(NULL, 1, -1);
		else if (newType == Game::ThingType["enemy"])
			mouseThing = new Enemy(NULL, 1, -1);
		else if (newType == Game::ThingType["tile"])
			mouseThing = new Tile(NULL, 1, -1);
		mouseThing->tgHitboxRect.x = mx;
		mouseThing->tgHitboxRect.y = my;
	}

	return true;
}

void LevelEditor::leRender()
{
	SDL_SetRenderDrawColor(Game::gRenderer, 105, 105, 245, 255);
	SDL_RenderClear(Game::gRenderer);
	Graphics::gxRender(false);
	mouseThing->tgRender();
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

	return true;
}

bool LevelEditor::leOpen()
{
	Level::generateLevel(-1);
	return true;
}