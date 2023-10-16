#include "Maze.h"

#include <ImGui.h>
#include <algorithm>
#include <limits>
#include <math.h>
#include <string>

#include "AStarUtils.h"
#include "Application.h"
#include "Log.h"
#include "RandomGen.h"
#include "VertexBufferLayout.h"

#include "GUIStack.h"

#include "NPC.h"
#include "Room.h"
#include "Tile.h"

#include "WorldItem.h"

#include "FireStaff.h"
#include "Potion.h"

#define LAYER_MAX_FOR_DIRECTIONS 4

#include <vector>

#define MAZE_MIDPOINT ((float) MAZE_SIZE / 2.0f) * ROOM_PIXEL_SIZE - TILE_SIZE / 2

Maze::Maze()
	: Level(MAZE_MIDPOINT, MAZE_MIDPOINT, MAZE_SIZE, MAZE_SIZE, {0, 0}),
	  finishedGenerating(true),
	  moves(0)
{
	for(int i = 0; i < 4; i++)
		isMoving[i] = false;

	// NOTE: Because of how it is rendering the coords (0,0) on the m_Board is the bottom left, not the top left!!

	currentPaths.reserve(2 * MAZE_SIZE);   // The data is reserved here because not all the data is needed, but it could be used and so for efficiency, it is reserved on init

	NPC *follower = new NPC(3100.0f, 3800.0f, this);
	m_Player.addFollower(follower);
	m_Entities.push_back(follower);

	Application::addOverlay(new GUIStack(this));

	Log::info("Maze initialised");
}

Maze::~Maze()
{
	// NOTE: This needs to be caused before the progam ends as it frees up the memory
	Log::info("Maze destroyed");
}

void Maze::update()
{
	if(!finishedGenerating)
		return;

	if(finishedGenerating)
	{
		bool noEntrances = true;
		for(int i = 0; i < MAZE_SIZE; i++)
		{
			if(pathsNorth[i] || pathsSouth[i] || pathsEast[i] || pathsWest[i])
			{
				noEntrances = false;
				break;
			}
		}

		if(noEntrances)
			resetMaze();
	}

	Vec2f playerPos = convertToRelativePos({m_Player.getX(), m_Player.getY()});
	if(playerPos.y > (getMidPoint() + 1) * TILE_SIZE * ROOM_SIZE)
		playerMoved(Direction::north);
	else if(playerPos.y < getMidPoint() * TILE_SIZE * ROOM_SIZE)
		playerMoved(Direction::south);

	if(playerPos.x > (getMidPoint() + 1) * TILE_SIZE * ROOM_SIZE)
		playerMoved(Direction::east);
	else if(playerPos.x < getMidPoint() * TILE_SIZE * ROOM_SIZE)
		playerMoved(Direction::west);

	Level::update();
}

#ifdef DEBUG
void Maze::imGuiRender()
{
	if(ImGui::Button("Reload Maze"))
	{
		// TODO: Change player coords
		generate();
	}
	ImGui::Checkbox("Render all", &renderAll);
	m_Player.imGuiRender();
}
#endif

bool Maze::eventCallback(const Event::Event &e)
{
	if(e.getType() == Event::EventType::keyInput)
	{
		const Event::KeyboardEvent &ne = static_cast<const Event::KeyboardEvent &>(e);
		if(ne.key == GLFW_KEY_E && ne.action == GLFW_PRESS)
		{
			Event::ChangeGUIActiveLayer e(InGameGUILayer::playerInventory);
			Application::callEvent(e, true);

			return true;
		}
		else if(ne.key == GLFW_KEY_ESCAPE && ne.action == GLFW_PRESS)
		{
			Event::ChangeGUIActiveLayer e(InGameGUILayer::exitMenu);
			Application::callEvent(e, true);

			return true;
		}
	}


	return Level::eventCallback(e);
}

void Maze::endLevel()
{
	Event::ChangeGUIActiveLayer e(InGameGUILayer::playerWin);
	Application::callEvent(e, true);
}

void Maze::playerDeath()
{
	Log::info("Player has died");
	m_Player.resetStats();
	m_Player.changeX(3800.0f - m_Player.getX());
	m_Player.changeY(3800.0f - m_Player.getY());
	resetMaze();
}

void Maze::resetMaze()
{
	Log::info("Resetting the maze!");
	for(Entity *e : m_Entities)
		delete e;
	m_Entities.clear();

	for(Projectile *p : m_Projectiles)
		delete p;
	m_Projectiles.clear();

	for(int i = 0; i < MAZE_SIZE * MAZE_SIZE; i++)
	{
		if(m_Board[i])
		{
			delete m_Board[i];
			m_Board[i] = nullptr;
		}
	}

	generate();
}

// SECTION: Rooms
void Maze::addRoom(int x, int y, bool north, bool south, bool east, bool west)
{
	bool entrances[4]          = {north, south, east, west};
	RoomType type                  = RoomType::Empty;

	float graphX  = std::abs(x - MAZE_SIZE / 2) + std::abs(y - MAZE_SIZE / 2) + moves;
	auto  sigmoid = [graphX](float lim, float decay) -> int {
        return round((2 * lim) / (1 + exp(-decay * graphX / 5.0f)) - lim);
	};

	// TODO: Make this based on the distance moved
	int num = Random::getNum(1, 100);
	if(num == sigmoid(1, 0.5f))
		type = RoomType::Exit;
	else if(num < sigmoid(7, 1))
		type = RoomType::Enemy;   // NOTE: This was enemy
	else if(num < sigmoid(12, 1))
		type = RoomType::NPC;
	else if(num < sigmoid(22, 1))
		type = RoomType::Trap;
	else if(num < sigmoid(32, 2))
		type = RoomType::Chest;

	Level::addRoom(x, y, entrances, type);
}

void Maze::playerMoved(Direction dir)
{
	if(isMoving[dir])
	{
		isMoving[dir] = false;

		if(moves >= std::numeric_limits<uint64_t>::max() - 2)
			moves = 0;
		else
			moves += 2;
	}
	else
		isMoving[dir] = true;

	switch(dir)
	{
	case Direction::north:
		Log::info("Player moved north");
		isMoving[Direction::south] = false;
		moveNorth();
		break;
	case Direction::south:
		Log::info("Player moved south");
		isMoving[Direction::north] = false;
		moveSouth();
		break;
	case Direction::east:
		Log::info("Player moved east");
		isMoving[Direction::west] = false;
		moveEast();
		break;
	case Direction::west:
		Log::info("Player moved west");
		isMoving[Direction::east] = false;
		moveWest();
		break;
	default:
		Log::warning("Unknown direction! When generating");
		break;
	}

	getMidRoom()->active();
}

// !SECTION

// SECTION: Generation
void Maze::generatePaths(int layerMax, int startMax)
{
	// Log::info("Generating paths");
	int layer = 0;

	// Resets the paths north
	for(int i = 0; i < MAZE_SIZE; i++)
	{
		pathsNorth[i] = false;
		pathsSouth[i] = false;
		pathsEast[i]  = false;
		pathsWest[i]  = false;
	}

	/*
            This maze generation works using a tree style method, where each branch or path is generated one at a time, and then
            any open entrances that is left is also then added to the currentPaths vector, ready to be generated in the next layer.
            TODO: Add the ability to go back, if not many rooms have been created and look for spaces some can be
        */

	std::vector<Vec2i> newPaths;   // This stores the newPaths for the next layer
	newPaths.reserve(MAZE_SIZE);   // Reserves space because, not all spaces may be used
	while(currentPaths.size() > 0)
	{
		for(int i = 0; i < currentPaths.size(); i++)
		{   // This goes through all the current path options and generates the options for the rooms
			Vec2i pos = currentPaths[i];

			if(get(pos.y, pos.x))   // Checks that the pointer is a nullptr - so it doesn't overwrite any rooms
				continue;

			int prob = startMax;   // This sets the probability of for the chance of generating an entrance
			if(pos.x > (3 * MAZE_SIZE / 4 + 1) || pos.x < MAZE_SIZE / 4 + 1)
			{
				prob++;
			}
			else if(pos.y > (3 * MAZE_SIZE / 4 + 1) || pos.y < MAZE_SIZE / 4 + 1)
			{
				prob++;
			}

			int pathCount = 0;   // Stores the number of paths from that room

			// 0: closed but could be open, 1: open, 2: closed and cannot be open
			EntranceState north = shouldBeOpen(get(pos.y + 1, pos.x), Direction::south, prob, &pathCount);
			EntranceState south = shouldBeOpen(get(pos.y - 1, pos.x), Direction::north, prob, &pathCount);
			EntranceState east  = shouldBeOpen(get(pos.y, pos.x + 1), Direction::west, prob, &pathCount);
			EntranceState west  = shouldBeOpen(get(pos.y, pos.x - 1), Direction::east, prob, &pathCount);

			// To increase the spread for the beginning part of generation, if only one entrance has been generated -
			// then it will force another entrance if the layer is still below the layerMax
			if(pathCount <= 1 && layer < layerMax)
			{
				forceEntrance(&north, &south, &east, &west);

				int r = Random::getNum(0, 2);
				if(r != 2)
				{
					forceEntrance(&north, &south, &east, &west);
				}
			}
			else if(pathCount == 2 && layer < layerMax - MAZE_SIZE / 3)
			{
				int r = Random::getNum(0, 2);
				if(r != 2)
				{
					forceEntrance(&north, &south, &east, &west);
				}
			}

			// This is to check if any errors have occurred when generating the maze
			if(north == EntranceState::isOpen && pos.y < MAZE_SIZE - 1 && get(pos.y + 1, pos.x) && !get(pos.y + 1, pos.x)->isOpen(Direction::south))
				Log::error("Room generated incorrectly!", LOGINFO);
			if(south == EntranceState::isOpen && pos.y > 0 && get(pos.y - 1, pos.x) && !get(pos.y - 1, pos.x)->isOpen(Direction::north))
				Log::error("Room generated incorrectly!", LOGINFO);
			if(east == EntranceState::isOpen && pos.x < MAZE_SIZE - 1 && get(pos.y, pos.x + 1) && !get(pos.y, pos.x + 1)->isOpen(Direction::west))
				Log::error("Room generated incorrectly!", LOGINFO);
			if(west == EntranceState::isOpen && pos.x > 0 && get(pos.y, pos.x - 1) && !get(pos.y, pos.x - 1)->isOpen(Direction::east))
				Log::error("Room generated incorrectly!", LOGINFO);

			// This adds any new paths made to the newPaths list
			if(north == EntranceState::isOpen && pos.y < MAZE_SIZE - 1 && !get(pos.y + 1, pos.x))
				newPaths.push_back({pos.x, pos.y + 1});
			if(south == EntranceState::isOpen && pos.y > 0 && !get(pos.y - 1, pos.x))
				newPaths.push_back({pos.x, pos.y - 1});
			if(east == EntranceState::isOpen && pos.x < MAZE_SIZE - 1 && !get(pos.y, pos.x + 1))
				newPaths.push_back({pos.x + 1, pos.y});
			if(west == EntranceState::isOpen && pos.x > 0 && !get(pos.y, pos.x - 1))
				newPaths.push_back({pos.x - 1, pos.y});

			addRoom(pos.x, pos.y,
					north == EntranceState::isOpen,
					south == EntranceState::isOpen,
					east == EntranceState::isOpen,
					west == EntranceState::isOpen);
		}
		currentPaths = newPaths;
		newPaths.clear();

		layer++;
	}

	updatePaths();

	finishedGenerating = true;
}

void Maze::multithreadGenerating(int layerMax, int startMax)
{
	if(!finishedGenerating)
		Log::critical("Stacked maze generating!!", LOGINFO);
	finishedGenerating = false;
	std::thread t1(&Maze::generatePaths, this, layerMax, startMax);   // This starts the multithreading
	t1.detach();
}

void Maze::generate()
{
	for(int i = 0; i < MAZE_SIZE * MAZE_SIZE; i++)
	{
		if(m_Board[i])
		{
			delete m_Board[i];   // NOTE: This frees up the memory, but does not make it a nullptr
			m_Board[i] = nullptr;
		}
	}
	m_BoardOffset.y = 0;
	m_BoardOffset.x = 0;
	int midpoint    = MAZE_SIZE / 2;
	// NOTE: MUST DELETE ALL ROOMS!

	bool entrances[4] = {true, true, true, true};
	Level::addRoom(midpoint, midpoint, entrances, RoomType::Empty);
	currentPaths.push_back({midpoint - 1, midpoint});
	currentPaths.push_back({midpoint, midpoint - 1});
	currentPaths.push_back({midpoint + 1, midpoint});
	currentPaths.push_back({midpoint, midpoint + 1});

	Item *     item      = new FireStaff();
	WorldItem *worldItem = new WorldItem(3800.0f, 3800.0f, TILE_SIZE / 2, this, item);
	getMidRoom()->addEntity(worldItem);
	Item *     item2      = new FireStaff();
	WorldItem *worldItem2 = new WorldItem(3900.0f, 3800.0f, TILE_SIZE / 2, this, item2);
	getMidRoom()->addEntity(worldItem2);

	auto healthPotionFunc = [](Mob *mob) {
		mob->changeHealth(100);
	};

	Item *     potion     = new Potion("Large Health Potion", POTION_HEALTH, healthPotionFunc);
	WorldItem *worldItem3 = new WorldItem(3800.0f, 3900.0f, TILE_SIZE / 2, this, potion);
	getMidRoom()->addEntity(worldItem3);

	multithreadGenerating(MAZE_SIZE * 4 / 5, 1);
	// TODO: Add check to see if okay maze
}

void Maze::moveNorth()
{
	for(int i = 0; i < MAZE_SIZE; i++)
	{
		// NOTE: The coordinates given is the top layer
		if(pathsNorth[i])
			currentPaths.push_back({i, MAZE_SIZE - 1});

		// This deletes any room that is being forgotten
		// NOTE it is 0 because it is getting rid of the bottom layer - which will become the top layer
		removeRoom(0, i);
	}
	changeYOffset(1);             // Changes the y offset because of the new layout
	if(currentPaths.size() > 0)   // This checks if there is any point to generate new paths
		multithreadGenerating(LAYER_MAX_FOR_DIRECTIONS, 1);
	else
		updatePaths();
}

void Maze::moveSouth()
{   // These all do the same as moveNorth but just specialized for the direction
	for(int i = 0; i < MAZE_SIZE; i++)
	{
		if(pathsSouth[i])
			currentPaths.push_back({i, 0});

		removeRoom(MAZE_SIZE - 1, i);
	}
	changeYOffset(-1);
	if(currentPaths.size() > 0)
		multithreadGenerating(LAYER_MAX_FOR_DIRECTIONS, 1);
	else
		updatePaths();
}

void Maze::moveEast()
{
	for(int i = 0; i < MAZE_SIZE; i++)
	{
		if(pathsEast[i])
			currentPaths.push_back({MAZE_SIZE - 1, i});

		removeRoom(i, 0);
	}
	changeXOffset(1);
	if(currentPaths.size() > 0)
		multithreadGenerating(LAYER_MAX_FOR_DIRECTIONS, 1);
	else
		updatePaths();
}

void Maze::moveWest()
{
	for(int i = 0; i < MAZE_SIZE; i++)
	{
		if(pathsWest[i])
			currentPaths.push_back({0, i});

		removeRoom(i, MAZE_SIZE - 1);
	}
	changeXOffset(-1);
	if(currentPaths.size() > 0)
		multithreadGenerating(LAYER_MAX_FOR_DIRECTIONS, 1);
	else
		updatePaths();
}

Maze::EntranceState Maze::shouldBeOpen(Room *room, Direction nextEntrance, int prob, int *pathCount)
{
	if(room)
	{
		if(room->isOpen(nextEntrance))
		{   // If the opposite entrance is open then it needs to have the entrance open
			(*pathCount)++;
			return EntranceState::isOpen;
		}
		else   // If it is closed then there cannot be an entrance
			return EntranceState::isClosed;
	}
	else
	{   // Randomly generates whether the entrance will be open
		int r = Random::getNum(0, prob);
		if(r == 0)
		{
			(*pathCount)++;
			return EntranceState::isOpen;
		}
		return EntranceState::couldOpen;
	}
}

void Maze::forceEntrance(Maze::EntranceState *north, Maze::EntranceState *south, Maze::EntranceState *east, Maze::EntranceState *west)
{
	std::vector<EntranceState *> entrances;   // This will store the pointers to the entrance values
	entrances.reserve(3);

	// This checks if another entrance can be made - if so it adds it to the list
	if(!(*north))
		entrances.push_back(north);
	if(!(*south))
		entrances.push_back(south);
	if(!(*east))
		entrances.push_back(east);
	if(!(*west))
		entrances.push_back(west);

	if(entrances.size() == 1)   // This just makes sure that random
	{
		*entrances[0] = EntranceState::isOpen;
	}
	else if(entrances.size() > 0)
	{
		int r         = Random::getNum(0, entrances.size() - 1);
		*entrances[r] = EntranceState::isOpen;
	}
}

void Maze::updatePaths()
{
	/* NOTE: Probably don't even try to change where this is calculated! This must go here, because if it doesn't you would have to do something special with the paths variables or realise that you actually have to check at
        some point if the player is stuck.*/

	for(int i = 0; i < MAZE_SIZE; i++)
	{
		if(get(MAZE_SIZE - 1, i) && get(MAZE_SIZE - 1, i)->isOpen(Direction::north))
			pathsNorth[i] = true;
		if(get(0, i) && get(0, i)->isOpen(Direction::south))
			pathsSouth[i] = true;
		if(get(i, MAZE_SIZE - 1) && get(i, MAZE_SIZE - 1)->isOpen(Direction::east))
			pathsEast[i] = true;
		if(get(i, 0) && get(i, 0)->isOpen(Direction::west))
			pathsWest[i] = true;
	}
}

// !SECTION