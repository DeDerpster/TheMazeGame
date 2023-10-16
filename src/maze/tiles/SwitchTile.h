#pragma once

#include "Tile.h"
#include "Utils.h"

class SwitchTile : public Tile
{
  protected:
	Sprite::ID m_AltSpriteID;
	double   m_AltRotation;
	bool     showAlt;

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

	virtual bool isSolid() override
	{
		return showAlt || m_IsSolid;
	}
};