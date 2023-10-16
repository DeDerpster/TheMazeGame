#pragma once

#include "Camera.h"
#include "Level.h"
#include "Renderer.h"
#include "Room.h"
#include "Shader.h"

#include "AnimatedSprite.h"
#include "MazeHeaders.h"

#include "Player.h"

struct VecInt2
{
	int x, y;
};

class Maze : public Level
{
  private:
	static const int BOARD_SIZE = 11;

	std::vector<Room *> board;         // This stores Room * so that you can have the different subclasses of rooms also stored
	int                 xoffset = 0;   // This is the offset, which will allow the board not to shift all the pointers when more of the maze is generated
	int                 yoffset = 0;   // instead this allows keeps track of how "offset" the coordinates are

	// This is for multithreading - and are the variables that allow threads to communicate
	// int  addRoomAtX = -1, addRoomAtY = -1;
	// bool addRoomEntrances[4] = {false, false, false, false};
	bool finishedGenerating = true;

	std::vector<VecInt2> currentPaths;

	bool pathsNorth[BOARD_SIZE];
	bool pathsSouth[BOARD_SIZE];
	bool pathsEast[BOARD_SIZE];
	bool pathsWest[BOARD_SIZE];

	Player m_Player;

	enum EntranceState
	{
		couldOpen = 0,
		isOpen    = 1,
		isClosed  = 2
	};

#ifdef DEBUG
	bool renderAll = false;
#endif

	int           coordsToIndex(int x, int y);
	void          addRoom(int x, int y, bool north, bool south, bool east, bool west, bool isInSubThread);
	void          removeRoom(int y, int x);
	void          updatePaths();
	EntranceState shouldBeOpen(Room *room, int nextEntrance, int prob, int *pathCount);
	void          forceEntrance(EntranceState *north, EntranceState *south, EntranceState *east, EntranceState *west);
	void          multithreadGenerating(int layerMax, int startMax);
	void          generatePaths(int layerMax, int startMax, bool isInSubThread);

	virtual bool eventCallback(const Application::Event &e) override;
	virtual bool setEffect(const Effect::RenderEffect &e) override;

  public:
	Maze();
	~Maze();
	virtual void render() override;
	virtual void update() override;
#ifdef DEBUG
	virtual void imGuiRender() override;
#endif
	virtual void updateMVP(glm::mat4 &view) override;

	void          generate();
	void          moveNorth();
	void          moveSouth();
	void          moveEast();
	void          moveWest();
	Room *        get(int y, int x);
	virtual Tile *getTile(int x, int y) override;
};
