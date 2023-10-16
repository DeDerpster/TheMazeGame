#pragma once

#include <string>

// This is for storing and loading textures
class Texture
{
  private:
	// This is a cache for what is currently bound to which slot
	static const Texture *bufferStorage[32];

	uint32_t       m_RendererID;
	std::string    m_FilePath;   // This is for debugging purposes
	unsigned char *m_LocalBuffer;
	int            m_Width, m_Height, m_BPP;

  public:
	Texture(const std::string &path);
	Texture(uint16_t width, uint16_t height, unsigned char *buffer);
	~Texture();

	void bind(uint8_t slot = 0) const;
	void unbind() const;

	inline int getWidth() const { return m_Width; }
	inline int getHeight() const { return m_Height; }
	inline uint32_t getID() const { return m_RendererID; }

	// Functions for interacting with the bufferStorage cache
	static const Texture *getTextureInBuffer(uint8_t slot) { return bufferStorage[slot]; }
	static uint8_t        getBoundSlot(Texture *tex);
	static void           clearBufferSlots();
};