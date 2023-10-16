#include "glDebug.h"

#include "Renderer.h"

#include "Log.h"
#include "VertexBufferLayout.h"

#include "Application.h"
#include "Sprite.h"

#include <cstring>
#include <sstream>

namespace Render
{

	// SECTION: SmartBuffer
	SmartBuffer::SmartBuffer(uint32_t maxVertices, void (*layoutCreator)(VertexBufferLayout &))
		: m_Offset(0), maxVertices(maxVertices)
	{
		// Creates the VAO and links it to the vertex buffer
		m_VAO = std::make_unique<VertexArray>();
		m_VAO->bind();
		m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, sizeof(Vertex) * maxVertices);
		m_BufferSize   = sizeof(Vertex) * maxVertices;

		// Creates the layout of variables being pushed to the shader
		VertexBufferLayout layout;   // NOTE: Must change this when I change the shaders
		(*layoutCreator)(layout);
		m_VAO->addBuffer(*m_VertexBuffer, layout);   // Adds it to the VAO

		// Initialises the index buffer
		m_IndexBuffer = std::make_unique<IndexBuffer>((maxVertices / 4) * 6);
		m_IndexBuffer->bind();

		// Unbinds everything
		m_VAO->unbind();
		m_IndexBuffer->unbind();
		Log::info("Smart Buffer created");
	}

	SmartBuffer::~SmartBuffer()
	{
		Log::info("Smart buffer destroyed");
	}

	// SECTION: updating Buffer
	bool SmartBuffer::addToBuffer(const void *vertices, uint32_t size)
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
			m_VertexBuffer->addToBuffer(m_Offset, size, vertices);

			m_Offset += size;   // Adds to the m_Offset so it correctly positions the next vertices
			return true;
		}
	}   // namespace Render

	void SmartBuffer::resetBuffer()
	{
		// Clears the buffer data
		m_VertexBuffer->clearBufferData();
		m_Offset = 0;   // Resets the offset
	}

	void SmartBuffer::draw() const
	{
		// Binds what this renderer is using for vertices
		m_VAO->bind();
		m_IndexBuffer->bind();
		GLCall(glDrawElements(GL_TRIANGLES, m_IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr));
	}
	// !SECTION
	// !SECTION
}   // namespace Render
