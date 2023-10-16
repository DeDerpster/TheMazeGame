#include "Entity.h"

#include "Tile.h"

Entity::Entity()
	: x(0.0f), y(0.0f), width(Tile::TILE_SIZE), height(Tile::TILE_SIZE), m_Level(nullptr), m_CollisionBox({{0.0f, 0.0f}, {0.0f, 0.0f}}), m_SpriteID(0) {}
Entity::Entity(float x, float y, float size, uint16_t spriteID)
	: x(x), y(y), width(size), height(size), m_Level(nullptr), m_CollisionBox({{-size / 2, -size / 2}, {size / 2, size / 2}}), m_SpriteID(spriteID) {}
Entity::Entity(float x, float y, float size, Level *level, uint16_t spriteID)
	: x(x), y(y), width(size), height(size), m_Level(level), m_CollisionBox({{-size / 2, -size / 2}, {size / 2, size / 2}}), m_SpriteID(spriteID) {}
Entity::Entity(float x, float y, float size, CollisionBox box, Level *level, uint16_t spriteID)
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
bool  Entity::getIsMoving() { return false; }

bool Entity::doesIntersectWith(Vec2f pos)
{
	float lowerX = x + m_CollisionBox.lowerBound.x;
	float lowerY = y + m_CollisionBox.lowerBound.y;
	float upperX = x + m_CollisionBox.upperBound.x;
	float upperY = y + m_CollisionBox.upperBound.y;

	return pos.x > lowerX && pos.y > lowerY && pos.x < upperX && pos.y < upperY;
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