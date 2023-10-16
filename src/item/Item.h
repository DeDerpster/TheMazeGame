#pragma once

#include <string>

class Item
{
  protected:
	uint16_t    m_SpriteID;
	std::string m_Name;

  public:
	Item();
	Item(const char *name, uint32_t spriteID);
	virtual ~Item();

	void render(float x, float y, double rotation, float size, uint8_t layer, bool isOverlay = false);

	int          getSpriteID();
	std::string *getName();
};