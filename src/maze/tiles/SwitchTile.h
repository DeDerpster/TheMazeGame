#pragma once

#include "Tile.h"
#include "Utils.h"

class SwitchTile : public Tile
{
  protected:
	uint32_t m_AltSpriteID;
	double   m_AltRotation;
	bool     showAlt;

  public:
	SwitchTile();
	SwitchTile(float x, float y, double rotation, uint32_t defaultSpriteID, double altRotation, uint32_t altSpriteID, bool isSolid, Level *level);
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