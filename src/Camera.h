#pragma once

#include "GLM.h"

#include "Layer.h"
#include "Mob.h"

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
	uint16_t m_ZoomEffectID;
	uint16_t m_PositionEffectID;

  public:
	Camera();
	Camera(float x, float y);

	void update();
	void render();
#ifdef DEBUG
	void imGuiRender();
#endif

	bool eventCallback(const Event::Event &e);
	bool setEffect(const Effect::RenderEffect &e);

	// TODO: Sort this out
	uint16_t  getPositionEffectID();
	uint16_t  getZoomEffectID();
	void      setShaderEffects();
	void      updatePositionEffect();
	void      updateZoomEffect();
	bool      isInFrame(float x, float y, CollisionBox &box);
	void      setLock(bool locked);
	void      setX(float newX);
	void      setY(float newY);
	void      setAnchor(Mob *e);
	void      clearAnchor();
	float     getZoom();
	void      keyCallback(int key, int scancode, int action, int mods);
	void      changeUpdateView();

	float getX() { return x; }
	float getY() { return y; }

	Vec2f convertWindowToLevel(Vec2f inp);
};