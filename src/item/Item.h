#pragma once

#include <string>

#include "rendering/sprite/Sprite.h"

class Item
{
  protected:
	// Stores the sprite of the item
	Sprite::ID  m_SpriteID;
	std::string m_Name;   // Stores the name of the item

	bool forceRenderOnly;

  public:
	Item();
	Item(std::string name, Sprite::ID spriteID);
	Item(const char *name, Sprite::ID spriteID);
	virtual ~Item();

	// Renders the item at a given settings (allows the ease of rendering the item in different scenarios)
	void render(float x, float y, double rotation, float size, uint8_t layer, bool isOverlay = false, bool forceRender = false);

	Sprite::ID   getSpriteID();
	std::string *getName();

	// This allows for when transfering an object to stop the rendering of it in MIHMs
	void setForcedRender(bool forceRender) { forceRenderOnly = forceRender; }
};