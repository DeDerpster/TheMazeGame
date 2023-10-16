#pragma once

#include <GLM.h>

#include "entity/movableEntity/mob/Mob.h"
#include "layer/Layer.h"

#include "CollisionBox.h"

class Camera
{
  private:
	// All the variables for the camera
	float x, y;
	float zoomPercentage;   // Stores the zoom level
	float moveSpeed;        // This is for when it is not following a character
	bool  moveLock;         // Locks the movement
	// Information for the following a mob e.g. the player
	bool  lockOnAnchor;
	Mob * m_Anchor;
	// Stores information about its effect
	uint16_t m_ZoomEffectID;
	uint16_t m_PositionEffectID;
	bool     updateView;   // Variable to tell it to update its effect

	void updatePositionEffect();
	void updateZoomEffect();

  public:
	Camera();
	Camera(float x, float y);

	void update();
	void render();
#ifdef DEBUG
	void imGuiRender();
#endif

	bool eventCallback(const Event::Event &e);

	float     getX() { return x; }
	float     getY() { return y; }
	float     getZoom();
	uint16_t  getPositionEffectID();
	uint16_t  getZoomEffectID();
	Vec2f     convertWindowToLevel(Vec2f inp);
	bool      isInFrame(float x, float y, CollisionBox &box);

	void      setShaderEffects();
	void      setLock(bool locked);
	void      setX(float newX);
	void      setY(float newY);
	void      setAnchor(Mob *e);
	void      clearAnchor();
	void      changeUpdateView();
};