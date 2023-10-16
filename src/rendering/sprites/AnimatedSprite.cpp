#include "AnimatedSprite.h"

namespace Render
{
	AnimatedSprite::AnimatedSprite()
		: sprites(), index(-1), textureSwapDelay(20), textureSwapCount(0)
	{
	}
	AnimatedSprite::AnimatedSprite(uint16_t frames, uint16_t spriteID)
		: sprites(), index(0), textureSwapDelay(20), textureSwapCount(0)
	{
		for(int i = 1; i <= frames; i++)
			sprites.push_back(spriteID + i);
	}
	AnimatedSprite::AnimatedSprite(uint16_t frames, uint16_t spriteID, uint16_t textureSwapDelay)
		: index(0), textureSwapDelay(textureSwapDelay), textureSwapCount(0)
	{
		for(int i = 1; i <= frames; i++)
			sprites.push_back(spriteID + i);
	}

	void AnimatedSprite::addSprite(int sprite)
	{
		sprites.push_back(sprite);
		if(index == -1)
			index = 0;
	}

	void AnimatedSprite::update()
	{
		if(textureSwapCount == textureSwapDelay)
		{
			nextFrame();
			textureSwapCount = 0;
		}
		textureSwapCount++;
	}

	void AnimatedSprite::nextFrame()
	{
		if(index != -1)
		{
			index++;
			if(index == sprites.size())
				index = 0;
		}
	}

	void AnimatedSprite::setFrame(int i)
	{
		if(index != -1 && i > -1 && i < sprites.size())
			index = i;
	}

	void AnimatedSprite::render(float x, float y, double rotation, float size)
	{
		Sprite::getSprite(sprites[index])->render(x, y, rotation, size);
	}
}   // namespace Render