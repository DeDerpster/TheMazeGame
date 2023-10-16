#include "Maze.h"

#include "ImGui.h"
#include <algorithm>
#include <string>

#include "Application.h"
#include "Log.h"
#include "RandomGen.h"
#include "VertexBufferLayout.h"

#include "EmptyRoom.h"
#include "Enemy.h"
#include "Follower.h"
#include "Tile.h"

#include "WorldItem.h"

#include "FireStaff.h"

#define LAYER_MAX_FOR_DIRECTIONS 4

Maze::Maze()
	: Level(BOARD_SIZE * ROOM_SIZE, BOARD_SIZE * ROOM_SIZE), m_Player(4500.0f, 4500.0f, this)
{
	// NOTE: Because of how it is rendering the coords (0,0) on the board is the bottom left, not the top left!!

	board.resize(BOARD_SIZE * BOARD_SIZE, nullptr);   // It is resized, because all positions are used straight away and fills any data slots will nullptr
	currentPaths.reserve(2 * BOARD_SIZE);             // The data is reserved here because not all the data is needed, but it could be used and so for efficiency, it is reserved on init

	m_Shader = std::make_unique<Shader>("res/shaders/BasicShader.glsl");

	auto bufferInit = [](VertexBufferLayout &layout) {
		layout.push<float>(2);
		layout.push<float>(2);
		layout.push<float>(1);
	};

	m_Buffer = std::make_unique<Render::SmartBuffer>(3528, bufferInit);

	m_Shader->bind();

	int samplers[32];
	for(int i = 0; i < 32; i++)
		samplers[i] = i;
	m_Shader->setUniform1iv("u_Textures", 32, samplers);

	m_Shader->setUniform4f("u_Zoom", 1.0f, 1.0f, 1.0f, 1.0f);

	Application::getCamera()->setAnchor(&m_Player);

	Enemy *enemy = new Enemy(3800.0f, 4500.0f, this);
	// follower->setFollower(&m_Player);
	m_Entities.push_back(enemy);

	Item *     item      = new FireStaff();
	WorldItem *worldItem = new WorldItem(3800.0f, 3800.0f, this, item);
	m_Entities.push_back(worldItem);

	Log::info("Maze initialised");
}

Maze::~Maze()
{
	// NOTE: This needs to be caused before the progam ends as it frees up the memory
	for(int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
	{
		if(board[i])
			delete board[i];
	}
	// for(Entity *entity : m_Entities)
	// delete entity;
	Log::info("Maze destroyed");
}

// Level overrides
void Maze::render()
{
	m_Shader->bind();
	float multiple = ROOM_SIZE * Tile::TILE_SIZE;
	int   midpoint = BOARD_SIZE / 2 + 1;
#ifdef DEBUG
	if(renderAll)
	{   // This is to allow the option to render all - however only when debugging - because of the limit with vertices, they must be rendered in blocks
		int c = 0;
		for(int i = 0; i < BOARD_SIZE; i++)
		{
			for(int j = 0; j < BOARD_SIZE; j++)
			{
				Room *room = get(i, j);
				if(room)
				{
					room->render(j * multiple, i * multiple);
					c++;
				}
			}
		}
	}
	else
	{
		Room *mid = get(midpoint, midpoint);
		mid->render(midpoint * multiple, midpoint * multiple);
		if(mid->isOpen(0) && get(midpoint + 1, midpoint))
			get(midpoint + 1, midpoint)->render(midpoint * multiple, (midpoint + 1) * multiple);
		if(mid->isOpen(1) && get(midpoint - 1, midpoint))
			get(midpoint - 1, midpoint)->render(midpoint * multiple, (midpoint - 1) * multiple);
		if(mid->isOpen(2) && get(midpoint, midpoint + 1))
			get(midpoint, midpoint + 1)->render((midpoint + 1) * multiple, midpoint * multiple);
		if(mid->isOpen(3) && get(midpoint, midpoint - 1))
			get(midpoint, midpoint - 1)->render((midpoint - 1) * multiple, midpoint * multiple);
	}
#else
	Room *mid = get(midpoint, midpoint);
	mid->render(midpoint * multiple, midpoint * multiple);
	if(mid->isOpen(0))
		get(midpoint + 1, midpoint)->render(midpoint * multiple, (midpoint + 1) * multiple);
	if(mid->isOpen(1))
		get(midpoint - 1, midpoint)->render(midpoint * multiple, (midpoint - 1) * multiple);
	if(mid->isOpen(2))
		get(midpoint, midpoint + 1)->render((midpoint + 1) * multiple, midpoint * multiple);
	if(mid->isOpen(3))
		get(midpoint, midpoint - 1)->render((midpoint - 1) * multiple, midpoint * multiple);

#endif
	Render::spriteRender(*m_Buffer);
	for(Entity *entity : m_Entities)
	{
		if(Application::getCamera()->isInFrame(entity->getX(), entity->getY()))
			entity->render();
	}
	Render::spriteRender(*m_Buffer);
	m_Player.render();
	for(Projectile *projectile : m_Projectiles)
	{
		if(Application::getCamera()->isInFrame(projectile->getX(), projectile->getY()))
			projectile->render();
	}
	Render::spriteRender(*m_Buffer);
}

void Maze::update()
{
	bool  movedPlayer = false;
	float changeXBy   = 0;
	float changeYBy   = 0;
	if(m_Player.getY() > ((BOARD_SIZE / 2) + 2) * Tile::TILE_SIZE * ROOM_SIZE)
	{
		Log::info("Player moved North");
		// changeYBy = -Tile::TILE_SIZE * ROOM_SIZE;
		Application::MazeMovedEvent e(0.0f, (float) -Tile::TILE_SIZE * ROOM_SIZE);
		Application::callEvent(e, true);
		// m_Player.changeY(changeYBy);
		moveNorth();
		// movedPlayer = true;
	}
	else if(m_Player.getY() < ((BOARD_SIZE / 2) + 1) * Tile::TILE_SIZE * ROOM_SIZE)
	{
		Log::info("Player moved South");

		Application::MazeMovedEvent e(0.0f, (float) Tile::TILE_SIZE * ROOM_SIZE);
		Application::callEvent(e, true);
		// changeYBy = Tile::TILE_SIZE * ROOM_SIZE;
		// m_Player.changeY(changeYBy);
		moveSouth();
		// movedPlayer = true;
	}
	if(m_Player.getX() > ((BOARD_SIZE / 2) + 2) * Tile::TILE_SIZE * ROOM_SIZE)
	{
		Log::info("Player moved East");

		Application::MazeMovedEvent e((float) -Tile::TILE_SIZE * ROOM_SIZE, 0.0f);
		Application::callEvent(e, true);
		// changeXBy = -Tile::TILE_SIZE * ROOM_SIZE;
		// m_Player.changeX(changeXBy);
		moveEast();
		// movedPlayer = true;
	}
	else if(m_Player.getX() < ((BOARD_SIZE / 2) + 1) * Tile::TILE_SIZE * ROOM_SIZE)
	{
		Log::info("Player moved West");
		Application::MazeMovedEvent e((float) Tile::TILE_SIZE * ROOM_SIZE, 0.0f);
		Application::callEvent(e, true);
		// changeXBy = Tile::TILE_SIZE * ROOM_SIZE;
		// m_Player.changeX(changeXBy);
		moveWest();
		// movedPlayer = true;
	}

	m_Player.update();

	// if(movedPlayer)
	// {
	// 	Application::getCamera()->changeUpdateView();
	// }

	for(auto it = m_Entities.begin(); it != m_Entities.end();)
	{
		(*it)->update();
		if((*it)->deleteMe() || (*it)->getX() < 0 || (*it)->getX() > width * Tile::TILE_SIZE || (*it)->getY() < 0 || (*it)->getY() > height * Tile::TILE_SIZE)
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
		if((*it)->deleteMe() || (*it)->getX() < 0 || (*it)->getX() > width * Tile::TILE_SIZE || (*it)->getY() < 0 || (*it)->getY() > height * Tile::TILE_SIZE)
		{
			delete *it;
			it = m_Projectiles.erase(it);
		}
		else
			++it;
	}

	if(finishedGenerating)
	{
		bool noEntrances = true;
		for(int i = 0; i < BOARD_SIZE; i++)
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

	int midpoint = BOARD_SIZE / 2 + 1;
	for(int x = midpoint - 1; x < midpoint + 2; x++)
	{
		for(int y = midpoint - 1; y < midpoint + 2; y++)
		{
			if(get(y, x))
				get(y, x)->update();
		}
	}

	if(m_Player.deleteMe())
		playerDeath();
}

#ifdef DEBUG
void Maze::imGuiRender()
{
	if(ImGui::Button("Reload Maze"))
	{
		generate();
	}
	ImGui::Checkbox("Render all", &renderAll);
	m_Player.imGuiRender();
}
#endif

bool Maze::eventCallback(const Application::Event &e)
{
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
	return false;
}

void Maze::playerDeath()
{
	Log::info("Player has died");
	m_Player.resetStats();
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

	for(int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
	{
		if(board[i])
		{
			delete board[i];
			board[i] = nullptr;
		}
	}

	generate();
}

// SECTION: Rooms
int Maze::coordsToIndex(int x, int y)
{
	if(x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE)
	{
		Log::error("Tried to get index out of bounds", LOGINFO);
		return -1;
	}
	int xCoord = x + xoffset;
	int yCoord = y + yoffset;
	if(xCoord >= BOARD_SIZE)
		xCoord -= BOARD_SIZE;
	if(yCoord >= BOARD_SIZE)
		yCoord -= BOARD_SIZE;
	return yCoord * BOARD_SIZE + xCoord;
}

void Maze::addRoom(int x, int y, bool north, bool south, bool east, bool west, bool isInSubThread)
{
	// TODO: Add more randomization for the different types of rooms as well as entities and objects
	bool entrances[4]          = {north, south, east, west};
	board[coordsToIndex(x, y)] = new EmptyRoom(entrances);
}

void Maze::removeRoom(int y, int x)
{
	// TODO: take any entities out of the room!
	if(x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE)
	{
		Log::error("Tried to set room out of bounds", LOGINFO);
		return;
	}
	delete board[coordsToIndex(x, y)];
	board[coordsToIndex(x, y)] = nullptr;
}
std::vector<Vec2f> *Maze::getPath(Vec2f startPos, Vec2f dest, CollisionBox collisionBox)
{
	if(collisionDetection(dest.x, dest.y, collisionBox))
	{
		std::vector<Vec2f> *path = new std::vector<Vec2f>();
		path->push_back(dest);
		return path;
	}

	if(startPos.x / ROOM_SIZE != dest.x / ROOM_SIZE || startPos.y / ROOM_SIZE != dest.y / ROOM_SIZE)
	{   // TODO: Make this check if rooms are used in the level
		std::vector<Vec2i> openList;

		// NOTE: This is hard coded, might need to change this
		bool closedList[BOARD_SIZE][BOARD_SIZE];
		memset(closedList, false, sizeof(closedList));

		std::array<std::array<Node, BOARD_SIZE>, BOARD_SIZE> nodeMap;

		Vec2i startVec = {(int) startPos.x / (ROOM_SIZE * Tile::TILE_SIZE), (int) startPos.y / (ROOM_SIZE * Tile::TILE_SIZE)};
		Vec2i destVec  = {(int) dest.x / (ROOM_SIZE * Tile::TILE_SIZE), (int) dest.y / (ROOM_SIZE * Tile::TILE_SIZE)};

		{
			float hCost                            = distBetweenVec2i(startVec, destVec);
			nodeMap[startVec.x][startVec.y].vec    = startVec;
			nodeMap[startVec.x][startVec.y].parent = {-1, -1};
			nodeMap[startVec.x][startVec.y].fCost  = hCost;
			nodeMap[startVec.x][startVec.y].gCost  = 0.0f;
			nodeMap[startVec.x][startVec.y].hCost  = hCost;

			openList.push_back(startVec);
		}

		while(openList.back() != destVec)
		{
			Vec2i currentPos  = openList.back();
			Node *currentNode = &nodeMap[currentPos.x][currentPos.y];
			openList.pop_back();
			closedList[currentPos.x][currentPos.y] = true;
			for(int i = 0; i < 4; i++)
			{
				if(!get(currentPos.y, currentPos.x) || !get(currentPos.y, currentPos.x)->isOpen(i))
					continue;

				Vec2i dirVec = {0, 0};
				if(i == Direction::north)
					dirVec.y = 1;
				else if(i == Direction::south)
					dirVec.y = -1;
				else if(i == Direction::east)
					dirVec.x = 1;
				else if(i == Direction::west)
					dirVec.x = -1;
				Vec2i nextPos {currentPos.x + dirVec.x, currentPos.y + dirVec.y};

				if(closedList[nextPos.x][nextPos.y])
					continue;

				float gCost = currentNode->gCost + distBetweenVec2i(currentPos, nextPos);
				float hCost = distBetweenVec2i(nextPos, destVec);
				float fCost = gCost + hCost;
				if(nodeMap[nextPos.x][nextPos.y].vec.x != -1)
				{
					if(fCost >= nodeMap[currentPos.x + dirVec.x][currentPos.y + dirVec.y].fCost)
						continue;
					nodeMap[nextPos.x][nextPos.y].parent = currentPos;
					nodeMap[nextPos.x][nextPos.y].fCost  = fCost;
					nodeMap[nextPos.x][nextPos.y].gCost  = gCost;
					nodeMap[nextPos.x][nextPos.y].hCost  = hCost;
					openList.erase(std::remove(openList.begin(), openList.end(), nextPos), openList.end());
				}
				else
				{
					nodeMap[currentPos.x + dirVec.x][currentPos.y + dirVec.y].vec    = nextPos;
					nodeMap[currentPos.x + dirVec.x][currentPos.y + dirVec.y].parent = currentPos;
					nodeMap[currentPos.x + dirVec.x][currentPos.y + dirVec.y].fCost  = fCost;
					nodeMap[currentPos.x + dirVec.x][currentPos.y + dirVec.y].gCost  = gCost;
					nodeMap[currentPos.x + dirVec.x][currentPos.y + dirVec.y].hCost  = hCost;
				}

				int insertIndex = getIndexOfInsertion(openList, nodeMap, nextPos);

				openList.insert(openList.begin() + insertIndex, nextPos);
			}
			if(openList.size() == 0 /*|| openList.size() > BOARD_SIZE * BOARD_SIZE / 4*/)
			{
				Log::warning("Cannot find route to destination");
				std::vector<Vec2f> *path = new std::vector<Vec2f>();
				path->push_back(dest);
				return path;
			}
		}
		std::vector<Vec2i> path;
		Vec2i              currentPos = openList.back();
		while(currentPos != startVec)
		{
			Node *currentNode = &nodeMap[currentPos.x][currentPos.y];
			path.push_back({currentNode->vec.x, currentNode->vec.y});
			currentPos = currentNode->parent;
		}
		if(path.size() > 2)
		{
			float mid = ((float) Tile::TILE_SIZE * ROOM_SIZE) / 2;
			dest      = {(float) path[path.size() - 2].x * Tile::TILE_SIZE * ROOM_SIZE + mid, (float) path[path.size() - 2].y * Tile::TILE_SIZE * ROOM_SIZE + mid};
		}
	}
	std::vector<Vec2f> *path = Level::getPath(startPos, dest, collisionBox);
	return path;
}
// !SECTION

// SECTION: Generation
void Maze::generatePaths(int layerMax, int startMax, bool isInSubThread)
{
	// Log::info("Generating paths");
	int layer = 0;

	// Resets the paths north
	for(int i = 0; i < BOARD_SIZE; i++)
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

	std::vector<Vec2i> newPaths;    // This stores the newPaths for the next layer
	newPaths.reserve(BOARD_SIZE);   // Reserves space because, not all spaces may be used
	while(currentPaths.size() > 0)
	{
		for(int i = 0; i < currentPaths.size(); i++)
		{   // This goes through all the current path options and generates the options for the rooms
			Vec2i pos = currentPaths[i];

			if(get(pos.y, pos.x))   // Checks that the pointer is a nullptr - so it doesn't overwrite any rooms
				continue;

			int prob = startMax;   // This sets the probability of for the chance of generating an entrance
			if(pos.x > (3 * BOARD_SIZE / 4 + 1) || pos.x < BOARD_SIZE / 4 + 1)
			{
				prob++;
			}
			else if(pos.y > (3 * BOARD_SIZE / 4 + 1) || pos.y < BOARD_SIZE / 4 + 1)
			{
				prob++;
			}

			int pathCount = 0;   // Stores the number of paths from that room

			// 0: closed but could be open, 1: open, 2: closed and cannot be open
			EntranceState north = shouldBeOpen(get(pos.y + 1, pos.x), SOUTH_ENTRANCE, prob, &pathCount);
			EntranceState south = shouldBeOpen(get(pos.y - 1, pos.x), NORTH_ENTRANCE, prob, &pathCount);
			EntranceState east  = shouldBeOpen(get(pos.y, pos.x + 1), WEST_ENTRANCE, prob, &pathCount);
			EntranceState west  = shouldBeOpen(get(pos.y, pos.x - 1), EAST_ENTRANCE, prob, &pathCount);

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
			else if(pathCount == 2 && layer < layerMax - BOARD_SIZE / 3)
			{
				int r = Random::getNum(0, 2);
				if(r != 2)
				{
					forceEntrance(&north, &south, &east, &west);
				}
			}

			// This is to check if any errors have occurred when generating the maze
			if(north == EntranceState::isOpen && pos.y < BOARD_SIZE - 1 && get(pos.y + 1, pos.x) && !get(pos.y + 1, pos.x)->isOpen(SOUTH_ENTRANCE))
				Log::error("Room generated incorrectly!", LOGINFO);
			if(south == EntranceState::isOpen && pos.y > 0 && get(pos.y - 1, pos.x) && !get(pos.y - 1, pos.x)->isOpen(NORTH_ENTRANCE))
				Log::error("Room generated incorrectly!", LOGINFO);
			if(east == EntranceState::isOpen && pos.x < BOARD_SIZE - 1 && get(pos.y, pos.x + 1) && !get(pos.y, pos.x + 1)->isOpen(WEST_ENTRANCE))
				Log::error("Room generated incorrectly!", LOGINFO);
			if(west == EntranceState::isOpen && pos.x > 0 && get(pos.y, pos.x - 1) && !get(pos.y, pos.x - 1)->isOpen(EAST_ENTRANCE))
				Log::error("Room generated incorrectly!", LOGINFO);

			// This adds any new paths made to the newPaths list
			if(north == EntranceState::isOpen && pos.y < BOARD_SIZE - 1 && !get(pos.y + 1, pos.x))
				newPaths.push_back({pos.x, pos.y + 1});
			if(south == EntranceState::isOpen && pos.y > 0 && !get(pos.y - 1, pos.x))
				newPaths.push_back({pos.x, pos.y - 1});
			if(east == EntranceState::isOpen && pos.x < BOARD_SIZE - 1 && !get(pos.y, pos.x + 1))
				newPaths.push_back({pos.x + 1, pos.y});
			if(west == EntranceState::isOpen && pos.x > 0 && !get(pos.y, pos.x - 1))
				newPaths.push_back({pos.x - 1, pos.y});

			addRoom(pos.x, pos.y,
					north == EntranceState::isOpen,
					south == EntranceState::isOpen,
					east == EntranceState::isOpen,
					west == EntranceState::isOpen,
					isInSubThread);
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
	std::thread t1(&Maze::generatePaths, this, layerMax, startMax, true);   // This starts the multithreading
	t1.detach();
}

void Maze::generate()
{
	for(int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
	{
		if(board[i])
		{
			delete board[i];   // NOTE: This frees up the memory, but does not make it a nullptr
			board[i] = nullptr;
		}
	}
	yoffset      = 0;
	xoffset      = 0;
	int midpoint = BOARD_SIZE / 2 + 1;
	// NOTE: MUST DELETE ALL ROOMS!

	addRoom(midpoint, midpoint, true, true, true, true, false);
	currentPaths.push_back({midpoint - 1, midpoint});
	currentPaths.push_back({midpoint, midpoint - 1});
	currentPaths.push_back({midpoint + 1, midpoint});
	currentPaths.push_back({midpoint, midpoint + 1});

	multithreadGenerating(BOARD_SIZE * 4 / 5, 1);
	// TODO: Add check to see if okay maze
}

void Maze::moveNorth()
{
	for(int i = 0; i < BOARD_SIZE; i++)
	{
		// NOTE: The coordinates given is the top layer
		if(pathsNorth[i])
			currentPaths.push_back({i, BOARD_SIZE - 1});

		// This deletes any room that is being forgotten
		// NOTE it is 0 because it is getting rid of the bottom layer - which will become the top layer
		removeRoom(0, i);
	}
	yoffset++;   // Changes the y offset because of the new layout
	if(yoffset == BOARD_SIZE)
		yoffset = 0;
	if(currentPaths.size() > 0)   // This checks if there is any point to generate new paths
		multithreadGenerating(LAYER_MAX_FOR_DIRECTIONS, 1);
	else
		updatePaths();
}

void Maze::moveSouth()
{   // These all do the same as moveNorth but just specialized for the direction
	for(int i = 0; i < BOARD_SIZE; i++)
	{
		if(pathsSouth[i])
			currentPaths.push_back({i, 0});

		removeRoom(BOARD_SIZE - 1, i);
	}
	yoffset--;
	if(yoffset == -1)
		yoffset = BOARD_SIZE - 1;
	if(currentPaths.size() > 0)
		multithreadGenerating(LAYER_MAX_FOR_DIRECTIONS, 1);
	else
		updatePaths();
}

void Maze::moveEast()
{
	for(int i = 0; i < BOARD_SIZE; i++)
	{
		if(pathsEast[i])
			currentPaths.push_back({BOARD_SIZE - 1, i});

		removeRoom(i, 0);
	}
	xoffset++;
	if(xoffset == BOARD_SIZE)
		xoffset = 0;
	if(currentPaths.size() > 0)
		multithreadGenerating(LAYER_MAX_FOR_DIRECTIONS, 1);
	else
		updatePaths();
}

void Maze::moveWest()
{
	for(int i = 0; i < BOARD_SIZE; i++)
	{
		if(pathsWest[i])
			currentPaths.push_back({0, i});

		removeRoom(i, BOARD_SIZE - 1);
	}
	xoffset--;
	if(xoffset == -1)
		xoffset = BOARD_SIZE - 1;
	if(currentPaths.size() > 0)
		multithreadGenerating(LAYER_MAX_FOR_DIRECTIONS, 1);
	else
		updatePaths();
}

Maze::EntranceState Maze::shouldBeOpen(Room *room, int nextEntrance, int prob, int *pathCount)
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

	for(int i = 0; i < BOARD_SIZE; i++)
	{
		if(get(BOARD_SIZE - 1, i) && get(BOARD_SIZE - 1, i)->isOpen(NORTH_ENTRANCE))
			pathsNorth[i] = true;
		if(get(0, i) && get(0, i)->isOpen(SOUTH_ENTRANCE))
			pathsSouth[i] = true;
		if(get(i, BOARD_SIZE - 1) && get(i, BOARD_SIZE - 1)->isOpen(EAST_ENTRANCE))
			pathsEast[i] = true;
		if(get(i, 0) && get(i, 0)->isOpen(WEST_ENTRANCE))
			pathsWest[i] = true;
	}
}

Entity *Maze::entityCollisionDetection(float nextX, float nextY, CollisionBox collisionBox)
{
	if(m_Player.hasCollidedWith(nextX, nextY, collisionBox))
		return &m_Player;
	return Level::entityCollisionDetection(nextX, nextY, collisionBox);
}
// !SECTION

// SECTION: Getters
Room *Maze::get(int y, int x)
{
	if(x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE)
		return nullptr;
	return board[coordsToIndex(x, y)];
}

Tile *Maze::getTile(int x, int y)
{
	int roomX = x / ROOM_SIZE;
	int tileX = x % ROOM_SIZE;
	int roomY = y / ROOM_SIZE;
	int tileY = y % ROOM_SIZE;

	Room *room = get(roomY, roomX);
	if(!room)
	{
		// Log::warning("Trying to access room that doesn't exist!");
		return nullptr;
	}

	return room->getTile(tileX, tileY);
}
// !SECTION
