#include "Enemy.h"

Enemy::Enemy(SDL_Rect* box, int subtype, int unit) : Thing(box, ENEMY, unit)
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

void Enemy::tgResolveCollision(Thing* thing, Direction dir)
{
	if (thing->tgType == PLAYER || thing->tgType == ENEMY)
	{
		if (dir == RIGHT || dir == LEFT)
		{
			thing->tgSpeed *= 0.5;
		}
		else if (thing->tgVerticals != 0)
		{
			thing->tgVerticals = (tgVerticals < 0 ? Game::jumpArray.size() - 25 : Game::gravityArray.size());
		}
		else
			std::cout << "Collision error: " << tgLevelUnit << " " << thing->tgLevelUnit << std::endl;

		if (thing->tgType == PLAYER)
			thing->tgHealth -= enPower;
	}
	/*
	else if (tgType == TILE)
	{
		enIsColliding = true;
		if (dir == RIGHT || dir == LEFT)
		{
			tgDirection = invertDir(tgDirection);
		}
		else if (dir == DOWN)
		{
			if (tgVerticals < 0)
				tgVerticals = 1;
		}
		else if (dir == UP)
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

	++tgFrameWaitCounter;
	if ((tgDirection != NO_DIRECTION && tgFrameWaitCounter % 4 == 0) ||
		(tgFrame == IDLE_1 && tgFrameWaitCounter % 64 == 0) ||
		(tgFrame > IDLE_1 && tgFrame <= IDLE_5 && tgFrameWaitCounter % 16 == 0))
	{
		tgFrame++;
		tgFrameWaitCounter = 0;
	}

	if (tgVerticals < 0 && (tgFrame < JUMP_1 || tgFrame > JUMP_5))
		tgFrame = JUMP_1;
	else if (tgVerticals > 0 && (tgFrame < FALL_1 || tgFrame > FALL_5))
		tgFrame = FALL_1;
	else if (tgSpeed < 0 && tgVerticals == 0 && (tgFrame < WALK_LEFT_1 || tgFrame > WALK_LEFT_5))
		tgFrame = WALK_LEFT_1;
	else if (tgSpeed > 0 && tgVerticals == 0 && (tgFrame < WALK_RIGHT_1 || tgFrame > WALK_RIGHT_5))
		tgFrame = WALK_RIGHT_1;
	else if (tgSpeed == 0 && tgVerticals == 0 && (tgFrame < IDLE_1 || tgFrame > IDLE_5))
		tgFrame = IDLE_1;

	Game::gGraphics.enemyTextures[enType - ENEMY_TYPE_OFFSET][tgFrame].txRect = tgGFXrect;
	Game::gGraphics.enemyTextures[enType - ENEMY_TYPE_OFFSET][tgFrame].txRender();
}

void Enemy::tgApplyAI(void)
{
	// tgHandleVerticals();
	// tgHitboxRect.x += tgSpeed;
	SDL_Rect directionCheckRect{ tgHitboxRect.x, tgHitboxRect.y, 1, 1 /* tgHitboxRect.w, tgHitboxRect.h */ };
	Thing directionCheck{ &directionCheckRect, TEMP, -1 };
	Direction direction = NO_DIRECTION;
	bool actuallyColliding = false;
	bool colliding = Game::checkCollision(Game::things[tgLevelUnit], NULL, tgLevelUnit, true);

	if (colliding)
	{
		for (int i = 0; i < (int)Game::gColliding.size(); i++)
		{
			if (Game::gColliding[i] != NULL)
			{
				directionCheck.tgHitboxRect.h = tgHitboxRect.h;

				// check which direction for each collision
				for (int j = LEFT; j < Game::TOTAL_DIRECTIONS + 1; j++)
				{
					switch (j)
					{
					case RIGHT:
						directionCheck.tgHitboxRect.x += tgHitboxRect.w;
						direction = RIGHT;
						break;
					case LEFT:
						directionCheck.tgHitboxRect.x = tgHitboxRect.x;
						direction = LEFT;
						break;
					}
					if (Game::checkCollision(&directionCheck, Game::things[Game::gColliding[i]->thing2->tgLevelUnit], -1, false))
						break;
					directionCheck.tgHitboxRect.x = tgHitboxRect.x;
				}
				directionCheck.tgHitboxRect.x = tgHitboxRect.x;
				directionCheck.tgHitboxRect.h = 1;
				directionCheck.tgHitboxRect.w = tgHitboxRect.w;
				for (int j = UP; j < Game::TOTAL_DIRECTIONS - 1; j++)
				{
					switch (j)
					{
					case UP:
						directionCheck.tgHitboxRect.y = tgHitboxRect.y;
						break;
					case DOWN:
						directionCheck.tgHitboxRect.y += tgHitboxRect.h;
						break;
					}
					if (Game::checkCollision(&directionCheck, Game::things[Game::gColliding[i]->thing2->tgLevelUnit], -1, false))
					{
						if (j == UP)
							direction = UP;
						else if (j == DOWN)
							direction = DOWN;
						break;
					}
					directionCheck.tgHitboxRect.y = tgHitboxRect.y;
				}

				Game::things[Game::gColliding[i]->thing2->tgLevelUnit]->tgResolveCollision(Game::things[tgLevelUnit], direction);
				actuallyColliding = true;
			}
		}
	}

	if (!colliding || !actuallyColliding)
	{
		if (tgVerticals == 0)
			tgVerticals++;
	}


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