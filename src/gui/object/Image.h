#pragma once

#include "MenuObject.h"

#include "rendering/sprite/Sprite.h"

class Image : public MenuObject
{
  private:   // Stores the image's sprite id
	Sprite::ID m_SpriteID;

  public:
	Image(float x, float y, float width, float height, Sprite::ID spriteID, Layer *layer);
	Image(std::function<void(float *, float *, float *, float *)> posFunc, Sprite::ID spriteID, Layer *layer);

	virtual void render() override;
	virtual void update() override;
};