#pragma once

#include "GLM.h"

#include "Layer.h"

#include "Mob.h"

class Entity;

class Camera
{
  private:
	float x, y;
	float zoomPercentage;
	float moveSpeed;
	bool  moveLock;
	bool  updateView;
	bool  lockOnAnchor;
	Mob * m_Anchor;

  public:
	Camera();
	Camera(float x, float y);

	void update();
	void render();
#ifdef DEBUG
	void imGuiRender();
#endif

	bool eventCallback(const Application::Event &e);
	bool setEffect(const Effect::RenderEffect &e);

	glm::mat4 getView();
	bool      isInFrame(float x, float y); // TODO: add Collision box
	void      setLock(bool locked);
	void      setX(float newX);
	void      setY(float newY);
	void      setAnchor(Mob *e);
	float     getZoom();
	void      keyCallback(int key, int scancode, int action, int mods);
	void      changeUpdateView();

	float getX() { return x; }
	float getY() { return y; }

	Vec2f convertWindowToLevel(Vec2f inp);
};