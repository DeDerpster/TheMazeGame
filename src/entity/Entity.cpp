#include "Entity.h"

#include "KeyDefinitions.h"
#include "Level.h"

Entity::Entity()
	: x(0.0f), y(0.0f), width(TILE_SIZE), height(TILE_SIZE), m_Level(nullptr), m_CollisionBox({{0.0f, 0.0f}, {0.0f, 0.0f}}), m_SpriteID(Sprite::ID::errorID) {}
Entity::Entity(float x, float y, float size, Sprite::ID spriteID)
	: x(x), y(y), width(size), height(size), m_Level(nullptr), m_CollisionBox({{-size / 2, -size / 2}, {size / 2, size / 2}}), m_SpriteID(spriteID) {}
Entity::Entity(float x, float y, float size, Level *level, Sprite::ID spriteID)
	: x(x), y(y), width(size), height(size), m_Level(level), m_CollisionBox({{-size / 2, -size / 2}, {size / 2, size / 2}}), m_SpriteID(spriteID) {}
Entity::Entity(float x, float y, float size, CollisionBox box, Level *level, Sprite::ID spriteID)
	: x(x), y(y), width(size), height(size), m_Level(level), m_CollisionBox(box), m_SpriteID(spriteID) {}
Entity::~Entity() {}

bool Entity::eventCallback(const Event::Event &e)
{
	if(e.getType() == Event::EventType::mazeMovedEvent)
	{
		const Event::MazeMovedEvent &ne = static_cast<const Event::MazeMovedEvent &>(e);
		x += ne.changeX;
		y += ne.changeY;
	}
	return false;
}

float Entity::getX() const { return x; }
float Entity::getY() const { return y; }
float Entity::getWidth() const { return width; }
float Entity::getHeight() const { return height; }
bool  Entity::getIsMoving() { return false; }

bool Entity::doesIntersectWith(Vec2f pos)
{
	return doesPointIntersectWithBox(pos, {x, y}, m_CollisionBox);
}
bool Entity::deleteMe() { return false; }

void Entity::changeX(float changeBy) { x += changeBy; }
void Entity::changeY(float changeBy) { y += changeBy; }
void Entity::setLevel(Level *level) { m_Level = level; }

bool Entity::hasCollidedWith(float xs, float ys, CollisionBox box)
{
	if(xs + box.lowerBound.x >= x + m_CollisionBox.upperBound.x || x + m_CollisionBox.lowerBound.x >= xs + box.upperBound.x)
		return false;
	if(ys + box.upperBound.y <= y + m_CollisionBox.lowerBound.y || y + m_CollisionBox.upperBound.y <= ys + box.lowerBound.y)
		return false;
	return true;
}

#ifdef DEBUG
void Entity::imGuiRender()
{
}
#endif