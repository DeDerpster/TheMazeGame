#include "Sprite.h"

#include "SpritePaths.h"

#include <string>

std::array<std::unique_ptr<Sprite>, Sprite::ID::numOfSprites> Sprite::sprites;

Sprite::Sprite(ID id)
{
	m_Texture = std::make_unique<Texture>(getPath(id).c_str());   // Creates the texture for the sprite based of id of sprite
}

Sprite::Sprite(const char *texturePath)
{
	m_Texture = std::make_unique<Texture>(texturePath);   // Creates texture based off given path to sprite
}

Sprite::~Sprite()
{
}

void Sprite::bind(uint8_t slot)   // Go between to bind the texture
{
	m_Texture->bind(slot);
}

void Sprite::unbind()   // Unbinds the texture
{
	m_Texture->unbind();
}

void Sprite::init()
{
	// This initialises all the textures from the Sprite::ID data type
	for(ID id = ID::tileBasicWall; id < ID::numOfSprites; ++id)
	{
		sprites[static_cast<int>(id)] = std::make_unique<Sprite>(id);
	}

	Log::info("Sprites have been loaded");
}
