#include "Sprite.h"

#include "SpritePaths.h"

#include <string>

std::array<std::unique_ptr<Sprite>, Sprite::ID::numOfSprites> Sprite::sprites;

Sprite::Sprite(ID id)
{
	m_Texture = std::make_unique<Texture>(getPath(id).c_str());
}

Sprite::Sprite(const char *texturePath)
{
	m_Texture = std::make_unique<Texture>(texturePath);
}

Sprite::~Sprite()
{
}

void Sprite::bind(uint8_t slot)
{
	m_Texture->bind(slot);
}

void Sprite::unbind()
{
	m_Texture->unbind();
}

void Sprite::init()
{
	for(ID id = ID::tileBasicWall; id < ID::numOfSprites; ++id)
	{
		sprites[static_cast<int>(id)] = std::make_unique<Sprite>(id);
	}

	Log::info("Sprites have been loaded");
}
