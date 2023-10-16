#include "Item.h"

#include "Renderer.h"

Item::Item()
	: m_SpriteID(Sprite::ID::errorID), m_Name("I am an item")
{
}
Item::Item(std::string name, Sprite::ID spriteID)
	: m_SpriteID(spriteID), m_Name(name)
{
}

Item::Item(const char *name, Sprite::ID spriteID)
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

Sprite::ID   Item::getSpriteID() { return m_SpriteID; }
std::string *Item::getName() { return &m_Name; }