#pragma once

#include "Camera.h"
#include "Renderer.h"
#include "Room.h"

class EmptyRoom : public Room
{
  public:
	EmptyRoom(bool entrances[4]);
	~EmptyRoom() override;

#ifdef DEBUG
	void imGuiRender() override;
#endif
};