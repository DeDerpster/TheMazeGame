#include "Maze.h"

#include "ImGui.h"

#include "Log.h"
#include "RandomGen.h"

#include "EmptyRoom.h"

#include "Application.h"

static const int LAYER_MAX_FOR_DIRECTIONS = 4;

Maze::Maze()
{
	// NOTE: Because of how it is rendering the coords (0,0) on the board is the bottom left, not the top left!!

	board.resize(BOARD_SIZE * BOARD_SIZE, nullptr);   // It is resized, because all positions are used straight away and fills any data slots will nullptr
	currentPaths.reserve(2 * BOARD_SIZE);             // The data is reserved here because not all the data is needed, but it could be used and so for efficiency, it is reserved on init

	m_Shader = std::make_unique<Shader>("res/shaders/BasicShader.glsl");

	m_Shader->bind();

	int samplers[32];
	for(int i = 0; i < 32; i++)
		samplers[i] = i;
	m_Shader->SetUniform1iv("u_Textures", 32, samplers);

	float zoomLevel = 1.0f;
	m_Shader->SetUniform4f("u_Zoom", zoomLevel, zoomLevel, 1.0f, 1.0f);
	testPlayer = std::make_unique<Player>(4500.0f, 4500.0f, this);
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
	Log::info("Maze destroyed");
}

void Maze::render()
{
	// m_Renderer->resetBuffer();

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
		if(mid->isOpen(0))
			get(midpoint + 1, midpoint)->render(midpoint * multiple, (midpoint + 1) * multiple);
		if(mid->isOpen(1))
			get(midpoint - 1, midpoint)->render(midpoint * multiple, (midpoint - 1) * multiple);
		if(mid->isOpen(2))
			get(midpoint, midpoint + 1)->render((midpoint + 1) * multiple, midpoint * multiple);
		if(mid->isOpen(3))
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
		/*for(int i = midpoint - 1; i < midpoint + 2; i++)
	{
		for(int j = midpoint - 1; j < midpoint + 2; j++)
		{
			Room *room = get(i, j);
			if(room)
				room->render(j * multiple, i * multiple);
		}
	}*/

#endif
	testPlayer->render();
	m_Shader->bind();
}

void Maze::updateMVP(glm::mat4 &view)
{
	glm::mat4 mvp = Application::getProj() * view;
	m_Shader->SetUniformMat4f("u_MVP", mvp);
}

void Maze::update()
{
	bool movedPlayer = false;
	if(testPlayer->getY() > ((BOARD_SIZE / 2) + 2) * Tile::TILE_SIZE * ROOM_SIZE)
	{
		Log::info("Player moved North");
		testPlayer->changeY(-Tile::TILE_SIZE * ROOM_SIZE);
		moveNorth();
		movedPlayer = true;
	}
	if(testPlayer->getY() < ((BOARD_SIZE / 2) + 1) * Tile::TILE_SIZE * ROOM_SIZE)
	{
		Log::info("Player moved South");
		testPlayer->changeY(Tile::TILE_SIZE * ROOM_SIZE);
		moveSouth();
		movedPlayer = true;
	}
	if(testPlayer->getX() > ((BOARD_SIZE / 2) + 2) * Tile::TILE_SIZE * ROOM_SIZE)
	{
		Log::info("Player moved East");
		testPlayer->changeX(-Tile::TILE_SIZE * ROOM_SIZE);
		moveEast();
		movedPlayer = true;
	}
	if(testPlayer->getX() < ((BOARD_SIZE / 2) + 1) * Tile::TILE_SIZE * ROOM_SIZE)
	{
		Log::info("Player moved West");
		testPlayer->changeX(Tile::TILE_SIZE * ROOM_SIZE);
		moveWest();
		movedPlayer = true;
	}
	testPlayer->update();
	if(movedPlayer)
	{
		camera->changeUpdateView();
		camera->update();
	}
	// TODO: Add check to see if all of the pathsNorth... are all false - so will need to reset the maze
	for(int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
	{
		if(board[i])   // This makes sure that the board[i] is not a nullptr - so that it won't throw any errors
			board[i]->update();
	}
	// TODO: change this so it only updates the 9 center ones
}

#ifdef DEBUG
void Maze::imGuiRender()
{
	if(ImGui::Button("Reload Maze"))
	{
		generate();
	}
	ImGui::Checkbox("Render all", &renderAll);
	testPlayer->imGuiRender();
	/*if(ImGui::Button("Move north"))
		moveNorth();
	if(ImGui::Button("Move south"))
		moveSouth();
	if(ImGui::Button("Move east"))
		moveEast();
	if(ImGui::Button("Move west"))
		moveWest();*/
}
#endif

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
	/*if(false)
	{
		addRoomEntrances[NORTH_ENTRANCE] = north;   // This sets the variables that can be accessed by the main thread, because they are static
		addRoomEntrances[SOUTH_ENTRANCE] = south;
		addRoomEntrances[EAST_ENTRANCE]  = east;
		addRoomEntrances[WEST_ENTRANCE]  = west;
		addRoomAtX                       = x;
		addRoomAtY                       = y;
		using namespace std::literals::chrono_literals;
		while(addRoomAtX != -1)   // NOTE: This checks if the main thread has finished creating the last tile before moving on - this also slows the thread so it's not at full capacity all the time
		{
			std::this_thread::sleep_for(2ms);
		}
	}
	else
	{   // If in the main thread (And not in a sub thread) then it will do it normally */
	bool entrances[4]          = {north, south, east, west};
	board[coordsToIndex(x, y)] = new EmptyRoom(entrances);
	// }
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

	std::vector<VecInt2> newPaths;   // This stores the newPaths for the next layer
	newPaths.reserve(BOARD_SIZE);    // Reserves space because, not all spaces may be used
	while(currentPaths.size() > 0)
	{
		for(int i = 0; i < currentPaths.size(); i++)
		{   // This goes through all the current path options and generates the options for the rooms
			VecInt2 pos = currentPaths[i];

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

void Maze::multithreadGenerating(int layerMax, int startMax)
{
	/*addRoomAtX          = -1;   // TODO: should probably change this to a struct or something
	addRoomAtY          = -1;   // This is for multithreading
	addRoomEntrances[0] = false;
	addRoomEntrances[1] = false;
	addRoomEntrances[2] = false;
	addRoomEntrances[3] = false;*/

	if(!finishedGenerating)
		Log::critical("Stacked maze generating!!", LOGINFO);
	finishedGenerating = false;
	std::thread t1(&Maze::generatePaths, this, layerMax, startMax, true);   // This starts the multithreading
	t1.detach();
	/*while(!finishedGenerating)
	{
		if(addRoomAtX != -1 && addRoomAtY != -1)
		{   // NOTE: An error is called when you try to create empty room from thread - this is because openGL does not support it
			board[coordsToIndex(addRoomAtX, addRoomAtY)] = new EmptyRoom(addRoomEntrances);
			addRoomAtX                                   = -1;
			addRoomAtY                                   = -1;
		}
	}
	t1.join();   // This waits for the thread to be done*/
}

// ANCHOR: Generation
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

Room *Maze::get(int y, int x)
{
	if(x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE)
		return nullptr;
	return board[coordsToIndex(x, y)];
}

// ANCHOR callbacks
bool Maze::eventCallback(const Application::Event &e)
{
	return false;
}

bool Maze::setEffect(const Effect::RenderEffect &e)
{
	e.setEffect(*m_Shader);
	return false;
}

Tile *Maze::getTile(int x, int y)
{
	int roomX = x / ROOM_SIZE;
	int tileX = x % ROOM_SIZE;
	int roomY = y / ROOM_SIZE;
	int tileY = y % ROOM_SIZE;

	return get(roomY, roomX)->getTile(tileX, tileY);
}