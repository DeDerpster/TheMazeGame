#include "Level.h"

#include <algorithm>
#include <array>
#include <memory>

#include "AStarUtils.h"
#include "Application.h"
#include "Entity.h"
#include "Projectile.h"
#include "Room.h"
#include "Tile.h"

Level::Level(float playerStartX, float playerStartY, int width, int height, Vec2i offsetStart)
	: Layer(),
	  width(width * ROOM_SIZE),
	  height(height * ROOM_SIZE),
	  m_BoardOffset(offsetStart),
	  m_Player(playerStartX, playerStartY, this)
{
	m_Board.resize(width * height, nullptr);   // It is resized, because all positions are used straight away and fills any data slots will nullptr
	Application::getCamera()->setAnchor(&m_Player);
}

Level::~Level()
{
	for(int i = 0; i < m_Board.size(); i++)
	{
		if(m_Board[i])
			delete m_Board[i];
	}

	for(Entity *entity : m_Entities)
		delete entity;

	for(Projectile *projectile : m_Projectiles)
		delete projectile;

	for(Spawner *s : m_Spawners)
		delete s;
}

void Level::render()
{
	int midpoint = getMidPoint();
	Render::orderBuffersByYAxis();
#ifdef DEBUG
	if(renderAll)
	{   // This is to allow the option to render all - however only when debugging - because of the limit with vertices, they must be rendered in blocks
		int c = 0;
		for(int y = 0; y < MAZE_SIZE; y++)
		{
			for(int x = 0; x < MAZE_SIZE; x++)
			{
				Room *room = get(y, x);
				if(room)
				{
					room->render();
					c++;
				}
			}
		}
	}
	else
	{
		Room *mid = getMidRoom();
		mid->render();
		if(mid->isOpen(Direction::north) && get(midpoint + 1, midpoint))
			get(midpoint + 1, midpoint)->render();
		if(mid->isOpen(Direction::south) && get(midpoint - 1, midpoint))
			get(midpoint - 1, midpoint)->render();
		if(mid->isOpen(Direction::east) && get(midpoint, midpoint + 1))
			get(midpoint, midpoint + 1)->render();
		if(mid->isOpen(Direction::west) && get(midpoint, midpoint - 1))
			get(midpoint, midpoint - 1)->render();
	}
#else
	Room *mid = getMidRoom();
	mid->render();
	if(mid->isOpen(Direction::north))
		get(midpoint + 1, midpoint)->render();
	if(mid->isOpen(Direction::south))
		get(midpoint - 1, midpoint)->render();
	if(mid->isOpen(Direction::east))
		get(midpoint, midpoint + 1)->render();
	if(mid->isOpen(Direction::west))
		get(midpoint, midpoint - 1)->render();

#endif
	// Render::render(m_ShaderEffectsIDs);


	for(Entity *entity : m_Entities)
		entity->render();

	for(Projectile *projectile : m_Projectiles)
		projectile->render();

	for(Spawner *s : m_Spawners)
		s->render();

	m_Player.render();
}

void Level::update()
{
	m_Player.update();

	for(auto it = m_Entities.begin(); it != m_Entities.end();)
	{
		(*it)->update();
		if((*it)->deleteMe() || isOutOfBound((*it)->getX(), (*it)->getY()))
		{
			delete *it;
			it = m_Entities.erase(it);
		}
		else
			++it;
	}

	for(auto it = m_Projectiles.begin(); it != m_Projectiles.end();)
	{
		(*it)->update();
		if((*it)->deleteMe() || isOutOfBound((*it)->getX(), (*it)->getY()))
		{
			delete *it;
			it = m_Projectiles.erase(it);
		}
		else
			++it;
	}

	for(auto it = m_Spawners.begin(); it != m_Spawners.end();)
	{
		(*it)->update();
		if((*it)->deleteMe() || isOutOfBound((*it)->getX(), (*it)->getY()))
		{
			delete *it;
			it = m_Spawners.erase(it);
		}
		else
			++it;
	}

	getMidRoom()->update();

	if(m_Player.deleteMe())
		playerDeath();
}

bool Level::eventCallback(const Event::Event &e)
{
	if(e.getType() == Event::EventType::mazeMovedEvent)
	{
		for(Room *room : m_Board)
		{
			if(room)
				room->eventCallback(e);
		}
	}
	else if(getMidRoom()->eventCallback(e))
		return true;

	if(m_Player.eventCallback(e))
		return true;

	for(Projectile *p : m_Projectiles)
	{
		if(p->eventCallback(e))
			return true;
	}

	for(Entity *entity : m_Entities)
	{
		if(entity->eventCallback(e))
			return true;
	}

	for(Spawner *s : m_Spawners)
	{
		if(s->eventCallback(e))
			return true;
	}
	return false;
}

void Level::addRoom(int x, int y, bool entrances[4], RoomType type)
{
	if(x < 0 || x >= MAZE_SIZE || y < 0 || y >= MAZE_SIZE)
	{
		Log::error("Tried to create room out of bounds", LOGINFO);
		return;
	}

	m_Board[coordsToIndex(x, y)] = new Room(getX() + x * ROOM_PIXEL_SIZE, getY() + y * ROOM_PIXEL_SIZE, entrances, type, this);
}

void Level::removeRoom(int y, int x)
{
	if(x < 0 || x >= MAZE_SIZE || y < 0 || y >= MAZE_SIZE)
	{
		Log::error("Tried to delete room out of bounds", LOGINFO);
		return;
	}
	delete m_Board[coordsToIndex(x, y)];
	m_Board[coordsToIndex(x, y)] = nullptr;
}

Room * Level::get(int y, int x)
{
	int index = coordsToIndex(x, y);
	if(index == -1)
		return nullptr;
	return m_Board[coordsToIndex(x, y)];
}

int Level::getMidPoint()
{
	return MAZE_SIZE / 2;
}

Room *Level::getMidRoom()
{
	return get(getMidPoint(), getMidPoint());
}

Tile *Level::getTile(int x, int y)
{
	int roomX = x / ROOM_SIZE - m_BoardOffset.x;
	int tileX = x % ROOM_SIZE;
	int roomY = y / ROOM_SIZE - m_BoardOffset.y;
	int tileY = y % ROOM_SIZE;

	Room *room = get(roomY, roomX);
	if(!room)
	{
		// Log::warning("Trying to access room that doesn't exist!");
		return nullptr;
	}

	return room->getTile(tileX, tileY);
}

Player *Level::getPlayer()
{
	return &m_Player;
}



int Level::coordsToIndex(int x, int y)
{
	if(x < 0 || x >= MAZE_SIZE || y < 0 || y >= MAZE_SIZE)
		return -1;
	int xCoord = x + m_BoardOffset.x;
	int yCoord = y + m_BoardOffset.y;
	if(xCoord >= MAZE_SIZE)
		xCoord -= MAZE_SIZE;
	if(yCoord >= MAZE_SIZE)
		yCoord -= MAZE_SIZE;
	return yCoord * MAZE_SIZE + xCoord;
}



void Level::changeXOffset(int changeBy)
{
	m_BoardOffset.x += changeBy;

	if(m_BoardOffset.x == MAZE_SIZE)
	{
		m_BoardOffset.x = 0;

		Event::MazeMovedEvent e((float) -ROOM_PIXEL_SIZE * MAZE_SIZE, 0.0f);
		Application::callEvent(e, true);
	}
	else if(m_BoardOffset.x == -1)
	{
		m_BoardOffset.x = MAZE_SIZE - 1;
		Event::MazeMovedEvent e((float) ROOM_PIXEL_SIZE * MAZE_SIZE, 0.0f);
		Application::callEvent(e, true);
	}
}

void Level::changeYOffset(int changeBy)
{
	m_BoardOffset.y += changeBy;

	if(m_BoardOffset.y == MAZE_SIZE)
	{
		m_BoardOffset.y = 0;
		Event::MazeMovedEvent e(0.0f, (float) -ROOM_PIXEL_SIZE * MAZE_SIZE);
		Application::callEvent(e, true);
	}
	else if(m_BoardOffset.y == -1)
	{
		m_BoardOffset.y = MAZE_SIZE - 1;
		Event::MazeMovedEvent e(0.0f, (float) ROOM_PIXEL_SIZE * MAZE_SIZE);
		Application::callEvent(e, true);
	}
}

float Level::getX()
{
	return m_BoardOffset.x * ROOM_PIXEL_SIZE;
}

float Level::getY()
{
	return m_BoardOffset.y * ROOM_PIXEL_SIZE;

}

std::vector<Vec2f> *Level::getPath(Vec2f startPos, Vec2f destPos, CollisionBox box)
{
	Vec2f relativeStart = convertToRelativePos(startPos);
	Vec2f relativeDest = convertToRelativePos(destPos);

	std::vector<Vec2f> *path = new std::vector<Vec2f>();

	// A* on rooms
	Vec2i startRoom = {(int) relativeStart.x / ROOM_PIXEL_SIZE, (int) relativeStart.y / ROOM_PIXEL_SIZE};
	Vec2i destRoom  = {(int) relativeDest.x / ROOM_PIXEL_SIZE, (int) relativeDest.y / ROOM_PIXEL_SIZE};

	if(startRoom != destRoom)
	{
		if(!get(startRoom.y, startRoom.x) || !get(destRoom.y, destRoom.x))
		{
			Log::warning("Room that destination or start does not exist!");
			std::vector<Vec2f> *path = new std::vector<Vec2f>();
			path->push_back(destPos);
			return path;
		}

		std::array<Vec2i, 4> offsets;
		offsets[0] = {0, 1};
		offsets[1] = {0, -1};
		offsets[2] = {1, 0};
		offsets[3] = {-1, 0};

		std::function<bool(int, int, int, int, CollisionBox)> collisionDetection = [this](int x, int y, int xs, int ys, CollisionBox box) -> bool {
			if(!get(y + ys, x + xs) /* || !get(y, x)*/)
				return true;
			else if(ys == 1)
				return !get(y, x)->isOpen(Direction::north);
			else if(ys == -1)
				return !get(y, x)->isOpen(Direction::south);
			else if(xs == 1)
				return !get(y, x)->isOpen(Direction::east);
			else if(xs == -1)
				return !get(y, x)->isOpen(Direction::west);
			else
				return false;
		};
		std::function<Vec2f(Vec2i)> convert = [](Vec2i vec) -> Vec2f {
			return {(float) vec.x * ROOM_PIXEL_SIZE, (float) vec.y * ROOM_PIXEL_SIZE};
		};
		std::vector<Vec2f> *myPath = aStarAlgorithm<MAZE_SIZE, MAZE_SIZE, 4>(startRoom, destRoom, box, offsets, collisionDetection, convert, MAZE_SIZE * MAZE_SIZE);

		float mid = ((float) TILE_SIZE * ROOM_SIZE) / 2;
		relativeDest      = {(float) (*myPath)[myPath->size() - 1].x + mid, (float) (*myPath)[myPath->size() - 1].y + mid};

		delete myPath;
	}

	// A* on tiles

	// These are the coordinates to the nearest node (on the whole board)
	Vec2i startNode = {(int) round(relativeStart.x / X_STEP), (int) round(relativeStart.y / Y_STEP)};
	Vec2i destNode  = {(int) round(relativeDest.x / X_STEP), (int) round(relativeDest.y / Y_STEP)};

	if(collisionDetection(destNode.x * X_STEP + getX(), destNode.y * Y_STEP + getY(), box))
	{
		// TODO: Put this in a separate function
		bool foundAlternative = false;

		for(int x = -1; x < 2; x++)
		{
			for(int y = -1; y < 2; y++)
			{
				if(x == 0 && y == 0)
					continue;
				if(!collisionDetection((destNode.x + x) * X_STEP + getX(), (destNode.y + y) * Y_STEP + getY(), box))
				{
					destNode.x += x;
					destNode.y += y;
					foundAlternative = true;
					break;
				}
			}
		}

		if(!foundAlternative)
		{
			Log::warning("Cannot reach destNode!");
			path->push_back(destPos);
			return path;
		}
	}
	if(startNode == destNode)
	{
		path->push_back(destPos);
		return path;
	}

	// These positions are relative to the grid used in the A* algorithm
	Vec2i gridStartPos = {X_MAX / X_STEP, Y_MAX / Y_STEP};
	Vec2i gridDestPos  = {destNode.x - startNode.x + gridStartPos.x, destNode.y - startNode.y + gridStartPos.y};

	std::array<Vec2i, 8> offsets;
	offsets[0] = {-1, 1};
	offsets[1] = {0, 1};
	offsets[2] = {1, 1};
	offsets[3] = {1, 0};
	offsets[4] = {1, -1};
	offsets[5] = {0, -1};
	offsets[6] = {-1, -1};
	offsets[7] = {-1, 0};

	std::function<Vec2f(Vec2i)> convert = [gridStartPos, startNode, this](Vec2i vec) -> Vec2f {
		return {(float) (vec.x - gridStartPos.x + startNode.x) * X_STEP + getX(),
				(float) (vec.y - gridStartPos.y + startNode.y) * Y_STEP + getY()};
	};

	std::function<bool(int, int, int, int, CollisionBox)> collisionDetection = [this, &convert](int x, int y, int xs, int ys, CollisionBox box) -> bool {
		Vec2f pos = convert({x, y});
		return directionalCollision(pos.x, pos.y, xs * X_STEP, ys * Y_STEP, box);
	};

	Vec2f checkDest = {(float) destNode.x * X_STEP + getX(),
					   (float) destNode.y * Y_STEP + getY()};

	if(convert(gridDestPos) != checkDest)
		Log::critical("Convertion does not result in the correct value!", LOGINFO);

	// Log::info("Level A*");

	return aStarAlgorithm<2 * X_MAX / X_STEP, 2 * Y_MAX / Y_STEP, 8>(gridStartPos, gridDestPos, box, offsets, collisionDetection, convert, X_MAX / X_STEP);
}

float Level::convertToRelativeX(float x)
{
	return x - getX();
}

float Level::convertToRelativeY(float y)
{
	return y - getY();

}

Vec2f Level::convertToRelativePos(Vec2f pos)
{
	return {convertToRelativeX(pos.x), convertToRelativeY(pos.y)};
}

bool Level::isOutOfBound(float x, float y)
{
	Vec2f pos = convertToRelativePos({x, y});
	return pos.x < 0 || pos.x > width * TILE_SIZE || pos.y < 0 || pos.y > height * TILE_SIZE;
}

bool Level::collisionPointDetection(float nextX, float nextY)
{
	int   tileX = (int) nextX / TILE_SIZE;
	int   tileY = (int) nextY / TILE_SIZE;
	Tile *tile  = getTile(tileX, tileY);
	if(!tile)
		return true;

	return tile->isSolid();
}

bool Level::collisionTileDetection(int x, int y)
{
	Tile *tile = getTile(x, y);
	if(!tile)
		return true;

	return tile->isSolid();
}

bool Level::collisionDetection(float nextX, float nextY, CollisionBox box)
{
	if(isOutOfBound(nextX + box.lowerBound.x, nextY + box.lowerBound.y) || isOutOfBound(nextX + box.upperBound.x, nextY + box.upperBound.y))
		return true;

	bool lowerLeft  = collisionPointDetection(nextX + box.lowerBound.x, nextY + box.lowerBound.y);
	bool lowerRight = collisionPointDetection(nextX + box.upperBound.x, nextY + box.lowerBound.y);
	bool upperLeft  = collisionPointDetection(nextX + box.lowerBound.x, nextY + box.upperBound.y);
	bool upperRight = collisionPointDetection(nextX + box.upperBound.x, nextY + box.upperBound.y);

	return lowerLeft || lowerRight || upperLeft || upperRight;
}

bool Level::directionalCollision(float x, float y, float xs, float ys, CollisionBox box)
{
	if(xs == 0 && ys == 0)
		return collisionDetection(x, y, box);
	else
	{
		if(isOutOfBound(x + box.lowerBound.x, y + box.lowerBound.y) || isOutOfBound(x + box.upperBound.x, y + box.upperBound.y))
			return true;

		// TODO: Make this look nicer
		if((xs > 0 && ys > 0) || (xs < 0 && ys < 0))   // Travelling along a line close to this: / path
		{
			bool lowerRight = lineCollisionDetection(x + box.upperBound.x, y + box.lowerBound.y, xs, ys);
			bool upperLeft  = lineCollisionDetection(x + box.lowerBound.x, y + box.upperBound.y, xs, ys);

			bool middle;
			if(xs > 0)   // Upper right corner
				middle = lineCollisionDetection(x + box.upperBound.x, y + box.upperBound.y, xs, ys);
			else   // Lower left corner
				middle = lineCollisionDetection(x + box.lowerBound.x, y + box.lowerBound.y, xs, ys);

			// bool middle     = lineCollisionDetection(x + box.lowerBound.x, y + box.lowerBound.y, xs + box.upperBound.x - box.lowerBound.x, ys + box.upperBound.y - box.lowerBound.y);
			return lowerRight || upperLeft || middle;
		}
		else if((xs > 0 && ys < 0) || (xs < 0 && ys > 0))   // Travelling along a line close to this: \ path
		{
			bool lowerLeft  = lineCollisionDetection(x + box.lowerBound.x, y + box.lowerBound.y, xs, ys);
			bool upperRight = lineCollisionDetection(x + box.upperBound.x, y + box.upperBound.y, xs, ys);

			bool middle;
			if(xs > 0)   // Lower right corner
				middle = lineCollisionDetection(x + box.upperBound.x, y + box.lowerBound.y, xs, ys);
			else   // Upper left corner
				middle = lineCollisionDetection(x + box.lowerBound.x, y + box.upperBound.y, xs, ys);

			// bool middle     = lineCollisionDetection(x + box.upperBound.x, y + box.lowerBound.y, xs + box.lowerBound.x - box.upperBound.x, ys + box.upperBound.y - box.lowerBound.y);
			return lowerLeft || upperRight || middle;
		}
		else if(xs != 0)   // Travelling along a line perpendicular to ---
		{
			bool upper, lower;
			if(xs > 0)   // Check right side
			{
				upper = lineCollisionDetection(x + box.upperBound.x, y + box.upperBound.y, xs, ys);
				lower = lineCollisionDetection(x + box.upperBound.x, y + box.lowerBound.y, xs, ys);
			}
			else   // Check left side
			{
				upper = lineCollisionDetection(x + box.lowerBound.x, y + box.upperBound.y, xs, ys);
				lower = lineCollisionDetection(x + box.lowerBound.x, y + box.lowerBound.y, xs, ys);
			}
			return upper || lower;
		}
		else   // Travelling along a line perpendicular to |
		{
			bool left, right;
			if(ys > 0)   // Check upper side
			{
				right = lineCollisionDetection(x + box.upperBound.x, y + box.upperBound.y, xs, ys);
				left  = lineCollisionDetection(x + box.lowerBound.x, y + box.upperBound.y, xs, ys);
			}
			else   // Check lower side
			{
				right = lineCollisionDetection(x + box.upperBound.x, y + box.lowerBound.y, xs, ys);
				left  = lineCollisionDetection(x + box.lowerBound.x, y + box.lowerBound.y, xs, ys);
			}
			return left || right;
		}
	}
}

Entity *Level::entityCollisionDetection(float nextX, float nextY, CollisionBox box)
{
	if(m_Player.hasCollidedWith(nextX, nextY, box))
		return &m_Player;

	for(Entity *e : m_Entities)
	{
		if(e->hasCollidedWith(nextX, nextY, box))
			return e;
	}

	return getMidRoom()->entityCollisionDetection(nextX, nextY, box);
}

bool Level::lineCollisionDetection(float x, float y, float xs, float ys)
{
	bool noStep = collisionPointDetection(x, y);
	if(noStep)
		return true;

	// this is a simple function to return what sign a given float has
	// This is used so as a simple way to determine the direction of travel
	auto getSign = [](float check) -> int {
		if(check > 0)
			return 1;
		else
			return -1;
	};

	auto getPer = [](float relPos, float change) {
		// This calculates the percentage of the way through the tile boundary is hit, and thus the first boundary to be hit
		if(change > 0)
		{
			if(relPos + change > TILE_SIZE)
				return (TILE_SIZE - relPos) / change;
		}
		else
		{   // This means change is negative so it has to check it doesn't go passed 0
			if(relPos + change < 0.0f)
				return relPos / change;
		}
		// Set to 1.1 so that if it doesn't hit another boundary it does not interfere with the comparason
		return 1.1f;
	};

	// This makes the coordinates relative to the current the start of the line is on, making it easier for calculations later
	int   tileX     = (int) x / TILE_SIZE;
	int   tileY     = (int) y / TILE_SIZE;
	float relativeX = x - tileX * TILE_SIZE;
	float relativeY = y - tileY * TILE_SIZE;

	if(relativeX + xs < TILE_SIZE && relativeX + xs > 0 && relativeY + ys < TILE_SIZE && relativeY + ys > 0)
		return noStep;

	int xOffset = 0;
	int yOffset = 0;
	// This continues running until the xs or ys does not travel over a tile boundary
	while(relativeX + xs > TILE_SIZE || relativeY + ys > TILE_SIZE || relativeX + xs < 0 || relativeY + ys < 0)
	{
		float xPer = getPer(relativeX, xs);
		float yPer = getPer(relativeY, ys);

		if(xPer < yPer)   // This just goes through and checks with tile to check
		{
			xOffset += getSign(xs);
			if(collisionTileDetection(tileX + xOffset, tileY + yOffset))
				return true;
			relativeX -= getSign(xs) * TILE_SIZE;
		}
		else if(yPer < xPer)
		{
			yOffset += getSign(ys);
			if(collisionTileDetection(tileX + xOffset, tileY + yOffset))
				return true;
			relativeY -= getSign(ys) * TILE_SIZE;
		}
		else   // This means that it passes through the corner
		{
			if(collisionTileDetection(tileX + xOffset + getSign(xs), tileY + yOffset) || collisionTileDetection(tileX + xOffset, tileY + yOffset + getSign(ys)) || collisionTileDetection(tileX + xOffset + getSign(xs), tileY + xOffset + getSign(ys)))
				return true;

			relativeX -= getSign(xs) * TILE_SIZE;
			relativeY -= getSign(ys) * TILE_SIZE;

			xOffset += getSign(xs);
			yOffset += getSign(ys);
		}
	}

	return false;
}