#include "Image.h"

#include "rendering/Renderer.h"

Image::Image(float x, float y, float width, float height, Sprite::ID spriteID, Layer *layer)
	: MenuObject(x, y, width, height, layer), m_SpriteID(spriteID)
{
}

Image::Image(std::function<void(float *, float *, float *, float *)> posFunc, Sprite::ID spriteID, Layer *layer)
	: MenuObject(posFunc, layer), m_SpriteID(spriteID)
{
}

void Image::render()
{
	Render::sprite(x, y, 0.0f, width, height, m_SpriteID, 9, true);
}

void Image::update()
{
}