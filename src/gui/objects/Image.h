#pragma once

#include "MenuObject.h"

class Image : public MenuObject
{
  private:
	uint32_t m_SpriteID;

  public:
	Image(float x, float y, float width, float height, uint32_t spriteID, Layer *layer);
	Image(std::function<void(float *, float *, float *, float *)> posFunc, uint32_t spriteID, Layer *layer);

	virtual void render() override;
	virtual void update() override;
};