#include "glDebug.h"

#include "VertexBuffer.h"

#include "Log.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void *data, unsigned int size)
{
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	if(data)
	{
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
		isDynamic = false;
	}
	else
	{
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
		isDynamic = true;
	}
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::clearBufferData()
{
	GLCall(glClearNamedBufferData(m_RendererID, GL_RGBA16, GL_RGBA, GL_UNSIGNED_BYTE, nullptr););
}

void VertexBuffer::addToBuffer(int offset, int size, const void *data)
{
	if(isDynamic)
	{
		GLCall(glNamedBufferSubData(m_RendererID, offset, size, data));
	}
	else
		Log::error("Tried to add data to static buffer", LOGINFO);
}