#pragma once

#include <functional>

#include "Event.h"

class MenuObject
{
  protected:
	float x, y;
	float width, height;

	std::function<void(float *, float *, float *, float *)> positionFunc;

  public:
	MenuObject(float x, float y, float width, float height);
	MenuObject(std::function<void(float *, float *, float *, float *)> posFunc);
	virtual ~MenuObject();

	virtual void render() = 0;
	virtual void update() = 0;
	virtual bool eventCallback(const Event::Event &e);

	float getX() { return x; }
	float getY() { return y; }
};