#include "Item.h"

#include "Renderer.h"

Item::Item()
	: m_SpriteID(0), m_Name("I am an item")
{
}
Item::Item(const char *name, uint32_t spriteID)
	: m_SpriteID(spriteID), m_Name(name)
{
}
Item::~Item()
{
}

void Item::render(float x, float y, double rotation, float size, uint8_t layer, bool isOverlay)
{
	Render::sprite(x, y, rotation, size, m_SpriteID, layer, isOverlay);
}

int          Item::getSpriteID() { return m_SpriteID; }
std::string *Item::getName() { return &m_Name; }