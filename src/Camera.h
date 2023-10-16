#pragma once

#include "GLM.h"

#include "Layer.h"

#include "Entity.h"

class Entity;

class Camera
{
  private:
	float   x, y;
	float   zoomPercentage;
	float   moveSpeed;
	bool    moveUp, moveDown, moveLeft, moveRight;
	bool    moveLock;
	bool    updateView;
	bool    lockOnAnchor;
	Entity *m_Anchor;

  public:
	Camera();
	Camera(float x, float y);

	void update();
#ifdef DEBUG
	void imGuiRender();
#endif

	bool eventCallback(const Application::Event &e);
	bool setEffect(const Effect::RenderEffect &e);

	glm::mat4 getView();
	bool      isInFrame(float x, float y);
	void      setLock(bool locked);
	void      setX(float newX);
	void      setY(float newY);
	void      setAnchor(Entity *e);
	float     getZoom();
	void      keyCallback(int key, int scancode, int action, int mods);
	void      changeUpdateView();
};