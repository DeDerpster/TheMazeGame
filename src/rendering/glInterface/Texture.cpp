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
	// This is for loading a texture from a file using stb_image
	stbi_set_flip_vertically_on_load(1);   // This will it when loading so positive y goes up the image
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	// Loads the image into the buffer
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));

	// Sets the settings for the image so that it renders correctly
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glGenerateTextureMipmap(m_RendererID));   // Generates a mipmap for the texture

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if(m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);   // Frees the buffer used to load the image

	Log::variable<const std::string &>("Texture Initialised", path);
}

Texture::Texture(uint16_t width, uint16_t height, unsigned char *buffer)
	: m_RendererID(0), m_LocalBuffer(buffer), m_Width(width), m_Height(height), m_BPP(0)
{
	// This will set up textures that are created during the running of the program
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
	// Sets the settings for the image so that it renders correctly
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	GLCall(glGenerateTextureMipmap(m_RendererID));   // Generates a mipmap for the texture
}

Texture::~Texture()
{
	// Delete the texture
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::bind(uint8_t slot) const
{
	if(slot > 31)   // Checks slot is not over the slot limit
	{
		Log::critical("Trying to bind more than 32 textures!", LOGINFO);
	}
	bufferStorage[slot] = this;   // Updates the bufferStorage

	// Binds the texture
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	// Finds where it is bound in the bufferStorage and clears it through a simple linear search
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
	if(!tex)
	{
		Log::warning("Nullptr");
		return 32;
	}
	// Does a simple linear search to find where a texture is bound to
	for(uint8_t i = 0; i < 32; i++)
	{
		if(!bufferStorage[i])
			continue;
		if(bufferStorage[i]->getID() == tex->getID())
			return i;
	}
	return 32;
}

void Texture::clearBufferSlots()
{
	// Sets all the slots to nullptrs
	for(uint8_t i = 0; i < 32; i++)
		bufferStorage[i] = nullptr;
}