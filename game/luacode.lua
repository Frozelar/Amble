thingTypes = { "player", "tile", "enemy", "collectible" }

TILE_OFFSET = 10
tileTypes = { "dirtBlock" --[[= TILE_OFFSET + 1]], "dirtWall" }
TOTAL_TILE_TYPES = #tileTypes

ENEMY_OFFSET = 100
enemyTypes = { "dude" --[[= ENEMY_OFFSET + 1]] }
TOTAL_ENEMY_TYPES = #enemyTypes

COLLECTIBLE_OFFSET = 200
collectibleTypes = { "cbit" --[[= COLLECTIBLE_OFFSET + 1]], "cbyte" }
TOTAL_COLLECTIBLE_TYPES = #collectibleTypes

-- {name, # frames or # frame sets}
graphicsIdentifiers = { 
	{ {"Underground", 1} }, -- backgrounds (# frames)
	{ {"DirtBlock", 1}, {"DirtWall", 1} }, -- tiles (# frames (PER tile subidentifier))
	{ {"Player", 5} }, -- player states (# frame SETS (5 by default)
	{ {"Dude", 5} }, -- enemies (# frame SETS (5 by default))
	{ {"Bit", 4}, {"Byte", 4} }, -- collectibles (# frames)
	{ {"Red", 2}, {"Gray", 2}, {"Blue", 2}, {"BigRed", 2}, {"BigGray", 2}, {"BigBlue", 2} } -- particles (# frames)
}

tileSubIdentifiers = {
	"Center", "Top", "TopRight", "Right", "BottomRight", "Bottom", "BottomLeft", "Left", "TopLeft", 
	"Single",
	"SlopeTopLeft", "SlopeTopRight", "SlopeBottomLeft", "SlopeBottomRight"
}
TOTAL_TILE_SUBTYPES = #tileSubIdentifiers

-- {name}
audioIdentifiers = { 
	{ "Boom" }, -- sfx
	{ "Underground" } -- music 
}

-- {name, # frames}
frameIdentifiers = {
	{ {"Idle", 4}, {"WalkLeft", 4}, {"WalkRight", 4}, {"Jump", 2}, {"Fall", 2} }, -- most entities (currently only used with enemies and players)
	{ {"Default", 4} } -- most objects (CURRENTLY UNUSED, CHANGE FRAMES IN graphicsIdentifiers INSTEAD)
}

gravityArray = { 0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8 }
TOTAL_GRAVITY_ARRAY_UNITS = #gravityArray

jumpArray = { 8, 8, 8, 7, 7, 6, 6, 5, 5, 5, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 0 }
TOTAL_JUMP_ARRAY_UNITS = #jumpArray

dashArray = { 16, 12, 8, 6, 4, 2, 2, 1 }
TOTAL_DASH_ARRAY_UNITS = #dashArray

floatArray = { 0, 1, 1, 0, -1, -1, 0 }
TOTAL_FLOAT_ARRAY_UNITS = #floatArray

WINDOW_W = 800
WINDOW_H = 600
DEFAULT_W = 8
DEFAULT_H = 8
PLAYER_W = 8
PLAYER_H = 16
DEFAULT_ENEMY_W = 8
DEFAULT_ENEMY_H = 8
DEFAULT_OFFSET = 2
DEFAULT_SPEED = 2

LEVEL_UNITS = 0

gPlayer = nil
things = {}

points = 0

Rectangle = { x, y, w, h }
function Rectangle:new(px, py, pw, ph)
  setmetatable({}, Rectangle)
  self.x = px
  self.y = py
  self.w = pw
  self.h = ph
  return self
end

function Rectangle:__eq(other)
  self.x = other.x
  self.y = other.y
  self.w = other.w
  self.h = other.h
end

Thing = { tgVerticals, tgSpeed, tgHitbox }
function Thing:new(levelUnit)
  setmetatable({}, Thing)
  self.tgVerticals = 0
  self.tgSpeed = 0
  self.tgLevelUnit = levelUnit
  self.tgHitbox = Rectangle:new(0, 0, 0, 0)
  return self
end

Player = Thing:new()
function Player:new(levelUnit)
  setmetatable({}, Player)
  self.tgVerticals = 0
  self.tgSpeed = 0
  self.tgLevelUnit = levelUnit
  self.plJumps = 0
  self.plDashing = 0
  self.tgHitbox = Rectangle:new(0, 0, 0, 0)
  self.tgHitbox.w = PLAYER_W
  self.tgHitbox.h = PLAYER_H
  print("new playah")
  return self
end

Tile = Thing:new()
function Tile:new(levelUnit)
  setmetatable({}, Tile)
  self.tgVerticals = 0
  self.tgSpeed = 0
  self.tgLevelUnit = levelUnit
  self.tiIsSolid = true
  self.tiSubtype = -1
  self.tgHitbox = Rectangle:new(0, 0, 0, 0)
  self.tgHitbox.w = DEFAULT_W
  self.tgHitbox.h = DEFAULT_H
  return self
end

function Tile:tiHandleAI()
  for i = 1, #things do
    if things[i] ~= nil and things[i].tgType == thingTypes["tile"] then
      if things[i].tiType == tileTypes["dirtBlock"] then
        -- instead of doing this, just include the things that actually need AI??
      elseif things[i].tiType == tileTypes["dirtWall"] then
        
      end
    end
  end
end

Enemy = Thing:new()
function Enemy:new(levelUnit)
  setmetatable({}, Enemy)
  self.tgVerticals = 0
  self.tgSpeed = 0
  self.tgLevelUnit = levelUnit
  self.enPower = 5
  self.enSubtype = -1
  self.enDashing = 0
  self.tgHitbox = Rectangle:new(0, 0, 0, 0)
  self.tgHitbox.w = DEFAULT_ENEMY_W
  self.tgHitbox.h = DEFAULT_ENEMY_H
  return self
end

function Enemy:enHandleAI()
  local checkrect = Rectangle:new(0, 0, 0, 0)
  local iscolliding = false
  
  for i = 1, #things do
    if things[i] ~= nil and things[i].tgType == thingTypes["enemy"] then
      if things[i].enSubtype == enemyTypes["dude"] then
        if things[i].tgVerticals == 0 then
          checkrect = things[i].tgHitbox
          checkrect.w = checkrect.w + DEFAULT_W * 4
          checkrect.x = checkrect.x - DEFAULT_W * 2
          checkrect.h = checkrect.h + DEFAULT_H * 4
          checkrect.y = checkrect.y - DEFAULT_H * 2
          iscolliding = checkCollision(checkrect, gPlayer.tgHitbox)
          if iscolliding == true then
            things[i].tgVerticals = things[i].tgVerticals - 2
            things[i].tgDashing = things[i].tgDashing + 1
          end
        end
        if things[i].tgDashing ~= 0 then
          if things[i].tgDashing < #dashArray then
            if things[i].tgSpeed > 0 then
              things[i].tgHitbox.x = things[i].tgHitbox.x + dashArray[things[i].tgDashing]
            elseif things[i].tgSpeed < 0 then
              things[i].tgHitbox.x = things[i].tgHitbox.x - dashArray[things[i].tgDashing]
            end
          else
            things[i].tgDashing = 0
          end
        end
      end
    end
  end
end

Collectible = Thing:new()
function Collectible:new(levelUnit)
  setmetatable({}, Collectible)
  self.tgVerticals = 0
  self.tgSpeed = 0
  self.tgLevelUnit = levelUnit
  self.clSubtype = -1
  self.tgHitbox = Rectangle:new(0, 0, 0, 0)
  self.tgHitbox.w = DEFAULT_W
  self.tgHitbox.h = DEFAULT_H
  return self
end

function Collectible:clHandleAI()
	self.tgVerticals = self.tgVerticals + 1
	if self.tgVerticals > #floatArray then
		self.tgVerticals = 0
	end
	self.tgHitbox.y = self.tgHitbox.y + floatArray[self.tgVerticals]
end

function Collectible:clCollect()
  for i = 1, #things do
    if things[i] ~= nil and things[i].tgType == thingTypes["collectible"] then
      if things[i].clSubtype == collectibleTypes["cbit"] then
        points = points + 1
      elseif things[i].clSubtype == collectibleTypes["cbyte"] then
        points = points + 5
      end
    end
  end
end

gPlayer = Player:new()

function copythings(v)
  things[#things + 1] = v
end

function init()
  local j = -1
  for i = 1, #things, 1 do
    if things[i] ~= -1 then
      if thingTypes[things[i]] == "player" then
        things[i] = Player:new(i)
      elseif thingTypes[things[i]] == "tile" then
        things[i] = Tile:new(i)
      elseif thingTypes[things[i]] == "enemy" then
        things[i] = Enemy:new(i)
      elseif thingTypes[things[i]] == "collectible" then
        things[i] = Collectible:new(i)
      else
        things[i] = nil
      end
    else
		things[i] = nil
	end
  end
end

--[[
function init()
  local j = -1
  for i = 1, #things, 1 do
    if things[i] ~= -1 then
      if things[i] == thingTypes["player"] then
		print("PLAYER MADE EEEE")
        things[i] = Player:new(i)
      elseif things[i] == thingTypes["tile"] then
        things[i] = Tile:new(i)
      elseif things[i] == thingTypes["enemy"] then
        things[i] = Enemy:new(i)
      elseif things[i] == thingTypes["collectible"] then
        things[i] = Collectible:new(i)
      else
        things[i] = nil
      end
    else
		things[i] = nil
	end
  end
end
]]

--[[
function initIdentifiers()
	dir = "resources/"
	ext = ".png"
	
	for i = 1, #tileTypes do
		table.insert(graphicsIdentifiers, tiles[i], (dir .. tileTypes[i] .. ext))
	end
end
]]

--[[
function handleEnvironment()
  for i = 1, #things do
    if things[i].tgType == thingTypes[enemy]then
      things[i].enHandleAI()
    elseif things[i].tgType == thingTypes[tile] then
      things[i].tiHandleAI()
    end
  
  end
  
end
]]

--[[
function handleLevelLoad(levelUnits)
	for i = 1, levelUnits + 1, 1 do
		--if things[i] == nil then
		things[i] = -1
		print("index->(-1) \n")
		--end
	end
end
]]

--[[
function Thing:tgHandleVerticals()
  if tgVerticals >= #gravityArray then
      tgVerticals = #gravityArray - 2
    elseif (-tgVerticals >= #jumpArray) then
      tgVerticals = 0
    end

    if (tgVerticals < 0) then
      tgHitbox.y = tgHitbox.y - jumpArray[-(tgVerticals)]
      tgVerticals = tgVerticals - 1
    elseif (tgVerticals > 0) then
      tgHitbox.y = tgHitbox.y + jumpArray[tgVerticals]
      tgVerticals = tgVerticals + 1
    end
      
      return tgVerticals
  end
]]



--[[
function Player:tgHandleVerticals()
    	if tgVerticals >= #gravityArray then
      tgVerticals = #gravityArray - 2
    elseif (-tgVerticals >= #jumpArray) then
      tgVerticals = 0
    end

    if (tgVerticals < 0) then
      tgVerticals = tgVerticals - 1
    elseif (tgVerticals > 0) then
      tgVerticals = tgVerticals + 1
    end
      
      
      
      return tgVerticals
  end

  function Player:plMove()
	--Direction dir = NO_DIRECTION;
	colliding = checkCollision(true, gPlayer.tgHitbox)
	nextMove = self.tgHitbox
	moveX = 0, moveY = 0
	left = false, right = false, up = false, down = false

	if (tgVerticals == 0 && plOldVerticals != 0) then
		self:tgHandleVerticals()
		self:plHandleDashing()
		plOldVerticals = tgVerticals
		plOldHitboxRect = tgHitbox
	else
		plOldVerticals = tgVerticals
		plOldHitboxRect = tgHitbox
		self:tgHandleVerticals()
		self:plHandleDashing()
	end

	if (tgSpeed != 0) then
		nextMove = self.tgHitboxRect
		moveX = moveX + self.tgSpeed
		nextMove.x = nextMove.x + tgSpeed;
		if (checkCollision(true, nextMove)) then  
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
				dir = UP;
			}
			else if (left)
			{
				left = false;
				dir = LEFT;
			}
			else if (right)
			{
				right = false;
				dir = RIGHT;
			}
			else if (down)
			{
				down = false;
				dir = DOWN;
			}
			Game::gColliding[i]->thing2->tgResolveCollision(Game::things[tgLevelUnit], dir);
		}
	}
}
]]

--[[
vertFuncs = { 
  function plHandleVerticals(plVerticals)
  	if plVerticals >= #gravityArray then
      plVerticals = #gravityArray - 2
    elseif (-plVerticals >= #jumpArray) then
      plVerticals = 0
    end

    if (plVerticals < 0) then
      plVerticals = plVerticals - 1
    elseif (plVerticals > 0) then
      plVerticals = plVerticals + 1
    end
      
      
      
      return plVerticals
  end,

  function tgHandleVerticals(tgVerticals, tgRect)
  	if tgVerticals >= #gravityArray then
      tgVerticals = #gravityArray - 2
    elseif (-tgVerticals >= #jumpArray) then
      tgVerticals = 0
    end

    if (tgVerticals < 0) then
      tgRect.y = tgRect.y - jumpArray[-(tgVerticals)]
      tgVerticals = tgVerticals - 1
    elseif (tgVerticals > 0) then
      tgRect.y = tgRect.y + jumpArray[tgVerticals]
      tgVerticals = tgVerticals + 1
    end
      
      return tgVerticals
  end
}
]]