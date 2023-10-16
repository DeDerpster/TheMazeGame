#pragma once

class VertexBuffer
{
  private:
	unsigned int m_RendererID;
	bool         isDynamic;
	int          m_Offset;
	uint32_t     m_BufferSize;

  public:
	VertexBuffer(const void *data, uint32_t size);
	~VertexBuffer();

	void bind() const;
	void unbind() const;
	void clearBufferData();
	bool addToBuffer(const void *vertices, uint32_t size);

	uint32_t getBufferSize() { return m_BufferSize; }
	bool     canStore(uint32_t size) { return m_Offset + size <= m_BufferSize; }
	bool     isEmpty() { return m_Offset == 0; }
};