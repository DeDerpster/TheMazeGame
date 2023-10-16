#include "glDebug.h"

#include "VertexBuffer.h"

#include "Log.h"

VertexBuffer::VertexBuffer(const void *data, uint32_t size)
	: m_Offset(0), m_BufferSize(size)
{
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	if(data)
		Log::error("Tried to create a static buffer!", LOGINFO);
	else
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
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
	m_Offset = 0;   // Resets the offset
}

bool VertexBuffer::addToBuffer(const void *vertices, uint32_t size)
{
	// Checks if the buffer is full and if it is it reports the error
	if(!canStore(size))
	{
		Log::error("Render buffer full!", LOGINFO);
		return false;
	}
	else
	{
		// Adds to the vertex buffer
		GLCall(glNamedBufferSubData(m_RendererID, m_Offset, size, vertices));

		m_Offset += size;   // Adds to the m_Offset so it correctly positions the next vertices
		return true;
	}
}
