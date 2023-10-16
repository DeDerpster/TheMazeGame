#pragma once

class VertexBuffer
{
  private:
	unsigned int m_RendererID;
	bool         isDynamic;

  public:
	VertexBuffer(const void *data, unsigned int size);
	~VertexBuffer();

	void bind() const;
	void unbind() const;
	void clearBufferData();
	void addToBuffer(int offset, int size, const void *data);
};