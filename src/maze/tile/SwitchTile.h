#pragma once

#include "Tile.h"

class SwitchTile : public Tile
{
  protected:
	Sprite::ID m_AltSpriteID;   // Stores the secondary sprite ID
	double     m_AltRotation;   // Stores the secondary rotation for the alternative sprite
	bool       showAlt;         // Stores whether it is showing the alternative sprite

  public:
	SwitchTile();
	SwitchTile(float x, float y, double rotation, Sprite::ID defaultSpriteID, double altRotation, Sprite::ID altSpriteID, bool isSolid, Level *level);
	virtual ~SwitchTile() override;

	virtual void render() override;
	virtual void update() override;
	virtual bool eventCallback(const Event::Event &e) override;
#ifdef DEBUG
	virtual void imGuiRender() override;
#endif

	// Overrides isSolid to include a check to see whether it is showing the alternative sprite
	virtual bool const isSolid() const override
	{
		return showAlt || m_IsSolid;
	}
};