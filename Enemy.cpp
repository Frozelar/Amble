#include "Enemy.h"
#include "Game.h"
#include "Graphics.h"
#include "Level.h"
#include "Texture.h"

Enemy::Enemy(SDL_Rect* box, int subtype, int unit) : Thing(box, Game::ThingType["enemy"], unit)
{
	tgHitboxRect.w = Game::DEFAULT_ENEMY_W;
	tgHitboxRect.h = Game::DEFAULT_ENEMY_H;
	tgHitboxRect.x = (box == NULL ? ((unit - ((unit / Level::LEVEL_W) * Level::LEVEL_W)) * Game::DEFAULT_W) : box->x);
	tgHitboxRect.y = (box == NULL ? ((unit / Level::LEVEL_W) * Game::DEFAULT_H) : box->y);
	tgGFXrect.x = tgHitboxRect.x - Game::DEFAULT_OFFSET;
	tgGFXrect.y = tgHitboxRect.y - Game::DEFAULT_OFFSET;
	tgGFXrect.w = Game::DEFAULT_ENEMY_W + Game::DEFAULT_OFFSET * 2;
	tgGFXrect.h = Game::DEFAULT_ENEMY_H + Game::DEFAULT_OFFSET * 2;
	enType = subtype;
}

void Enemy::tgResolveCollision(Thing* thing, int dir)
{
	// tgColliding[dir].thing1 = Game::things[tgLevelUnit];
	tgColliding[dir] = thing->tgLevelUnit;

	if (thing->tgType == Game::ThingType["player"] || thing->tgType == Game::ThingType["enemy"])
	{
		if (dir == Game::Direction["right"] || dir == Game::Direction["left"])
		{
			// thing->tgSpeed *= 0.5;
		}
		else if (thing->tgVerticals != 0)
		{
			// thing->tgVerticals = (tgVerticals < 0 ? Game::jumpArray.size() - 25 : Game::gravityArray.size() - 1);
		}
		else
			std::cout << "Collision error: " << tgLevelUnit << " " << thing->tgLevelUnit << std::endl;

		// if (thing->tgType == Game::ThingType["player"])
		// 	thing->tgHealth -= enPower;
	}
	/*
	else if (thing->tgType == Game::ThingType["tile"])
	{
		enIsColliding = true;
		if (dir == Game::Direction["right"] || dir == Game::Direction["left"])
		{
			// tgDirection = invertDir(tgDirection);
			tgSpeed = -tgSpeed;
		}
		else if (dir == Game::Direction["down"])
		{
			if (tgVerticals < 0)
				tgVerticals = 1;
		}
		else if (dir == Game::Direction["up"])
		{
			if (tgVerticals > 0)
				tgVerticals = 0;
		}
	}
	*/
}

void Enemy::tgRender(void)
{
	tgSyncTexture();

	/*
	++tgFrameWaitCounter;
	if ((tgSpeed != 0 && tgFrameWaitCounter % 4 == 0) ||
		(tgFrame == Game::EntityFrameType["Idle1"] && tgFrameWaitCounter % 64 == 0) ||
		(tgFrame > Game::EntityFrameType["Idle1"] && tgFrame <= Game::EntityFrameGroup["Idle"] && tgFrameWaitCounter % 16 == 0))
	{
		tgFrame++;
		tgFrameWaitCounter = 0;
	}

	if (tgVerticals < 0 && (tgFrame < Game::EntityFrameType["Jump1"] || tgFrame > Game::EntityFrameGroup["Jump"]))
		tgFrame = Game::EntityFrameType["Jump1"];
	else if (tgVerticals > 0 && (tgFrame < Game::EntityFrameType["Fall1"] || tgFrame > Game::EntityFrameGroup["Fall"]))
		tgFrame = Game::EntityFrameType["Fall1"];
	else if (tgSpeed < 0 && tgVerticals == 0 && (tgFrame < Game::EntityFrameType["WalkLeft1"] || tgFrame > Game::EntityFrameGroup["WalkLeft"]))
		tgFrame = Game::EntityFrameType["WalkLeft1"];
	else if (tgSpeed > 0 && tgVerticals == 0 && (tgFrame < Game::EntityFrameType["WalkRight1"] || tgFrame > Game::EntityFrameGroup["WalkRight"]))
		tgFrame = Game::EntityFrameType["WalkRight1"];
	else if (tgSpeed == 0 && tgVerticals == 0 && (tgFrame < Game::EntityFrameType["Idle1"] || tgFrame > Game::EntityFrameGroup["Idle"]))
		tgFrame = Game::EntityFrameType["Idle1"];
		*/

	Graphics::enemyTextures[enType - 1][tgFrame]->txRect = tgGFXrect;
	Graphics::enemyTextures[enType - 1][tgFrame]->txRender();
}

void Enemy::tgApplyAI(void)
{
	// int dir = Game::Direction["none"];
	// bool colliding = Game::checkCollision(Game::things[tgLevelUnit], NULL, tgLevelUnit, true);
	// Thing nextMove{ &tgHitboxRect, Game::ThingType["temp"], tgLevelUnit };
	// int moveX = 0, moveY = 0;
	// bool left = false, right = false, up = false, down = false;
	bool collided = false;

	tgHandleVerticals();

	if (tgSpeed != 0)
	{
		tgHitboxRect.x += tgSpeed;
		if (Game::checkCollision(Game::things[tgLevelUnit], NULL, tgLevelUnit, true))
			for (int i = 0; i < Game::gColliding.size(); i++)
				if (Game::gColliding[i] != NULL)
				{
					collided = true;
					Game::gColliding[i]->thing2->tgResolveCollision(Game::things[tgLevelUnit], (tgSpeed > 0 ? Game::Direction["right"] : Game::Direction["left"]));
				}
	}
	if (!collided /* || (collided && tgSpeed > 0) */)
		tgColliding[Game::Direction["left"]] = -1;
	if (!collided /* || (collided && tgSpeed < 0) */)
		tgColliding[Game::Direction["right"]] = -1;
	if (collided)
		collided = false;

	if (tgVerticals == 0)
	{
		tgHitboxRect.y++;
		if (!Game::checkCollision(Game::things[tgLevelUnit], NULL, tgLevelUnit, false))
			tgVerticals++;
		tgHitboxRect.y--;
	}
	if (tgVerticals != 0)
	{
		tgHitboxRect.y += (tgVerticals < 0 ? -Game::jumpArray[-tgVerticals] : Game::gravityArray[tgVerticals]);
		if (Game::checkCollision(Game::things[tgLevelUnit], NULL, tgLevelUnit, true))
		{
			for (int i = 0; i < Game::gColliding.size(); i++)
				if (Game::gColliding[i] != NULL)
				{
					collided = true;
					Game::gColliding[i]->thing2->tgResolveCollision(Game::things[tgLevelUnit], (tgVerticals > 0 ? Game::Direction["down"] : Game::Direction["up"]));
				}
		}
	}
	if (!collided /* || (collided && tgVerticals > 0) */)
		tgColliding[Game::Direction["up"]] = -1;
	if (!collided /* || (collided && tgVerticals < 0) */)
		tgColliding[Game::Direction["down"]] = -1;

	/*
	if (!Game::checkCollision(Game::things[tgLevelUnit], NULL, tgLevelUnit, false))
		for (int i = 0; i < tgColliding.size(); i++)
			tgColliding[i] = -1;
			*/

	/*
	tgHandleVerticals();
	// tgHitboxRect.x += tgSpeed;
	SDL_Rect directionCheckRect{ tgHitboxRect.x, tgHitboxRect.y, 1, 1 /* tgHitboxRect.w, tgHitboxRect.h  };
	Thing directionCheck{ &directionCheckRect, Game::ThingType["temp"], -1 };
	int direction = Game::Direction["none"];
	bool actuallyColliding = false;
	bool colliding = Game::checkCollision(Game::things[tgLevelUnit], NULL, tgLevelUnit, true);

	if (colliding)
	{
		for (int i = 0; i < (int)Game::gColliding.size(); i++)
		{
			if (Game::gColliding[i] != NULL)
			{
				directionCheck.tgHitboxRect.h = tgHitboxRect.h - 1;

				// check which direction for each collision
				for (int j = Game::Direction["left"]; j < Game::Direction["total"]; j++)
				{
					if (j == Game::Direction["left"])
						directionCheck.tgHitboxRect.x = tgHitboxRect.x;
					else if (j == Game::Direction["right"])
						directionCheck.tgHitboxRect.x += tgHitboxRect.w;
					if (Game::checkCollision(&directionCheck, Game::things[Game::gColliding[i]->thing2->tgLevelUnit], -1, false))
					{
						if (j == Game::Direction["left"])
							direction = Game::Direction["left"];
						else if (j == Game::Direction["right"])
							direction = Game::Direction["right"];
						break;
					}
					directionCheck.tgHitboxRect.x = tgHitboxRect.x;

					/*
					if (j == Game::Direction["right"])
					{
						directionCheck.tgHitboxRect.x += tgHitboxRect.w;
						direction = Game::Direction["right"];
					}
					else if (j == Game::Direction["left"])
					{
						directionCheck.tgHitboxRect.x = tgHitboxRect.x;
						direction = Game::Direction["left"];
					}
					if (Game::checkCollision(&directionCheck, Game::things[Game::gColliding[i]->thing2->tgLevelUnit], -1, false))
						break;
					directionCheck.tgHitboxRect.x = tgHitboxRect.x;
					
				}

				if(direction != Game::Direction["none"])
					Game::things[Game::gColliding[i]->thing2->tgLevelUnit]->tgResolveCollision(Game::things[tgLevelUnit], direction);
				else
				{
					std::cout << direction << std::endl;
					directionCheck.tgHitboxRect.x = tgHitboxRect.x;
					directionCheck.tgHitboxRect.h = 1;
					directionCheck.tgHitboxRect.w = tgHitboxRect.w;
					for (int j = Game::Direction["up"]; j < Game::Direction["total"]; j++)
					{
						if (j == Game::Direction["up"])
							directionCheck.tgHitboxRect.y = tgHitboxRect.y;
						else if (j == Game::Direction["down"])
							directionCheck.tgHitboxRect.y += tgHitboxRect.h;
						if (Game::checkCollision(&directionCheck, Game::things[Game::gColliding[i]->thing2->tgLevelUnit], -1, false))
						{
							if (j == Game::Direction["up"])
								direction = Game::Direction["up"];
							else if (j == Game::Direction["down"])
								direction = Game::Direction["down"];
							Game::things[Game::gColliding[i]->thing2->tgLevelUnit]->tgResolveCollision(Game::things[tgLevelUnit], direction);
							break;
						}
						directionCheck.tgHitboxRect.y = tgHitboxRect.y;
					}
				}

				actuallyColliding = true;
			}
		}
	}

	if (!colliding || !actuallyColliding)
	{
		if (tgVerticals == 0)
			tgVerticals++;
	}
	*/


	/*
	switch (enType)
	{
	case DUDE:
		if (tgDirection == LEFT)
			tgSpeed = -Game::DEFAULT_SPEED;
		else if (tgDirection == RIGHT)
			tgSpeed = Game::DEFAULT_SPEED;
		else
		{
			tgDirection = RIGHT;
			tgSpeed = Game::DEFAULT_SPEED;
		}

		if (direction != LEFT && tgSpeed < 0)
			tgHitboxRect.x += tgSpeed;
		else if (direction != RIGHT && tgSpeed > 0)
			tgHitboxRect.x += tgSpeed;
		break;
	}
	*/
}

int Enemy::tgGetSubtype(void)
{
	return enType;
}