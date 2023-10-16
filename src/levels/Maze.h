#pragma once

#include "Camera.h"
#include "GUILayer.h"
#include "Level.h"
#include "Renderer.h"
#include "Room.h"

#include "AnimatedSprite.h"
#include "MazeHeaders.h"

#include "Player.h"

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

	// TODO: Change this to application?
	// Menu layers - these are the layers that are involved in the menu system and can be quickly pushed to the layer stack if needed
	GUILayer *activeGUI;   // This stores the current GUI layer in the application stack, so that when deleting layers, it does not cause an error
	GUILayer *m_OverlayGUI;
	GUILayer *m_InventoryGUI;
	GUILayer *m_ChestGUI;

	std::function<void(std::vector<Item *> &)> setChestToMenu;

	void addRoom(int x, int y, bool north, bool south, bool east, bool west);
	void updatePaths();

	void multithreadGenerating(int layerMax, int startMax);
	void generatePaths(int layerMax, int startMax);

	EntranceState shouldBeOpen(Room *room, Direction nextEntrance, int prob, int *pathCount);
	void          forceEntrance(EntranceState *north, EntranceState *south, EntranceState *east, EntranceState *west);

	// TODO: Make this a event
	virtual void playerDeath() override;
	void resetMaze();

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

	// Functions for interaction with gui layers
	virtual void openChest(std::vector<Item *> &items) override;
	virtual void endLevel() override;
	void         returnToGame();
};
