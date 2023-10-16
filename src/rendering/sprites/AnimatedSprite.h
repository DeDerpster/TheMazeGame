#pragma once

#include "Sprite.h"
#include <vector>

namespace Render
{
	class AnimatedSprite
	{
	  private:
		std::vector<int> sprites;
		int              index;
		uint16_t         textureSwapDelay, textureSwapCount;

	  public:
		AnimatedSprite();
		AnimatedSprite(uint16_t frames, uint16_t spriteID);
		AnimatedSprite(uint16_t frames, uint16_t spriteID, uint16_t textureSwapDelay);

		void addSprite(int sprite);

		void update();

		void nextFrame();

		void setFrame(int i);

		void render(float x, float y, double rotation, float size);
	};
}   // namespace Sprite