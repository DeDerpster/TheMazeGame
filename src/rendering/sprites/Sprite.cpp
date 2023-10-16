#include "Sprite.h"

std::array<std::unique_ptr<Sprite>, NUM_OF_SPRITES> Sprite::sprites;

Sprite::Sprite(const char *texturePath)
{
	m_Texture = std::make_unique<Texture>(texturePath);
}

Sprite::~Sprite()
{
}

void Sprite::bind(unsigned int slot)
{
	m_Texture->bind(slot);
}

void Sprite::unbind()
{
	m_Texture->unbind();
}
