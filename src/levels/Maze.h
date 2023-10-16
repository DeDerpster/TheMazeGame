#pragma once

#include "Camera.h"
#include "Level.h"
#include "Renderer.h"
#include "Room.h"
#include "Shader.h"

#include "AnimatedSprite.h"
#include "MazeHeaders.h"

#include "Player.h"

class Maze : public Level
{
  protected:
	static const int BOARD_SIZE = 11;

	std::vector<Room *> board;         // This stores Room * so that you can have the different subclasses of rooms also stored
	int                 xoffset = 0;   // This is the offset, which will allow the board not to shift all the pointers when more of the maze is generated
	int                 yoffset = 0;   // instead this allows keeps track of how "offset" the coordinates are

	// This is for multithreading - and are the variables that allow threads to communicate
	bool finishedGenerating = true;

	std::vector<Vec2i> currentPaths;   // This stores the current possible paths

	// This stores the entrance state, allowing the possibility of forcing an entrance later on after generatings
	enum EntranceState
	{
		couldOpen = 0,
		isOpen    = 1,
		isClosed  = 2
	};

	bool pathsNorth[BOARD_SIZE];   // This stores the avaliable paths for each direction, allowing generation when the player moves
	bool pathsSouth[BOARD_SIZE];
	bool pathsEast[BOARD_SIZE];
	bool pathsWest[BOARD_SIZE];

	Player                m_Player;

#ifdef DEBUG
	bool renderAll = false;
#endif

	int coordsToIndex(int x, int y);

	void addRoom(int x, int y, bool north, bool south, bool east, bool west);
	void removeRoom(int y, int x);
	void updatePaths();

	void multithreadGenerating(int layerMax, int startMax);
	void generatePaths(int layerMax, int startMax);

	EntranceState shouldBeOpen(Room *room, int nextEntrance, int prob, int *pathCount);
	void          forceEntrance(EntranceState *north, EntranceState *south, EntranceState *east, EntranceState *west);

	void playerDeath();
	void resetMaze();

  public:
	Maze();
	~Maze();

	virtual void render() override;
	virtual void update() override;
#ifdef DEBUG
	virtual void imGuiRender() override;
#endif
	virtual bool eventCallback(const Event::Event &e) override;

	void generate();
	void moveNorth();
	void moveSouth();
	void moveEast();
	void moveWest();

	Room *                      get(int y, int x);
	virtual Tile *              getTile(int x, int y) override;
	virtual Player *            getPlayer() override { return &m_Player; }

	virtual std::vector<Vec2f> *getPath(Vec2f startPos, Vec2f dest, CollisionBox collisionBox) override;

	virtual Entity *entityCollisionDetection(float nextX, float nextY, CollisionBox collisionBox) override;
};
