#pragma once

#include "Sprite.h"

#include <vector>

class AnimatedSprite
{
  private:
	std::vector<Sprite::ID> sprites;
	int              index;
	uint16_t         textureSwapDelay, textureSwapCount;

  public:
	AnimatedSprite();
	AnimatedSprite(uint16_t frames, Sprite::ID spriteID);
	AnimatedSprite(uint16_t frames, Sprite::ID spriteID, uint16_t textureSwapDelay);

	void addSprite(Sprite::ID sprite);

	void update();

	void nextFrame();

	void setFrame(int i);

	void render(float x, float y, double rotation, float size, uint8_t layer);
	void render(float x, float y, double rotation, float width, float height, uint8_t layer);
};