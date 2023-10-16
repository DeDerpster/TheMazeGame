#pragma once
#include <string>

#include "Sprite.h"

class Item
{
  protected:
	uint16_t    m_SpriteID;
	std::string m_Name;

  public:
	Item()
		: m_SpriteID(0), m_Name("I am an item") {}
	Item(const char *name, uint32_t spriteID)
		: m_SpriteID(spriteID), m_Name(name) {}
	virtual ~Item() {}

	void render(float x, float y, double rotation, float size)
	{
		Render::Sprite::getSprite(m_SpriteID)->render(x, y, rotation, size);
	}

	int          getSpriteID() { return m_SpriteID; }
	std::string *getName() { return &m_Name; }
};