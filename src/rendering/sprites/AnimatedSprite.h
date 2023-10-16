#pragma once

#include "Sprite.h"
#include <vector>

namespace Sprite
{
	class AnimatedSprite
	{
	  private:
		std::vector<int> sprites;
		int              index;
		int              textureSwapDelay, textureSwapCount;

	  public:
		AnimatedSprite()
			: sprites(), index(-1), textureSwapDelay(20), textureSwapCount(0)
		{
		}
		AnimatedSprite(int frames)
			: sprites(), index(-1), textureSwapDelay(20), textureSwapCount(0)
		{
			sprites.reserve(frames);
		}
		AnimatedSprite(int frames, int textureSwapDelay)
			: index(-1), textureSwapDelay(textureSwapDelay), textureSwapCount(0)
		{
			sprites.reserve(frames);
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