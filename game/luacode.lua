getmetatable('').__index = function(str, i)
	return string.sub(str, i, i)
end

thingTypes = { "player", "tile", "enemy", "collectible" }

TILE_OFFSET = 10
tileTypes = { "dirtBlock", "dirtWall" }
TOTAL_TILE_TYPES = #tileTypes

ENEMY_OFFSET = 100
enemyTypes = { "dude" }
TOTAL_ENEMY_TYPES = #enemyTypes

COLLECTIBLE_OFFSET = 200
collectibleTypes = { "cbit", "cbyte" }
TOTAL_COLLECTIBLE_TYPES = #collectibleTypes

-- {name, # frames or # frame sets}
graphicsIdentifiers = { 
	{ {"Underground", 1} }, -- backgrounds (# frames)
	{ {"DirtBlock", 1}, {"DirtWall", 1} }, -- tiles (# frames (PER tile subidentifier))
	{ {"" --[[ "Player" ]], 5} }, -- player states (# frame SETS (5 by default)
	{ {"Dude", 5} }, -- enemies (# frame SETS (5 by default))
	{ {"Bit", 4}, {"Byte", 4} }, -- collectibles (# frames)
	{ {"Red", 2}, {"Gray", 2}, {"Blue", 2}, {"BigRed", 2}, {"BigGray", 2}, {"BigBlue", 2} } -- particles (# frames)
}

tileSubIdentifiers = {
	"Center", "Top", "TopRight", "Right", "BottomRight", "Bottom", "BottomLeft", "Left", "TopLeft", 
	--[[ "Single",
	"SlopeTopLeft", "SlopeTopRight", "SlopeBottomLeft", "SlopeBottomRight" ]]
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

-- backgroundFrames = {}
entityFrames = {}
objectFrames = {}

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

Point = { x = 0, y = 0 }
Point.__index = Point

setmetatable(Point, {
	__call = function(pClass, ...)
		local self = setmetatable({}, pClass)
		self:new(...)
		return self
	end
})

function Point:new(px, py)
	self.x = px
	self.y = py
end

function Point:__eq(other)
	self.x = other.x
	self.y = other.y
end

Rectangle = { x = 0, y = 0, w = 0, h = 0 }
Rectangle.__index = Rectangle

setmetatable(Rectangle, {
	__call = function(pClass, ...)
		local self = setmetatable({}, pClass)
		self:new(...)
		return self
	end
})

function Rectangle:new(px, py, pw, ph)
  self.x = px
  self.y = py
  self.w = pw
  self.h = ph
end

function Rectangle:__eq(other)
  self.x = other.x
  self.y = other.y
  self.w = other.w
  self.h = other.h
end

Background = { bgType, bgRect, bgFrame, bgFrameInterval, bgMaxFrames }
Background.__index = Background

setmetatable(Background, {
	-- __index = Rectangle,
	__call = function(pClass, ...)
		local self = setmetatable({}, pClass)
		Background:new(...)
		return self
	end
})

function Background:new()
	self.bgRect = Rectangle(0, 0, WINDOW_W, WINDOW_H)
	self.bgType = -1
	self.bgFrame = 1
	self.bgFrameInterval = 1
	self.bgMaxFrames = -1
end

--[[
function Background:bgSetType(pType)
	print(pType)
	self.bgType = pType
	self.bgMaxFrames = graphicsIdentifiers[1][self.bgType][2]
end
]]

function bgSetType(pType)
	gBackground.bgType = pType
	gBackground.bgMaxFrames = graphicsIdentifiers[1][gBackground.bgType][2]
end

function Background:bgCycleFrames()
	if self.bgFrameInterval % 4 == 0 then
		self.bgFrame = self.bgFrame + 1
		self.bgFrameInterval = 0
	end
	if self.bgFrame > self.bgMaxFrames then
		self.bgFrame = 1
		self.bgFrameInterval = 0
	end
end

Particle = { ptType, ptRect, ptDestination, ptLife, ptSpeed, ptFrame, ptFrameInterval, ptMaxFrames }
Particle.__index = Particle

setmetatable(Particle, {
	__call = function(pClass, ...)
		local self = setmetatable({}, pClass)
		self:new(...)
		return self
	end
})

function Particle:new(pType, px, py)
	self.ptRect = Rectangle(px, py, 0, 0)
	if string.sub(graphicsIdentifiers[6][pType][1], 1, 3) == "Big" then
		ptRect.w = 2
		ptRect.h = 2
	else
		ptRect.w = 1
		ptRect.h = 1
	end
	
	self.ptType = pType
	self.ptDestination = Point:new(px + math.random(-8, 8), py + math.random(-8, 8))
	self.ptSpeed = math.random(1, 4)
	self.ptLife = ((px - self.ptDestination.x) + (py - self.ptDestination.y)) / ptSpeed
	self.ptFrame = 0
	self.ptFrameInterval = 0
	self.ptMaxFrames = graphicsIdentifiers[6][self.ptType][2]
end

function Particle:ptCycleFrames()
	if self.ptFrameInterval % 4 == 0 then
		self.ptFrame = self.ptFrame + 1
		self.ptFrameInterval = 0
	end
	if self.ptFrame > self.ptMaxFrames then
		self.ptFrame = 1
		self.ptFrameInterval = 0
	end
end

Thing = { tgType, tgVerticals, tgSpeed, tgHitbox, tgLevelUnit, tgFrame, tgFrameInterval, tgMaxFrames }
Thing.__index = Thing

setmetatable(Thing, {
	__call = function(pClass, ...)
		local self = setmetatable({}, pClass)
		self:new(...)
		return self
	end
})

function Thing:new(levelUnit)
  self.tgType = 0
  self.tgVerticals = 0
  self.tgSpeed = 0
  self.tgLevelUnit = levelUnit
  self.tgHitbox = Rectangle(0, 0, 0, 0)
  self.tgFrame = 1
  self.tgFrameInterval = 0
  -- self.tgMaxFrames = 0
end

Player = {plJumps, plDashing}
Player.__index = Player

setmetatable(Player, {
	__index = Thing,
	__call = function(pClass, ...)
		local self = setmetatable({}, pClass)
		self:new(...)
		return self
	end
})

function Player:new(levelUnit)
  --self.tgType = thingTypes["player"]
  self.tgType = "player"
  self.tgVerticals = 0
  self.tgSpeed = 0
  self.tgLevelUnit = levelUnit
  self.plJumps = 0
  self.plDashing = 0
  self.tgHitbox = Rectangle(0, 0, 0, 0)
  self.tgHitbox.w = PLAYER_W
  self.tgHitbox.h = PLAYER_H
end

function Player:plCycleFrames()
	local frameChange = ""
	self.tgFrameInterval = self.tgFrameInterval + 1
	-- if (speed ~= 0 and 4 frames have passed) or (frame == Idle1 and 64 frames have passed) or (thing is idle but is not on the first or last idle frame and 16 frames have passed) then (increment the frame and reset the frame counter)
	if (self.tgSpeed ~= 0 and self.tgFrameInterval % 4 == 0) or 
	(entityFrames[self.tgFrame] == "Idle1" and self.tgFrameInterval % 64 == 0) or
	(string.sub(entityFrames[self.tgFrame], 1, 4) == "Idle" and string.find(entityFrames[self.tgFrame], "1", 5) == nil and string.find(entityFrames[self.tgFrame], "4", 5) == nil and self.tgFrameInterval % 16 == 0 ) then
		self.tgFrame = self.tgFrame + 1
		self.tgFrameInterval = 0
	end
	-- if (thing is jumping) and (frame is not a jumping frame) then (change the frame to a jumping one)
	if (self.tgVerticals < 0) and (string.find(entityFrames[self.tgFrame], "Jump") == nil) then
		frameChange = "Jump1"
	elseif (self.tgVerticals > 0) and (string.find(entityFrames[self.tgFrame], "Fall") == nil) then
		frameChange = "Fall1"
	elseif (self.tgSpeed < 0) and (string.find(entityFrames[self.tgFrame], "WalkLeft") == nil) then
		frameChange = "WalkLeft1"
	elseif(self.tgSpeed > 0) and (string.find(entityFrames[self.tgFrame], "WalkRight") == nil) then
		frameChange = "WalkRight1"
	elseif(self.tgSpeed == 0) and (string.find(entityFrames[self.tgFrame], "Idle") == nil) then
		frameChange = "Idle1"
	end
	
	if frameChange ~= "" then
		self.tgFrame = 1
		while entityFrames[self.tgFrame] ~= frameChange do
			self.tgFrame = self.tgFrame + 1
		end
	end
end

Tile = {tiIsSolid, tiSubtype}
Tile.__index = Tile

setmetatable(Tile, {
	__index = Thing,
	__call = function(pClass, ...)
		local self = setmetatable({}, pClass)
		self:new(...)
		return self
	end
})

function Tile:new(levelUnit)
  --self.tgType = thingTypes["tile"]
  self.tgType = "tile"
  self.tgVerticals = 0
  self.tgSpeed = 0
  self.tgLevelUnit = levelUnit
  self.tiIsSolid = true
  self.tiSubtype = 1																-- NEED TO CHANGE THIS LATERRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR~~~~~~~~~~~~~~~~~~~~~~
  self.tgHitbox = Rectangle(0, 0, 0, 0)
  self.tgHitbox.w = DEFAULT_W
  self.tgHitbox.h = DEFAULT_H
  self.tgMaxFrames = graphicsIdentifiers[2][self.tiSubtype][2]
end

function Tile:tiHandleAI()
  for i = 1, #things do
    if things[i] ~= nil and thingTypes[things[i].tgType] == "tile" then
      if tileTypes[things[i].tiSubtype] == "dirtBlock" then
        -- instead of doing this, just include the things that actually need AI??
      elseif tileTypes[things[i].tiSubtype] == "dirtWall" then
        
      end
    end
  end
end

function Tile:tiCycleFrames()
	if self.tgFrameInterval % 4 == 0 then
		self.tgFrame = self.tgFrame + 1
		self.tgFrameInterval = 0
	end
	if self.tgFrame > self.tgMaxFrames then
		self.tgFrame = 1
		self.tgFrameInterval = 0
	end
end

Enemy = {enPower, enSubtype, enDashing}
Enemy.__index = Enemy

setmetatable(Enemy, {
	__index = Thing,
	__call = function(pClass, ...)
		local self = setmetatable({}, pClass)
		self:new(...)
		return self
	end
})

function Enemy:new(levelUnit)
  --self.tgType = thingTypes["enemy"]
  self.tgType = "enemy"
  self.tgVerticals = 0
  self.tgSpeed = 0
  self.tgLevelUnit = levelUnit
  self.enPower = 5
  self.enSubtype = -1
  self.enDashing = 0
  self.tgHitbox = Rectangle(0, 0, 0, 0)
  self.tgHitbox.w = DEFAULT_ENEMY_W
  self.tgHitbox.h = DEFAULT_ENEMY_H
end

function Enemy:enHandleAI()
  local checkrect = Rectangle(0, 0, 0, 0)
  local iscolliding = false
  
  for i = 1, #things do
    if things[i] ~= nil and thingTypes[things[i].tgType] == "enemy" then
      if enemyTypes[things[i].enSubtype] == "dude" then
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

function Enemy:enCycleFrames()
	local frameChange = ""
	local id = ""
	self.tgFrameInterval = self.tgFrameInterval + 1
	-- if (speed ~= 0 and 4 frames have passed) or (frame == Idle1 and 64 frames have passed) or (thing is idle but is not on the first or last idle frame and 16 frames have passed) then (increment the frame and reset the frame counter)
	if (self.tgSpeed ~= 0 and self.tgFrameInterval % 4 == 0) or 
	(entityFrames[self.tgFrame] == "Idle1" and self.tgFrameInterval % 64 == 0) or
	(string.sub(entityFrames[self.tgFrame], 1, 4) == "Idle" and string.find(entityFrames[self.tgFrame], "1", 5) == nil and string.find(entityFrames[self.tgFrame], "4", 5) == nil and self.tgFrameInterval % 16 == 0 ) then
		self.tgFrame = self.tgFrame + 1
		self.tgFrameInterval = 0
	end
	-- if (thing is jumping) and (frame is not a jumping frame) then (change the frame to a jumping one)
	if (self.tgVerticals < 0) and (string.find(entityFrames[self.tgFrame], "Jump") == nil) then
		frameChange = "Jump1"
	elseif (self.tgVerticals > 0) and (string.find(entityFrames[self.tgFrame], "Fall") == nil) then
		frameChange = "Fall1"
	elseif (self.tgSpeed < 0) and (string.find(entityFrames[self.tgFrame], "WalkLeft") == nil) then
		frameChange = "WalkLeft1"
	elseif(self.tgSpeed > 0) and (string.find(entityFrames[self.tgFrame], "WalkRight") == nil) then
		frameChange = "WalkRight1"
	elseif(self.tgSpeed == 0) and (string.find(entityFrames[self.tgFrame], "Idle") == nil) then
		frameChange = "Idle1"
	end
	if frameChange ~= "" then
		self.tgFrame = 1
		while entityFrames[self.tgFrame] ~= frameChange do
			self.tgFrame = self.tgFrame + 1
		end
	end
	for i = 1, (#entityFrames[self.tgFrame] - 1) do
		id = id .. (entityFrames[self.tgFrame])[i]
	end
	for i = 1, #entityFrames do
		if string.find(entityFrames[self.tgFrame + i], id) == nil then
			self.tgMaxFrames = (entityFrames[self.tgFrame + (i - 1)])[#entityFrames[self.tgFrame + (i - 1)]]
			i = #entityFrames + 1
		end
	end
	if self.tgFrame > self.tgMaxFrames then
		self.tgFrame = 1
		self.tgFrameInterval = 0
	end
end

Collectible = {clSubtype}
Collectible.__index = Collectible

setmetatable(Collectible, {
	__index = Thing,
	__call = function(pClass, ...)
		local self = setmetatable({}, pClass)
		self:new(...)
		return self
	end
})

function Collectible:new(levelUnit)
  self.tgType = "collectible"
  self.tgVerticals = 0
  self.tgSpeed = 0
  self.tgLevelUnit = levelUnit
  self.clSubtype = -1
  self.tgHitbox = Rectangle(0, 0, 0, 0)
  self.tgHitbox.w = DEFAULT_W
  self.tgHitbox.h = DEFAULT_H
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
    if things[i] ~= nil and thingTypes[things[i].tgType] == "collectible" then
      if collectibleTypes[things[i].clSubtype] == "cbit" then
        points = points + 1
      elseif collectibleTypes[things[i].clSubtype] == "cbyte" then
        points = points + 5
      end
    end
  end
end

function Collectible:clCycleFrames()
	self.tgFrameInterval = self.tgFrameInterval + 1
	if self.tgFrameInterval >= 4 then
		self.tgFrame = self.tgFrame + 1
		self.tgFrameInterval = 0
	end
	if self.tgFrame >= #objectFrames then
		self.tgFrame = 0
	end
end

gBackground = Background()
--gPlayer = Player:new()
gParticles = {}
totalParticles = #gParticles

function copythings(v)
  things[#things + 1] = v
end

function initFrames()
	local i = 1
	for j = 1, #frameIdentifiers[1] do
		for k = 1, frameIdentifiers[1][j][2] do
			entityFrames[i] = frameIdentifiers[1][j][1] .. k
			i = i + 1
		end
	end
	i = 1
	for j = 1, #frameIdentifiers[2] do
		for k = 1, frameIdentifiers[2][j][2] do
			objectFrames[i] = --[[ frameIdentifiers[2][j][1] .. ]] k
			i = i + 1
		end
	end
	
	--[[
	for i = 1, #frameIdentifiers[1] do
		for j = 1, frameIdentifiers[1][i][2] do
			entityFrames[i][j] = frameIdentifiers[1][i][1] .. j
		end
	end
	for i = 1, #frameIdentifiers[2] do
		for j = 1, frameIdentifiers[2][i][2] do
			objectFrames[i][j] = j
		end
	end
	]]
end

function init()
  math.randomseed(os.time())
  initFrames()
  print("INITTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT " .. #things)
  local j = -1
  for i = 1, #things do
    if things[i] ~= -1 then
      if thingTypes[things[i]] == "player" then
        things[i] = Player(i)
	  print("MADE A PLAYAH: " .. things[i].tgLevelUnit)
      elseif thingTypes[things[i]] == "tile" then
        things[i] = Tile(i)
	  print("MADE A TILE: " .. things[i].tgLevelUnit)
      elseif thingTypes[things[i]] == "enemy" then
        things[i] = Enemy(i)
      elseif thingTypes[things[i]] == "collectible" then
        things[i] = Collectible(i)
      else
        things[i] = nil
      end
    else
		things[i] = nil
	end
  end
end

function handleEnvironment()
	totalParticles = #gParticles
	for i = 0, #things, 1 do
		if things[i] ~= nil then
			if things[i].tgType == "player" then							-- DOES NOT WORKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK
				things[i]:plCycleFrames()
				
			elseif things[i].tgType == "tile" then
				things[i]:tiCycleFrames()
				things[i]:tiHandleAI()
				
			elseif things[i].tgType == "enemy" then
				things[i]:enCycleFrames()
				things[i]:enHandleAI()
				
			elseif things[i].tgType == "collectible" then
				things[i]:clCycleFrames()
				things[i]:clHandleAI()
				
			end
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