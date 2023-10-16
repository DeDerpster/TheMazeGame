#pragma once

// This class is for storing and creating the index buffer so that that I do not have to have duplicate vertices in the vertex buffer
class IndexBuffer
{
  private:
	uint32_t m_RendererID;
	uint32_t m_Count;

  public:
	IndexBuffer(const uint32_t *data, uint32_t count);
	IndexBuffer(uint32_t count);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	inline uint32_t getCount() const { return m_Count; }
};