#include "Player.h"

// Texture Graphics::playerTextures[TOTAL_ENTITY_FRAME_TYPES];
Player Game::gPlayer;

Player::Player(SDL_Rect* box, int unit) : Thing(box, Game::ThingType["player"], unit)
{
	tgHitboxRect.w = Game::PLAYER_W;
	tgHitboxRect.h = Game::PLAYER_H;
	tgGFXrect.x = (box == NULL ? Game::DEFAULT_OFFSET : box->x) - Game::DEFAULT_OFFSET;
	tgGFXrect.y = (box == NULL ? Game::DEFAULT_OFFSET : box->y) - Game::DEFAULT_OFFSET;
	tgGFXrect.w = Game::PLAYER_W + Game::DEFAULT_OFFSET * 2;
	tgGFXrect.h = Game::PLAYER_H + Game::DEFAULT_OFFSET * 2;
	tgHitboxRect.x = tgGFXrect.x + Game::DEFAULT_OFFSET;
	tgHitboxRect.y = tgGFXrect.y + Game::DEFAULT_OFFSET;
	plOldHitboxRect = tgHitboxRect;
}

void Player::plHandleEvent(SDL_Event* e)
{
	if (e->type == SDL_KEYUP && e->key.repeat == NULL)
	{
		switch (e->key.keysym.sym)
		{
		case SDLK_w:
			if (-tgVerticals < (int)Game::jumpArray.size() - 8 && tgVerticals < 0)
				tgVerticals = -((int)Game::jumpArray.size() - 8);
			break;
		case SDLK_a:
			if (tgSpeed < 0)
			{
				tgSpeed = 0;
				// tgDirection = NO_DIRECTION;
			}
			break;
		case SDLK_s:
			break;
		case SDLK_d:
			if (tgSpeed > 0)
			{
				tgSpeed = 0;
				// tgDirection = NO_DIRECTION;
			}
			break;
		}
	}
	else if (e->type == SDL_KEYDOWN && e->key.repeat == NULL)
	{
		switch (e->key.keysym.sym)
		{
		case SDLK_w:
			if (tgVerticals == 0 && plOldVerticals == 0 && plDashing == 0)
			{
				plJumps++;
				tgVerticals--;
			}
			else if ((plDashing == -1 || plDashing == 1))
			{
				plDashing += (plDashing < 0 ? -1 : 1);
			}
			else if (plJumps < 3 && (tgVerticals < -((int)Game::jumpArray.size() - 4) || (tgVerticals >= 1 && tgVerticals <= 8)))
			{
				plJumps++;
				tgVerticals = -1;
			}
			break;
		case SDLK_a:
			tgSpeed = -Game::DEFAULT_SPEED;
			// tgDirection = LEFT;
			break;
		case SDLK_s:
			// tgSpeed = DEFAULT_SPEED;
			// tgDirection = DOWN;
			break;
		case SDLK_d:
			tgSpeed = Game::DEFAULT_SPEED;
			// tgDirection = RIGHT;
			break;
		}
	}
}


void Player::tgHandleVerticals(void)
{
	if (tgVerticals >= (int)Game::gravityArray.size() - 1)
		tgVerticals = (int)Game::gravityArray.size() - 2;
	else if (-tgVerticals >= (int)Game::jumpArray.size())
		tgVerticals = 0;

	if (tgVerticals < 0)
		tgVerticals--;
	else if (tgVerticals > 0)
		tgVerticals++;

	/*
	if (tgVerticals > 0)
		tgHitboxRect.y += Game::gravityArray[tgVerticals];
	else if (tgVerticals < 0)
		tgHitboxRect.y += -Game::jumpArray[-tgVerticals];
		*/
		
}



void Player::plHandleDashing(void)
{
	if (tgVerticals == 0 && plOldVerticals > 0 && plDashing == 0 && tgSpeed != 0)
	{
		// plDashing == -1 || plDashing == 1 means the player CAN dash; other values (except 0) mean the player IS dashing
		plDashing += (tgSpeed < 0 ? -1 : 1);
		plActionCounter++;
	}
	else if ((plDashing > 1 || plDashing < -1) && (plDashing < (int)Game::dashArray.size() && plDashing > -(int)Game::dashArray.size()))
	{
		tgHitboxRect.x += (plDashing < 0 ? -Game::dashArray[-plDashing] : Game::dashArray[plDashing]);
		if (plDashing == 2 || plDashing == -2)
			tgHitboxRect.y -= 8;
		plDashing += (plDashing < 0 ? -1 : 1);
	}
	else if ((plDashing >= (int)Game::dashArray.size() || plDashing <= -(int)Game::dashArray.size()))
	{
		plDashing = (tgSpeed < 0 ? -1 : 1);
		plActionCounter = 0;
	}
	else if ((plActionCounter >= 4 && (plDashing == 1 || plDashing == -1)))
	{
		plDashing = 0;
		plActionCounter = 0;
	}
	else if (plDashing == 1 || plDashing == -1)
	{
		plActionCounter++;
	}
}


void Player::plMove(void)
{
	int dir = Game::Direction["none"];
	bool colliding = Game::checkCollision(Game::things[tgLevelUnit], NULL, tgLevelUnit, true);
	Thing nextMove{ &tgHitboxRect, Game::ThingType["temp"], tgLevelUnit };
	int moveX = 0, moveY = 0;
	bool left = false, right = false, up = false, down = false;

	if (tgVerticals == 0 && plOldVerticals != 0)
	{
		tgHandleVerticals();
		plHandleDashing();
		plOldVerticals = tgVerticals;
		plOldHitboxRect = tgHitboxRect;
	}
	else
	{
		plOldVerticals = tgVerticals;
		plOldHitboxRect = tgHitboxRect;
		tgHandleVerticals();
		plHandleDashing();
	}

	if (tgSpeed != 0)
	{
		nextMove.tgHitboxRect = tgHitboxRect;
		moveX += tgSpeed;
		nextMove.tgHitboxRect.x += tgSpeed;
		if (Game::checkCollision(&nextMove, NULL, tgLevelUnit, false))
		{
			moveX -= (tgSpeed < 0 ? -1 : 1);
			nextMove.tgHitboxRect.x -= (tgSpeed < 0 ? -1 : 1);
			while (Game::checkCollision(&nextMove, NULL, tgLevelUnit, false))
			{
				moveX -= (tgSpeed < 0 ? -1 : 1);
				nextMove.tgHitboxRect.x -= (tgSpeed < 0 ? -1 : 1);
			}
			if (tgSpeed > 0)
				right = true;
			else if (tgSpeed < 0)
				left = true;
			tgSpeed = 0;
		}
		tgHitboxRect.x += moveX;
	}
	if (tgVerticals != 0)
	{
		nextMove.tgHitboxRect = tgHitboxRect;
		moveY += (tgVerticals < 0 ? -Game::jumpArray[-tgVerticals] : Game::gravityArray[tgVerticals]);
		nextMove.tgHitboxRect.y += (tgVerticals < 0 ? -Game::jumpArray[-tgVerticals] : Game::gravityArray[tgVerticals]);
		if (Game::checkCollision(&nextMove, NULL, tgLevelUnit, false))
		{
			moveY -= (tgVerticals < 0 ? -1 : 1);
			nextMove.tgHitboxRect.y -= (tgVerticals < 0 ? -1 : 1);
			while (Game::checkCollision(&nextMove, NULL, tgLevelUnit, false))
			{
				moveY -= (tgVerticals < 0 ? -1 : 1);
				nextMove.tgHitboxRect.y -= (tgVerticals < 0 ? -1 : 1);
			}
			if (tgVerticals < 0)
				up = true;
			else if (tgVerticals > 0)
			{
				plJumps = 0;
				down = true;
			}
			tgVerticals = 0;
		}
		tgHitboxRect.y += moveY;
		// if (moveY == (tgVerticals < 0 ? -Game::jumpArray[-tgVerticals] : Game::gravityArray[tgVerticals]))
	}
	if (tgVerticals == 0)
	{
		nextMove.tgHitboxRect = tgHitboxRect;
		nextMove.tgHitboxRect.y++;
		if (!Game::checkCollision(&nextMove, NULL, tgLevelUnit, false))
			tgVerticals++;
	}

	Game::checkCollision(Game::things[tgLevelUnit], NULL, tgLevelUnit, true);
	for (int i = 0; i < Level::LEVEL_UNITS; i++)
	{
		if (Game::gColliding[i] != NULL)
		{
			if (up)
			{
				up = false;
				dir = Game::Direction["up"];
			}
			else if (left)
			{
				left = false;
				dir = Game::Direction["left"];
			}
			else if (right)
			{
				right = false;
				dir = Game::Direction["right"];
			}
			else if (down)
			{
				down = false;
				dir = Game::Direction["down"];
			}
			Game::gColliding[i]->thing2->tgResolveCollision(Game::things[tgLevelUnit], dir);
		}
	}
}

void Player::tgRender(void)
{
	tgSyncTexture();

	/*
	++tgFrameWaitCounter;
	if (((tgSpeed != 0 || tgVerticals != 0) && tgFrameWaitCounter % 4 == 0) ||
		(tgFrame == IDLE_1 && tgFrameWaitCounter % 64 == 0) ||
		(tgFrame > IDLE_1 && tgFrame <= IDLE_4 && tgFrameWaitCounter % 16 == 0))
	{
		tgFrame++;
		tgFrameWaitCounter = 0;
	}

	if (tgVerticals < 0 && (tgFrame < JUMP_1 || tgFrame > JUMP_2))
		tgFrame = JUMP_1;
	else if (tgVerticals > 0 && (tgFrame < FALL_1 || tgFrame > FALL_2))
		tgFrame = FALL_1;
	else if (tgSpeed < 0 && tgVerticals == 0 && (tgFrame < WALK_LEFT_1 || tgFrame > WALK_LEFT_4))
		tgFrame = WALK_LEFT_1;
	else if (tgSpeed > 0 && tgVerticals == 0 && (tgFrame < WALK_RIGHT_1 || tgFrame > WALK_RIGHT_4))
		tgFrame = WALK_RIGHT_1;
	else if (tgSpeed == 0 && tgVerticals == 0 && (tgFrame < IDLE_1 || tgFrame > IDLE_4))
		tgFrame = IDLE_1;
		*/

	// std::cout << "PLAYERTEXTURES SIZE:   " << Graphics::playerTextures[0].size() << std::endl;
	if ((plJumps < 3 && (tgVerticals < -((int)Game::jumpArray.size() - 4) || (tgVerticals >= 1 && tgVerticals <= 8))) || 
		(plDashing == 1 || plDashing == -1))
		Graphics::playerTextures[plState][tgFrame].txColor(210, 180, 90);
	else
		Graphics::playerTextures[plState][tgFrame].txColor(255, 255, 255);

	Graphics::playerTextures[plState][tgFrame].txRect = tgGFXrect; // ????
	Graphics::playerTextures[plState][tgFrame].txRender();
}