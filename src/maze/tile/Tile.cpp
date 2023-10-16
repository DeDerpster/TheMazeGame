#include "Tile.h"

#include "Application.h"
#include "Log.h"
#include "layer/level/Level.h"
#include "rendering/sprite/Sprite.h"

#include "event/game/MazeMoved.h"

Tile::Tile()
	: m_SpriteID(Sprite::ID::errorID), x(0.0f), y(0.0f), rotation(0.0f), m_IsSolid(false), m_Level(nullptr)
{
}
Tile::Tile(float x, float y, double rotation, Sprite::ID id, bool isSolid, Level *level)
	: m_SpriteID(id), x(x), y(y), rotation(rotation), m_IsSolid(isSolid), m_Level(level)
{
}

Tile::~Tile()
{
}

CollisionBox Tile::getCollisionBox()   // Simple collision box for a tile
{
	return {{-TILE_SIZE / 2, -TILE_SIZE / 2}, {TILE_SIZE / 2, TILE_SIZE / 2}};
}

void Tile::render()
{
	// Renders the tile on layer 0
	uint8_t layer = 0;
	Render::sprite(x, y, rotation, TILE_SIZE, m_SpriteID, layer);
}

void Tile::update()
{
}

bool Tile::eventCallback(const Event::Event &e)
{
	switch(e.getType())
	{
	case Event::EventType::MazeMoved:
	{   // Checks for maze moved events and updates its position
		const Event::MazeMovedEvent &ne = static_cast<const Event::MazeMovedEvent &>(e);
		x += ne.changeX;
		y += ne.changeY;
	}

	default:
		return false;
	}
}

#ifdef DEBUG
void Tile::imGuiRender()
{
}
#endif