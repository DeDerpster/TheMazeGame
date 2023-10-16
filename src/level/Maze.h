#pragma once

#include "MazeHeaders.h"
#include "level/Level.h"

#include "Camera.h"
#include "KeyDefinitions.h"

class Maze : public Level
{
  protected:
	// This is for multithreading - and are the variables that allow threads to communicate
	bool finishedGenerating;

	std::vector<Vec2i> currentPaths;   // This stores the current possible paths

	// This stores the entrance state, allowing the possibility of forcing an entrance later on after generatings
	enum EntranceState
	{
		couldOpen = 0,
		isOpen    = 1,
		isClosed  = 2
	};

	bool pathsNorth[MAZE_SIZE];   // This stores the avaliable paths for each direction, allowing generation when the player moves
	bool pathsSouth[MAZE_SIZE];
	bool pathsEast[MAZE_SIZE];
	bool pathsWest[MAZE_SIZE];

	uint64_t moves;
	bool     isMoving[4];

	void playerMoved(Direction dir);

	void addRoom(int x, int y, bool north, bool south, bool east, bool west);
	void updatePaths();

	void multithreadGenerating(int layerMax, int startMax);
	void generatePaths(int layerMax, int startMax);

	EntranceState shouldBeOpen(Room *room, Direction nextEntrance, int prob, int *pathCount);
	void          forceEntrance(EntranceState *north, EntranceState *south, EntranceState *east, EntranceState *west);

	// TODO: Make this a event
	virtual void endLevel() override;
	virtual void playerDeath() override;
	void         resetMaze();

  public:
	Maze();
	~Maze();

	virtual void update() override;
	virtual bool eventCallback(const Event::Event &e) override;
#ifdef DEBUG
	virtual void imGuiRender() override;
#endif

	void generate();
	void moveNorth();
	void moveSouth();
	void moveEast();
	void moveWest();
};
