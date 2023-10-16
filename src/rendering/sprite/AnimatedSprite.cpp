#include "AnimatedSprite.h"

#include "rendering/Renderer.h"

AnimatedSprite::AnimatedSprite()
	: sprites(), index(-1), textureSwapDelay(10), textureSwapCount(0)
{
}
AnimatedSprite::AnimatedSprite(uint16_t frames, Sprite::ID spriteID)
	: sprites(), index(0), textureSwapDelay(10), textureSwapCount(0)
{
	// Generates the sprites for walking (as it will continue to go back to the main one after every frame)
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
	// Generates the sprites for walking (as it will continue to go back to the main one after every frame)
	sprites.reserve(2 * frames);
	for(int i = 1; i <= frames; i++)
	{
		sprites.push_back(spriteID + i);
		sprites.push_back(spriteID);
	}
}

void AnimatedSprite::addSprite(Sprite::ID sprite)
{
	// Adds a frame to the animation
	sprites.push_back(sprite);
	if(index == -1)
		index = 0;
}

void AnimatedSprite::update()
{
	// This increases the swap count and if it reaches the delay it will go to the next frame
	if(textureSwapCount == textureSwapDelay)
	{
		nextFrame();
		textureSwapCount = 0;
	}
	textureSwapCount++;
}

void AnimatedSprite::nextFrame()
{
	// This increments the index to the next frame and loops back to 0 if it is triggered
	if(index != -1)
	{
		index++;
		if(index == sprites.size())
			index = 0;
	}
}

void AnimatedSprite::setFrame(int i)
{
	// Sets the frame to a specific value
	if(index != -1 && i > -1 && i < sprites.size())
		index = i;
}

// These two functions render the current active sprite
void AnimatedSprite::render(float x, float y, double rotation, float size, uint8_t layer)
{
	Render::sprite(x, y, rotation, size, sprites[index], layer);
}

void AnimatedSprite::render(float x, float y, double rotation, float width, float height, uint8_t layer)
{
	Render::sprite(x, y, rotation, width, height, sprites[index], layer);
}