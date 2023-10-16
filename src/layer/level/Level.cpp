#include "Level.h"

#include <algorithm>
#include <array>
#include <memory>

#include "AStarUtils.h"
#include "Application.h"
#include "entity/Entity.h"
#include "entity/movableEntity/projectile/Projectile.h"
#include "maze/tile/Tile.h"

#include "event/game/MazeMoved.h"
#include "event/game/MobDied.h"
#include "event/game/ShowAlternatives.h"

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
	// Delets all the rooms, entities, projectiles and spawners
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
	if(mid->isOpen(Direction::north) && get(midpoint + 1, midpoint))
		get(midpoint + 1, midpoint)->render();
	if(mid->isOpen(Direction::south) && get(midpoint - 1, midpoint))
		get(midpoint - 1, midpoint)->render();
	if(mid->isOpen(Direction::east) && get(midpoint, midpoint + 1))
		get(midpoint, midpoint + 1)->render();
	if(mid->isOpen(Direction::west) && get(midpoint, midpoint - 1))
		get(midpoint, midpoint - 1)->render();

#endif
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
	if(m_Player.deleteMe())   // Checks if the player has died
		playerDeath();

	// Updates all the entities and deletes them if they have died
	for(auto it = m_Entities.begin(); it != m_Entities.end();)
	{
		(*it)->update();
		if((*it)->deleteMe() || isOutOfBound((*it)->getX(), (*it)->getY()))
		{
			Mob *mob = dynamic_cast<Mob *>(*it);
			if(mob)
			{   // If it is a mob it will call the mob died
				Application::callEventLater(new Event::MobDiedEvent(mob));
				++it;
			}
			else
			{
				delete *it;
				it = m_Entities.erase(it);
			}
		}
		else
			++it;
	}

	// Goes through the projectiles and updates them and deletes them if needed
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

	// Updates all the spawners
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

	// Updates all the rooms that are visible to the player
	int   midpoint = getMidPoint();
	Room *mid      = getMidRoom();
	mid->update();
	if(mid->isOpen(Direction::north))
		get(midpoint + 1, midpoint)->update();
	if(mid->isOpen(Direction::south))
		get(midpoint - 1, midpoint)->update();
	if(mid->isOpen(Direction::east))
		get(midpoint, midpoint + 1)->update();
	if(mid->isOpen(Direction::west))
		get(midpoint, midpoint - 1)->update();
}

bool Level::eventCallback(const Event::Event &e)
{
	// If it is a maze moved event it calls the event on all the rooms otherwise it only calls the event on the room the player is in
	if(e.getType() == Event::EventType::MazeMoved)
	{
		for(Room *room : m_Board)
		{
			if(room)
				room->eventCallback(e);
		}
	}
	else if(getMidRoom()->eventCallback(e))
		return true;

	// Calls the event on all the other objects in the maze
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

	switch(e.getType())
	{
	case Event::EventType::MobDied:
	{
		// If a mob has died it will be deleted
		const Event::MobDiedEvent &ne = static_cast<const Event::MobDiedEvent &>(e);

		if(ne.mob == &m_Player)
			playerDeath();
		else
		{
			auto index = std::find(m_Entities.begin(), m_Entities.end(), ne.mob);
			if(index != m_Entities.end())
			{
				delete ne.mob;
				m_Entities.erase(index);
			}
		}

		return false;
	}

	case Event::EventType::ShowAltTile:
	{
		// If a room is closing them it move all the entities in the maze to the nearest entrance
		const Event::ShowAltTileEvent &ne = static_cast<const Event::ShowAltTileEvent &>(e);

		if(ne.showAlt)
		{
			auto dirToVec = [](Direction dir) -> Vec2f {
				Vec2f vec;
				switch(dir)
				{
				case Direction::north:
					vec = {(ROOM_SIZE / 2) * TILE_SIZE, (ROOM_SIZE - 2.5f) * TILE_SIZE};
					break;
				case Direction::south:
					vec = {(ROOM_SIZE / 2) * TILE_SIZE, 1.5f * TILE_SIZE};
					break;
				case Direction::east:
					vec = {(ROOM_SIZE - 2.5f) * TILE_SIZE, (ROOM_SIZE / 2) * TILE_SIZE};
					break;
				case Direction::west:
					vec = {1.5f * TILE_SIZE, (ROOM_SIZE / 2) * TILE_SIZE};
					break;
				default:
					vec = {0.0f, 0.0f};
					break;
				}
				return vec;
			};

			auto getChangeBy = [this, dirToVec](Vec2f startPos) -> Vec2f {
				Vec2f relPos = convertToRelativePos(startPos) - getMidPoint() * ROOM_PIXEL_SIZE;

				Direction shortestDir  = Direction::north;
				float     shortestDist = distBetweenVec(relPos, dirToVec(shortestDir));
				for(int dir = Direction::south; dir <= Direction::west; dir++)
				{
					float dist = distBetweenVec(relPos, dirToVec(static_cast<Direction>(dir)));
					if(dist < shortestDist)
					{
						shortestDir  = static_cast<Direction>(dir);
						shortestDist = dist;
					}
				}
				Vec2f pos = dirToVec(shortestDir);
				return pos - relPos;
			};

			Vec2f changeBy = getChangeBy({m_Player.getX(), m_Player.getY()});
			m_Player.changeX(changeBy.x);
			m_Player.changeY(changeBy.y);

			for(Entity *e : m_Entities)
			{
				Vec2f changeBy = getChangeBy({e->getX(), e->getY()});
				e->changeX(changeBy.x);
				e->changeY(changeBy.y);
			}
		}

		return false;
	}

	default:
		return Layer::eventCallback(e);
	}
}

void Level::addRoom(int x, int y, bool entrances[4], Room::Type type)
{
	// Checks if it is out of range
	if(x < 0 || x >= MAZE_SIZE || y < 0 || y >= MAZE_SIZE)
	{
		Log::error("Tried to create room out of bounds", LOGINFO);
		return;
	}

	// Adds room to the board
	m_Board[coordsToIndex(x, y)] = new Room(getX() + x * ROOM_PIXEL_SIZE, getY() + y * ROOM_PIXEL_SIZE, entrances, type, this);
}

void Level::removeRoom(int y, int x)
{
	if(x < 0 || x >= MAZE_SIZE || y < 0 || y >= MAZE_SIZE)
	{
		Log::error("Tried to delete room out of bounds", LOGINFO);
		return;
	}

	// Deletes the room and sets it to a nullptr
	delete m_Board[coordsToIndex(x, y)];
	m_Board[coordsToIndex(x, y)] = nullptr;
}

Room *const Level::get(int y, int x) const
{
	// Returns the room at given coordinates
	int index = coordsToIndex(x, y);
	if(index == -1)   // The function will return -1 if the coordinates are out of range and will log so no need here
		return nullptr;

	return m_Board[index];
}

int const Level::getMidPoint() const
{
	return MAZE_SIZE / 2;   // Returns the mid point of the maze
}

Room *const Level::getMidRoom() const
{
	return get(getMidPoint(), getMidPoint());   // Returns the middle room
}

const Tile *const Level::getTile(int x, int y) const
{
	// Returns tile from tile coordinates
	// Calculates the room coordinates and the tile coordinates relative to the room
	int roomX = x / ROOM_SIZE - m_BoardOffset.x;
	int tileX = x % ROOM_SIZE;
	int roomY = y / ROOM_SIZE - m_BoardOffset.y;
	int tileY = y % ROOM_SIZE;

	Room *room = get(roomY, roomX);
	if(!room)   // If the room doesn't exit it will return a nullptr
		return nullptr;

	return room->getTile(tileX, tileY);
}

Player *const Level::getPlayer()
{
	return &m_Player;
}

int const Level::coordsToIndex(int x, int y) const
{
	// If the coordinates are out of range it will return -1
	if(x < 0 || x >= MAZE_SIZE || y < 0 || y >= MAZE_SIZE)
	{
		// Log::warning("Trying to access room out of range");
		return -1;
	}
	// Adds the offsets to the coordinates
	int xCoord = x + m_BoardOffset.x;
	int yCoord = y + m_BoardOffset.y;

	// If the coordinates have gone over the limit it will loop back to the start
	if(xCoord >= MAZE_SIZE)
		xCoord -= MAZE_SIZE;
	if(yCoord >= MAZE_SIZE)
		yCoord -= MAZE_SIZE;

	// Returns the calculated index
	return yCoord * MAZE_SIZE + xCoord;
}

void Level::changeXOffset(int changeBy)
{
	m_BoardOffset.x += changeBy;

	// If the offset has reached the edge of the maze it will return a maze moved event in the given direction
	if(m_BoardOffset.x == MAZE_SIZE)
	{
		m_BoardOffset.x = 0;

		Event::MazeMovedEvent e((float) -ROOM_PIXEL_SIZE * MAZE_SIZE, 0.0f);
		Application::callEvent(e, Event::CallType::All);
	}
	else if(m_BoardOffset.x == -1)
	{
		m_BoardOffset.x = MAZE_SIZE - 1;
		Event::MazeMovedEvent e((float) ROOM_PIXEL_SIZE * MAZE_SIZE, 0.0f);
		Application::callEvent(e, Event::CallType::All);
	}
}

void Level::changeYOffset(int changeBy)
{
	m_BoardOffset.y += changeBy;

	// If the offset has reached the edge of the maze it will return a maze moved event in the given direction
	if(m_BoardOffset.y == MAZE_SIZE)
	{
		m_BoardOffset.y = 0;
		Event::MazeMovedEvent e(0.0f, (float) -ROOM_PIXEL_SIZE * MAZE_SIZE);
		Application::callEvent(e, Event::CallType::All);
	}
	else if(m_BoardOffset.y == -1)
	{
		m_BoardOffset.y = MAZE_SIZE - 1;
		Event::MazeMovedEvent e(0.0f, (float) ROOM_PIXEL_SIZE * MAZE_SIZE);
		Application::callEvent(e, Event::CallType::All);
	}
}

float const Level::getX() const
{
	return m_BoardOffset.x * ROOM_PIXEL_SIZE;
}

float const Level::getY() const
{
	return m_BoardOffset.y * ROOM_PIXEL_SIZE;
}

std::vector<Vec2f> *Level::getPath(Vec2f startPos, Vec2f destPos, CollisionBox box)
{
	// Makes the coordinates relative to the maze
	Vec2f relativeStart = convertToRelativePos(startPos);
	Vec2f relativeDest  = convertToRelativePos(destPos);

	relativeDest = getNextRoom(relativeStart, relativeDest);
	if(relativeDest.x == -1 && relativeDest.y == -1)
	{
		// std::vector<Vec2f> *path = new std::vector<Vec2f>();
		// path->push_back(destPos);
	}

	return getNodePath(relativeStart, relativeDest, box);
}

Vec2f Level::getNextRoom(Vec2f relativeStart, Vec2f relativeDest)
{
	// A* on rooms
	Vec2i startRoom = {(int) relativeStart.x / ROOM_PIXEL_SIZE, (int) relativeStart.y / ROOM_PIXEL_SIZE};
	Vec2i destRoom  = {(int) relativeDest.x / ROOM_PIXEL_SIZE, (int) relativeDest.y / ROOM_PIXEL_SIZE};

	// If the start and destination are in different rooms it will run the A* on the rooms for efficiency
	if(startRoom != destRoom)
	{
		// Checks both rooms exist
		if(!get(startRoom.y, startRoom.x) || !get(destRoom.y, destRoom.x))
		{
			Log::warning("Room that destination or start does not exist!");
			return {-1, -1};
		}

		// Sets the possible offsets that the algorithm can move in
		std::array<Vec2i, 4> offsets;
		offsets[0] = {0, 1};
		offsets[1] = {0, -1};
		offsets[2] = {1, 0};
		offsets[3] = {-1, 0};

		// Creates a function for collision detection with the rooms
		std::function<bool(int, int, int, int)> collisionDetection = [this](int x, int y, int xs, int ys) -> bool {
			if(!get(y + ys, x + xs) || !get(y, x))   // Checks both the rooms are nullptrs (if they are they cannot move)
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

		// Creates a convertion so the algorithm returns coordinates relative to the level
		std::function<Vec2f(Vec2i)> convert = [](Vec2i vec) -> Vec2f {
			float mid = ((float) TILE_SIZE * ROOM_SIZE) / 2;
			return {(float) vec.x * ROOM_PIXEL_SIZE + mid, (float) vec.y * ROOM_PIXEL_SIZE + mid};
		};

		// Runs the A* algorithm with the custom settings
		std::vector<Vec2f> *myPath = aStarAlgorithm<MAZE_SIZE, MAZE_SIZE, offsets.size()>(startRoom, destRoom, offsets, collisionDetection, convert, MAZE_SIZE * MAZE_SIZE);

		if(myPath->size() == 0)   // Checks the path size is 0 (this should not happen)
			Log::critical("A* Algorithm: algorithm returned a empty path!", LOGINFO);
		else
		{
			// Changes the relative dest so be the next room so the next level of the A* can run
			float mid    = ((float) TILE_SIZE * ROOM_SIZE) / 2;
			relativeDest = (*myPath)[myPath->size() - 1];

			// This changes the room based off the direction of the start (to make sure it is not returning a value of solid tile)
			Vec2f distToStart = relativeStart - relativeDest;
			if(fabs(distToStart.x) > fabs(distToStart.y))
				relativeDest.x += (distToStart.x > 0 ? 1 : -1) * TILE_SIZE;
			else
				relativeDest.y += (distToStart.y > 0 ? 1 : -1) * TILE_SIZE;
		}

		delete myPath;
	}

	return relativeDest;
}

std::vector<Vec2f> *Level::getNodePath(Vec2f relativeStart, Vec2f relativeDest, CollisionBox box)
{
	// These are the coordinates to the nearest node (on the whole board)
	Vec2i startNode = {(int) round(relativeStart.x / X_STEP), (int) round(relativeStart.y / Y_STEP)};
	Vec2i destNode  = {(int) round(relativeDest.x / X_STEP), (int) round(relativeDest.y / Y_STEP)};

	// Checks if the box there is collision detection with the destination
	if(collisionDetection(destNode.x * X_STEP + getX(), destNode.y * Y_STEP + getY(), box))
	{
		// If there is it will look at the different nodes around the destination to see if there are any that do not collide
		bool foundAlternative = false;
		int  maxRange         = (int) round(TILE_SIZE / X_STEP) + 1;

		// This will go through different ranges to find the closest
		for(int range = 1; range < maxRange; range++)
		{
			// Looks as in a square formation around the positions
			for(int x = -1; x < 2; x++)
			{
				for(int y = -1; y < 2; y++)
				{
					if(x == 0 && y == 0)
						continue;
					// Checks to see if that position is in the way
					if(!collisionDetection((destNode.x + x * range) * X_STEP + getX(), (destNode.y + y * range) * Y_STEP + getY(), box))
					{
						// If it is not it will change the destNode position to the one that is not a collision
						destNode.x += x * range;
						destNode.y += y * range;
						relativeDest.x += x * range * X_STEP;
						relativeDest.y += y * range * X_STEP;
						foundAlternative = true;
						break;
					}
				}
				if(foundAlternative)
					break;
			}
			if(foundAlternative)
				break;
		}

		// If it could not find an alternative it will just return the destination
		if(!foundAlternative)
		{
			Log::warning("A* Algorithm: Cannot find alternative for destination!");
			std::vector<Vec2f> *path = new std::vector<Vec2f>();
			path->push_back({relativeDest.x + getX(), relativeDest.y + getY()});
			return path;
		}
	}

	// Checks if the start node is the dest node after this because the new found position might be the start node
	if(startNode == destNode)
	{
		std::vector<Vec2f> *path = new std::vector<Vec2f>();
		path->push_back({relativeDest.x + getX(), relativeDest.y + getY()});
		return path;
	}

	// Gives the coordinates to the grid with the relative destination to check its conversion is okay
	return getGridPath(startNode, destNode, relativeDest, box);
}

std::vector<Vec2f> *Level::getGridPath(Vec2i startNode, Vec2i destNode, Vec2f relativeDest, CollisionBox box)
{
	// These positions are relative to the grid used in the A* algorithm
	Vec2i gridStartPos = {X_MAX / X_STEP, Y_MAX / Y_STEP};
	Vec2i gridDestPos  = {destNode.x - startNode.x + gridStartPos.x, destNode.y - startNode.y + gridStartPos.y};

	// Creates the list of offsets
	std::array<Vec2i, 8> offsets;
	offsets[0] = {-1, 1};
	offsets[1] = {0, 1};
	offsets[2] = {1, 1};
	offsets[3] = {1, 0};
	offsets[4] = {1, -1};
	offsets[5] = {0, -1};
	offsets[6] = {-1, -1};
	offsets[7] = {-1, 0};

	// Creates the conversion function
	std::function<Vec2f(Vec2i)> convert = [gridStartPos, startNode, this](Vec2i vec) -> Vec2f {
		return {(float) (vec.x - gridStartPos.x + startNode.x) * X_STEP + getX(),
				(float) (vec.y - gridStartPos.y + startNode.y) * Y_STEP + getY()};
	};

	// This is for checking the conversion is correct
	Vec2f destPos    = {relativeDest.x + getX(), relativeDest.y + getY()};
	Vec2f errorCheck = convert(gridDestPos) - destPos;
	if(fabs(errorCheck.x) > X_STEP || fabs(errorCheck.y) > Y_STEP)
		Log::error("Convertion does not result in the correct value!", LOGINFO);

	// Creates the collision detection algorithm
	std::function<bool(int, int, int, int)> collisionDetection = [this, &convert, box](int x, int y, int xs, int ys) -> bool {
		Vec2f pos = convert({x, y});
		return directionalCollision(pos.x, pos.y, xs * X_STEP, ys * Y_STEP, box);
	};

	// Runs the A* algorithm
	return aStarAlgorithm<2 * X_MAX / X_STEP, 2 * Y_MAX / Y_STEP, offsets.size()>(gridStartPos, gridDestPos, offsets, collisionDetection, convert, X_MAX / X_STEP);
}

Projectile *Level::getProjInRange(float x, float y, float range)
{
	Projectile *closestProj = nullptr;
	float       closestDist = range;

	// Goes through all  the projectiles in the level
	for(Projectile *proj : m_Projectiles)
	{
		//gets the distance between the projectiles
		float dist = distBetweenVec<Vec2f>({x, y}, {proj->getX(), proj->getY()});
		if(dist < closestDist)
		{   // If it is closer than current projectile it will check its direction
			Direction dir   = proj->getDirection();
			float     distX = proj->getX() - x;
			float     distY = proj->getY() - y;

			// This checks if the projectile is going towards the center (ignoring collisions)
			if(std::fabs(distX) > std::fabs(distY))
			{
				if((distX < 0 && dir == Direction::east) || (distX > 0 && dir == Direction::west))
					closestProj = proj;
			}
			else if(std::fabs(distX) > std::fabs(distY))
			{
				if((distY < 0 && dir == Direction::north) || (distY > 0 && dir == Direction::south))
					closestProj = proj;
			}
			else
				closestProj = proj;
		}
	}

	return closestProj;
}

// Simple conversion functions to convert to relative positions
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

void Level::addEntityToRoom(Entity *e)
{
	// Gets the room coordinates of the entity
	Vec2f relativePos = convertToRelativePos({e->getX(), e->getY()});

	int roomX = (int) (relativePos.x / ROOM_PIXEL_SIZE);
	int roomY = (int) (relativePos.y / ROOM_PIXEL_SIZE);
	// Gets the room and adds the entity into the rooms
	Room *room = get(roomY, roomX);
	if(room)
		room->addEntity(e);
	else
	{
		Log::error("Cannot put entity into room that does not exist!", LOGINFO);
		delete e;
	}
}

Entity *Level::entityCollisionDetection(float nextX, float nextY, CollisionBox box)
{
	// Goes through and checks if its collided with any entities
	if(m_Player.hasCollidedWith(nextX, nextY, box))
		return &m_Player;

	for(Entity *e : m_Entities)
	{
		if(e->hasCollidedWith(nextX, nextY, box))
			return e;
	}

	return getMidRoom()->entityCollisionDetection(nextX, nextY, box);
}

bool Level::collisionPointDetection(float nextX, float nextY)
{
	// Gets the position of tile
	int tileX = (int) nextX / TILE_SIZE;
	int tileY = (int) nextY / TILE_SIZE;

	return collisionTileDetection(tileX, tileY);
}

bool Level::collisionTileDetection(int tileX, int tileY)
{
	const Tile *const tile = getTile(tileX, tileY);
	if(!tile)   // If there is no tile it will return a collision
		return true;

	// Returns whether the tile is solid
	return tile->isSolid();
}

bool Level::collisionDetection(float nextX, float nextY, CollisionBox box)
{
	// Checks none of the points are out-of-bounds
	if(isOutOfBound(nextX + box.lowerBound.x, nextY + box.lowerBound.y) || isOutOfBound(nextX + box.upperBound.x, nextY + box.upperBound.y))
		return true;

	// Checks each corner of the collision box for a collision detection
	bool lowerLeft  = collisionPointDetection(nextX + box.lowerBound.x, nextY + box.lowerBound.y);
	bool lowerRight = collisionPointDetection(nextX + box.upperBound.x, nextY + box.lowerBound.y);
	bool upperLeft  = collisionPointDetection(nextX + box.lowerBound.x, nextY + box.upperBound.y);
	bool upperRight = collisionPointDetection(nextX + box.upperBound.x, nextY + box.upperBound.y);

	// If any of the points have hit something it will return it as a collision
	return lowerLeft || lowerRight || upperLeft || upperRight;
}

bool Level::directionalCollision(float x, float y, float xs, float ys, CollisionBox box)
{
	// Runs the directional collision check algorithm to get whether there has been a collision in the x or y position
	auto [colX, colY] = directionalCollisionCheck(x, y, xs, ys, box);

	// Returns true if any collision has been detected
	return colX || colY;
}

std::tuple<bool, bool> Level::directionalCollisionCheck(float x, float y, float xs, float ys, CollisionBox box)
{
	if(xs == 0 && ys == 0)
	{
		// If there is not movement it will just run the conventional collision detection algorithm
		bool collision = collisionDetection(x, y, box);

		return {collision, collision};
	}
	else
	{
		// Checks if the points are out of bounds
		if(isOutOfBound(x + box.lowerBound.x, y + box.lowerBound.y) || isOutOfBound(x + box.upperBound.x, y + box.upperBound.y))
			return {true, true};

		// Creates a function for the directional collision checks
		auto checks = [this](float x, float y, float xs, float ys, Vec2f(&posOffsets)[3]) -> std::tuple<bool, bool> {
			bool colX = false;
			bool colY = false;

			// Goes through all the offsets
			for(int i = 0; i < 3; i++)
			{
				if(posOffsets[i].x == 0.0f && posOffsets[i].y == 0.0f)
					continue;

				// Runs a line collision check algorithm on each point
				auto [tempX, tempY] = lineCollisionCheck(x + posOffsets[i].x, y + posOffsets[i].y, xs, ys, true);

				// Checks if a collision has been detected
				// NOTE: It does not just overwrite colX or colY because there could be a collision in one and this time it is false, so it would be turned to false
				if(tempX)
					colX = true;
				if(tempY)
					colY = true;
				if(colX && colY)
					break;
			}

			return {colX, colY};
		};

		// Goes through all the different possibilities of moving directions
		if((xs > 0 && ys > 0) || (xs < 0 && ys < 0))   // Travelling along a line close to this: / path
		{
			Vec2f offsets[3] = {
				{box.upperBound.x, box.lowerBound.y},   // Lower Right
				{box.lowerBound.x, box.upperBound.y},   // Upper Left
				{0.0f, 0.0f}                            // Middle
			};

			// Defining the middle
			if(xs > 0)   // Upper right corner
				offsets[2] = {box.upperBound.x, box.upperBound.y};
			else   // Lower left corner
				offsets[2] = {box.lowerBound.x, box.lowerBound.y};

			return checks(x, y, xs, ys, offsets);
		}
		else if((xs > 0 && ys < 0) || (xs < 0 && ys > 0))   // Travelling along a line close to this: \ path
		{
			Vec2f offsets[3] = {
				{box.lowerBound.x, box.lowerBound.y},   // Lower Left
				{box.upperBound.x, box.upperBound.y},   // Upper Right
				{0.0f, 0.0f}                            // Middle
			};

			// Defining the middle
			if(xs > 0)   // Lower right corner
				offsets[2] = {box.upperBound.x, box.lowerBound.y};
			else   // Upper left corner
				offsets[2] = {box.lowerBound.x, box.upperBound.y};

			return checks(x, y, xs, ys, offsets);
		}
		else if(xs != 0)   // Travelling along a line parallel to ---
		{
			Vec2f offsets[3] = {
				{xs > 0 ? box.upperBound.x : box.lowerBound.x, box.upperBound.y},   // Upper
				{xs > 0 ? box.upperBound.x : box.lowerBound.x, box.lowerBound.y},   // Lower
				{0.0f, 0.0f}                                                        // No checks for middle
			};
			return checks(x, y, xs, ys, offsets);
		}
		else   // Travelling along a line parallel to |
		{
			Vec2f offsets[3] = {
				{box.upperBound.x, ys > 0 ? box.upperBound.y : box.lowerBound.y},   // Upper
				{box.lowerBound.x, ys > 0 ? box.upperBound.y : box.lowerBound.y},   // Lower
				{0.0f, 0.0f}                                                        // No checks for middle
			};
			return checks(x, y, xs, ys, offsets);
		}
	}
}

std::tuple<bool, bool> Level::lineCollisionCheck(float x, float y, float xs, float ys, bool returnFirst)
{
	bool noStep = collisionPointDetection(x, y);
	if(noStep || (xs == 0 && ys == 0))
		return {noStep, noStep};

	// this is a simple function to return what sign a given float has
	// This is used so as a simple way to determine the direction of travel
	auto getSign = [](float check) -> int {
		return check > 0 ? 1 : -1;
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

	bool crossedX = false;
	bool crossedY = false;

	// This makes the coordinates relative to the current the start of the line is on, making it easier for calculations later
	int   tileX     = (int) x / TILE_SIZE;
	int   tileY     = (int) y / TILE_SIZE;
	float relativeX = x - tileX * TILE_SIZE;
	float relativeY = y - tileY * TILE_SIZE;

	if(relativeX + xs < TILE_SIZE && relativeX + xs > 0 && relativeY + ys < TILE_SIZE && relativeY + ys > 0)
		return {false, false};

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
			{
				if(returnFirst)
					return {true, false};
				crossedX = true;
			}
			relativeX -= getSign(xs) * TILE_SIZE;
		}
		else if(yPer < xPer)
		{
			yOffset += getSign(ys);
			if(collisionTileDetection(tileX + xOffset, tileY + yOffset))
			{
				if(returnFirst)
					return {false, true};
				crossedY = true;
			}
			relativeY -= getSign(ys) * TILE_SIZE;
		}
		else   // This means that it passes through the corner
		{
			if(collisionTileDetection(tileX + xOffset + getSign(xs), tileY + xOffset + getSign(ys)))
				return {true, true};

			if(collisionTileDetection(tileX + xOffset + getSign(xs), tileY + yOffset))
			{
				if(returnFirst)
					return {true, false};
				crossedX = true;
			}
			if(collisionTileDetection(tileX + xOffset, tileY + yOffset + getSign(ys)))
			{
				if(returnFirst)
					return {false, true};
				crossedY = true;
			}

			relativeX -= getSign(xs) * TILE_SIZE;
			relativeY -= getSign(ys) * TILE_SIZE;

			xOffset += getSign(xs);
			yOffset += getSign(ys);
		}
		if(crossedX && crossedY)
			break;
	}

	return {crossedX, crossedY};
}
