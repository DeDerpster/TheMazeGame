#include "glDebug.h"

#include "Texture.h"

#include "Log.h"
#include "Renderer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

const Texture *Texture::bufferStorage[32];

Texture::Texture(const std::string &path)
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glGenerateTextureMipmap(m_RendererID));

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	Log::variable<const std::string &>("Texture Initialised", path);

	if(m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture::Texture(uint16_t width, uint16_t height, unsigned char *buffer)
	: m_RendererID(0), m_LocalBuffer(buffer), m_Width(width), m_Height(height), m_BPP(0)
{
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RED,
		m_Width,
		m_Height,
		0,
		GL_RED,
		GL_UNSIGNED_BYTE,
		m_LocalBuffer);
	// set texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::bind(uint8_t slot) const
{
	if(slot > 31)
	{
		Log::critical("Trying to bind more than 32 textures!", LOGINFO);
	}
	bufferStorage[slot] = this;
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	for(uint8_t i = 0; i < 32; i++)
	{
		if(bufferStorage[i] == this)
		{
			bufferStorage[i] = nullptr;
			break;
		}
	}
}

uint8_t Texture::getBoundSlot(Texture *tex)
{
	for(uint8_t i = 0; i < 32; i++)
	{
		if(bufferStorage[i] == tex)
			return i;
	}
	return 32;
}

void Texture::clearBufferSlots()
{
	for(uint8_t i = 0; i < 32; i++)
		bufferStorage[i] = nullptr;
}