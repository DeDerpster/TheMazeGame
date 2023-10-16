#include "Tile.h"

#include "Application.h"
#include "Level.h"
#include "Log.h"
#include "Sprite.h"

Tile::Tile()
	: m_SpriteID(0), x(0.0f), y(0.0f), rotation(0.0f), m_IsSolid(false), m_Level(nullptr)
{
}
Tile::Tile(float x, float y, double rotation, uint32_t texID, bool isSolid, Level *level)
	: m_SpriteID(texID), x(x), y(y), rotation(rotation), m_IsSolid(isSolid), m_Level(level)
{
}

Tile::~Tile()
{
}

void Tile::render()
{
	Render::sprite(x, y, rotation, TILE_SIZE, m_SpriteID);
}

void Tile::update()
{
}

bool Tile::eventCallback(const Event::Event &e)
{
	if(e.getType() == Event::EventType::mazeMovedEvent)
	{
		const Event::MazeMovedEvent &ne = static_cast<const Event::MazeMovedEvent &>(e);
		x += ne.changeX;
		y += ne.changeY;
	}

	return false;
}

#ifdef DEBUG
void Tile::imGuiRender()
{
}
#endif