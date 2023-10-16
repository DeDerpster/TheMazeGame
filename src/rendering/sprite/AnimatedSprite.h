#pragma once

#include "Sprite.h"

#include <vector>

class AnimatedSprite
{
  private:
	std::vector<Sprite::ID> sprites;                              // Stores a list of sprite IDs for the animation
	int                     index;                                // Stores the index of the current sprite
	uint16_t                textureSwapDelay, textureSwapCount;   // Stores the info needed to swap between frames

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