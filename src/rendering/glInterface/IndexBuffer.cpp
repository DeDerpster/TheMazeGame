#include "glDebug.h"

#include "IndexBuffer.h"

#include "Renderer.h"

IndexBuffer::IndexBuffer(const uint32_t *data, uint32_t count)
	: m_Count(count)
{
	// This will generate the buffer and add the data
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW));
}

IndexBuffer::IndexBuffer(uint32_t count)
	: m_Count(count)
{
	// This generates a default for the data with a given count of vertices
	uint32_t *data = new uint32_t[count];

	uint32_t squares = count / 6;
	for(uint32_t i = 0; i < squares; i++)
	{
		data[i * 6]     = (i * 4);
		data[i * 6 + 1] = (i * 4) + 1;
		data[i * 6 + 2] = (i * 4) + 2;
		data[i * 6 + 3] = (i * 4) + 2;
		data[i * 6 + 4] = (i * 4) + 3;
		data[i * 6 + 5] = (i * 4);
	}
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(uint32_t), data, GL_STATIC_DRAW));

	delete[] data;   // Makes sure to delete the data
}

IndexBuffer::~IndexBuffer()
{
	// Deletes the buffer
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::bind() const
{
	// Binds the buffer
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::unbind() const
{
	// Unbinds the buffer
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}