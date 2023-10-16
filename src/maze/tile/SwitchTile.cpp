#include "SwitchTile.h"

#include "event/game/ShowAlternatives.h"
#include "layer/level/Level.h"
#include "spawner/particle/ParticleSpawner.h"

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
	// Chooses which sprite to render
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
		// Checks for the ShowAltTile event and changes the showAlt accordingly
	case Event::EventType::ShowAltTile:
	{
		const Event::ShowAltTileEvent &ne = static_cast<const Event::ShowAltTileEvent &>(e);
		// If it is showing it will spawn particles to transition the change
		if(ne.showAlt)
		{
			uint16_t  mLife     = 3;
			uint16_t  spawnRate = 1;
			uint16_t  minLife   = 5;
			uint16_t  maxLife   = 20;
			float     minSize   = 5;
			float     maxSize   = 20;
			uint16_t  groupSize = 10;
			glm::vec4 colour    = {0.3f, 0.3f, 0.3f, 1.0f};

			Direction dir      = revertDirection(rotationToDirection(m_AltRotation));
			float     minSpeed = 0.5f;
			float     maxSpeed = 5.0f;
			m_Level->addSpawner(new ParticleSpawner(x, y, m_Level, mLife, spawnRate, minLife, maxLife, dir, minSpeed, maxSpeed, minSize, maxSize, groupSize, colour));
		}

		showAlt = ne.showAlt;
		return false;   // Returns false as other tiles will need to hear about the event
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
