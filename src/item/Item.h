#pragma once

#include <string>

#include "Sprite.h"

class Item
{
  protected:
	Sprite::ID  m_SpriteID;
	std::string m_Name;

  public:
	Item();
	Item(std::string name, Sprite::ID spriteID);
	Item(const char *name, Sprite::ID spriteID);
	virtual ~Item();

	void render(float x, float y, double rotation, float size, uint8_t layer, bool isOverlay = false);

	Sprite::ID   getSpriteID();
	std::string *getName();
};