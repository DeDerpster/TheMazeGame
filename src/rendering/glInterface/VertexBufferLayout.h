#pragma once

#include "glDebug.h"
#include <vector>

#include "Renderer.h"

struct VertexBufferElement
{
	unsigned int  type;
	unsigned int  count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch(type)
		{
		case GL_FLOAT:
			return 4;
		case GL_UNSIGNED_INT:
			return 4;
		case GL_UNSIGNED_BYTE:
			return 4;
		default:
			break;
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
  private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int                     m_Stride;

  public:
	VertexBufferLayout()
		: m_Stride(0) {}
	~VertexBufferLayout() {}

	template <typename T>
	void push(unsigned int count);

	inline const std::vector<VertexBufferElement> &GetElements() const { return m_Elements; }
	inline unsigned int                            GetStride() const { return m_Stride; }
};