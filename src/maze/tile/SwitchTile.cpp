#include "SwitchTile.h"

#include "event/game/ShowAlternatives.h"

SwitchTile::SwitchTile()
	: Tile(), m_AltSpriteID(Sprite::ID::errorID), showAlt(false)
{
}

SwitchTile::SwitchTile(float x, float y, double rotation, Sprite::ID defaultSpriteID, double altRotation, Sprite::ID altSpriteID, bool isSolid, Level *level)
	: Tile(x, y, rotation, defaultSpriteID, isSolid, level), m_AltSpriteID(altSpriteID), m_AltRotation(altRotation), showAlt(false)
{
}

SwitchTile::~SwitchTile()
{
}

void SwitchTile::render()
{
	uint8_t layer = 0;
	if(showAlt)
		Render::sprite(x, y, m_AltRotation, TILE_SIZE, m_AltSpriteID, layer);
	else
		Render::sprite(x, y, rotation, TILE_SIZE, m_SpriteID, layer);
}

void SwitchTile::update()
{
}

bool SwitchTile::eventCallback(const Event::Event &e)
{
	switch(e.getType())
	{
	case Event::EventType::ShowAltTile:
	{
		// TODO: Add particles
		const Event::ShowAltTileEvent &ne = static_cast<const Event::ShowAltTileEvent &>(e);

		showAlt = ne.showAlt;
		return false;
	}

	default:
		return Tile::eventCallback(e);
	}
}

#ifdef DEBUG
void SwitchTile::imGuiRender()
{
}
#endif
