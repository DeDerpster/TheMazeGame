#include "glDebug.h"

#include "IndexBuffer.h"

#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count)
	: m_Count(count)
{
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::IndexBuffer(unsigned int count)
	: m_Count(count)
{
#ifdef IS_ON_WINDOWS
	if(count > 5292)   // 2646
	{
		Log::error("Index buffer: count is larger than expected!", LOGINFO);
		Log::variable("Size: ", count);
	}
	else if(count < 5292)
		Log::warning("Index buffer: count is smaller than expected");
	unsigned int data[5292];   // TODO: CHANGE ME! with definition
	count = 5292;
#else
	unsigned int data[count];
#endif
	int squares = count / 6;
	for(int i = 0; i < squares; i++)
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
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}