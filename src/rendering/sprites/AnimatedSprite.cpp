#include "AnimatedSprite.h"

#include "Renderer.h"

AnimatedSprite::AnimatedSprite()
	: sprites(), index(-1), textureSwapDelay(10), textureSwapCount(0)
{
}
AnimatedSprite::AnimatedSprite(uint16_t frames, Sprite::ID spriteID)
	: sprites(), index(0), textureSwapDelay(10), textureSwapCount(0)
{
	sprites.reserve(2 * frames);
	for(int i = 1; i <= frames; i++)
	{
		sprites.push_back(spriteID + i);
		sprites.push_back(spriteID);
	}
}
AnimatedSprite::AnimatedSprite(uint16_t frames, Sprite::ID spriteID, uint16_t textureSwapDelay)
	: index(0), textureSwapDelay(textureSwapDelay), textureSwapCount(0)
{
	sprites.reserve(2 * frames);
	for(int i = 1; i <= frames; i++)
	{
		sprites.push_back(spriteID + i);
		sprites.push_back(spriteID);
	}
}

void AnimatedSprite::addSprite(Sprite::ID sprite)
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

void AnimatedSprite::render(float x, float y, double rotation, float size, uint8_t layer)
{
	// Sprite::getSprite(sprites[index])->render(x, y, rotation, size, size);
	Render::sprite(x, y, rotation, size, sprites[index], layer);
}

void AnimatedSprite::render(float x, float y, double rotation, float width, float height, uint8_t layer)
{
	// Sprite::getSprite(sprites[index])->render(x, y, rotation, width, height);
	Render::sprite(x, y, rotation, width, height, sprites[index], layer);
}