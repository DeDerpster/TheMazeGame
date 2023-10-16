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
	Renderer::Renderer(unsigned int maxVertices)
		: m_Offset(0), maxVertices(maxVertices)
	{
		// Creates the VAO and links it to the vertex buffer
		m_VAO = std::make_unique<VertexArray>();
		m_VAO->bind();
		m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, sizeof(Vertex) * maxVertices);
		m_BufferSize   = sizeof(Vertex) * maxVertices;

		// Creates the layout of variables being pushed to the shader
		VertexBufferLayout layout;   // NOTE: Must change this when I change the shaders
		layout.Push<float>(2);
		layout.Push<float>(2);
		layout.Push<float>(1);
		m_VAO->AddBuffer(*m_VertexBuffer, layout);   // Adds it to the VAO

		// Initialises the index buffer
		m_IndexBuffer = std::make_unique<IndexBuffer>((maxVertices / 4) * 6);
		m_IndexBuffer->bind();

		// Unbinds everything
		m_VAO->unbind();
		m_IndexBuffer->unbind();
		Log::info("Renderer created");
	}

	Renderer::~Renderer()
	{
		Log::info("Renderer destroyed");
	}

	// SECTION: Rendering
	void Renderer::clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT);   // Resets the pixels on the screens
	}

	void Renderer::draw() const
	{
		// Binds what this renderer is using for vertices
		m_VAO->bind();
		m_IndexBuffer->bind();
		GLCall(glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr));
	}

	void Renderer::render()   // This goes through all the sprite's buffers and systematically renders them
	{
		unsigned int currentTexID = 0;   // This stores the slot the current texture is bound to, so it can set the texID part of the vertex
		for(int i = 0; i < NUM_OF_SPRITES; i++)
		{
			// Gets the buffer from the sprite
			std::vector<Sprite::Object> *buffer = Sprite::Sprite::getSprite(i)->getBuffer();
			if(buffer->size() == 0)   // Checks the buffer is not empty
				continue;

			// Binds the sprite texture to then currentTexID
			Sprite::Sprite::getSprite(i)->bind(currentTexID);

			// Goes through all the objects in the buffer and renders them
			for(Sprite::Object obj : *buffer)
			{
				// Checks if the buffer is full or the buffer is too big and draws what there is
				if(m_Offset + 4 * sizeof(Vertex) > m_BufferSize || currentTexID == 32)
				{
					draw();
					resetBuffer();      // Resets the buffer so it can draw again
					currentTexID = 0;   // resets this as all the textures have been rendered

					// Rebinds the texture so it is in the correct slot
					Sprite::Sprite::getSprite(i)->unbind();
					Sprite::Sprite::getSprite(i)->bind(0);
				}
				// Adds the current object to the buffer by creating its quad (this is for memory efficiency)
				addToBuffer(CreateQuad(obj.x, obj.y, obj.rotation, obj.size, currentTexID));
			}
			// Clears the buffer of the sprite as it will all be rendered
			Sprite::Sprite::getSprite(i)->clearBuffer();
			currentTexID++;
		}

		if(m_Offset != 0)   // If the buffer is not empty, it empties it
		{
			draw();
			resetBuffer();
		}
	}
	// !SECTION

	// SECTION: Buffers
	void Renderer::addToBuffer(const std::array<Vertex, 4> &vertices)   // This adds an array of vertices to the buffer
	{
		// Checks if the buffer is full and if it is it reports the error
		if(m_Offset + vertices.size() * sizeof(Vertex) > m_BufferSize)
			Log::error("Render buffer full!", LOGINFO);
		else
		{
			// Adds to the vertex buffer
			m_VertexBuffer->addToBuffer(m_Offset, vertices.size() * sizeof(Vertex), (void *) &vertices);

			m_Offset += vertices.size() * sizeof(Vertex);   // Adds to the m_Offset so it correctly positions the next vertices
		}
	}

	void Renderer::resetBuffer()
	{
		// Clears the buffer data
		m_VertexBuffer->clearBufferData();
		m_Offset = 0;   // Resets the offset
	}
	// !SECTION

}   // namespace Render
