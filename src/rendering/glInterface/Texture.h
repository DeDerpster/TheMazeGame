#pragma once

#include <string>

class Texture
{
  private:
	static const Texture *bufferStorage[32];

	unsigned int   m_RendererID;
	std::string    m_FilePath;
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

	static const Texture *getTextureInBuffer(uint16_t slot) { return bufferStorage[slot]; }
	static uint8_t        getBoundSlot(Texture *tex);
	static void           clearBufferSlots();
};