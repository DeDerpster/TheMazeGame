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
		AnimatedSprite()
			: sprites(), index(-1), textureSwapDelay(20), textureSwapCount(0)
		{
		}
		AnimatedSprite(uint16_t frames, uint16_t spriteID)
			: sprites(), index(0), textureSwapDelay(20), textureSwapCount(0)
		{
			for(int i = 0; i < frames; i++)
				sprites.push_back(spriteID + i);
		}
		AnimatedSprite(uint16_t frames, uint16_t spriteID, uint16_t textureSwapDelay)
			: index(0), textureSwapDelay(textureSwapDelay), textureSwapCount(0)
		{
			for(int i = 0; i < frames; i++)
				sprites.push_back(spriteID + i);
		}

		void addSprite(int sprite)
		{
			sprites.push_back(sprite);
			if(index == -1)
				index = 0;
		}

		void update()
		{
			if(textureSwapCount == textureSwapDelay)
			{
				nextFrame();
				textureSwapCount = 0;
			}
			textureSwapCount++;
		}

		void nextFrame()
		{
			if(index != -1)
			{
				index++;
				if(index == sprites.size())
					index = 0;
			}
		}

		void setFrame(int i)
		{
			if(index != -1 && i > -1 && i < sprites.size())
				index = i;
		}

		void render(float x, float y, double rotation, float size)
		{
			Sprite::getSprite(sprites[index])->render(x, y, rotation, size);
		}
	};
}   // namespace Sprite