#pragma once

#include <string>

#include "rendering/sprite/Sprite.h"

class Item
{
  protected:
	Sprite::ID  m_SpriteID;
	std::string m_Name;

	bool forceRenderOnly;

  public:
	Item();
	Item(std::string name, Sprite::ID spriteID);
	Item(const char *name, Sprite::ID spriteID);
	virtual ~Item();

	void render(float x, float y, double rotation, float size, uint8_t layer, bool isOverlay = false, bool forceRender = false);

	Sprite::ID   getSpriteID();
	std::string *getName();

	void setForcedRender(bool forceRender) { forceRenderOnly = forceRender; }
};