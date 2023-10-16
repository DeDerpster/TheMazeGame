#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

// This is for compining vertex buffer, index buffer and vertex buffer layout together
class VertexArray
{
  private:
	uint32_t m_RendererID;

  public:
	VertexArray();
	~VertexArray();

	void addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);

	void bind() const;
	void unbind() const;
};