#include "Sprite.h"

namespace Render
{
	std::vector<std::unique_ptr<Sprite>> Sprite::sprites;

	Sprite::Sprite(const char *texturePath)
	{
		m_Texture = std::make_unique<Texture>(texturePath);
		// m_SpriteID   = -1;
	}

	Sprite::~Sprite()
	{
	}

	void Sprite::render(float x, float y, double rotation, float size)
	{
		m_Buffer.push_back({x, y, rotation, size});
	}

	void Sprite::bind(unsigned int slot)
	{
		m_Texture->bind(slot);
	}

	void Sprite::unbind()
	{
		m_Texture->unbind();
	}

}   // namespace Sprite