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
		m_VAO = std::make_unique<VertexArray>();
		m_VAO->bind();
		m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, sizeof(Vertex) * maxVertices);
		m_BufferSize   = sizeof(Vertex) * maxVertices;

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		layout.Push<float>(1);
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>((maxVertices / 4) * 6);
		m_IndexBuffer->bind();

		m_VAO->unbind();
		m_IndexBuffer->unbind();
		Log::info("Renderer created");
	}

	Renderer::~Renderer()
	{
		Log::info("Renderer destroyed");
	}

	void Renderer::clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::draw() const
	{
		m_VAO->bind();
		m_IndexBuffer->bind();
		GLCall(glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr));
	}

	void Renderer::render()
	{
		unsigned int currentTexID = 0;
		for(int i = 0; i < NUM_OF_SPRITES; i++)
		{
			std::vector<Sprite::Object> *buffer = Sprite::Sprite::getSprite(i)->getBuffer();
			if(buffer->size() == 0)
				continue;
			Sprite::Sprite::getSprite(i)->bind(currentTexID);
			for(Sprite::Object obj : *buffer)
			{
				if(m_Offset + 4 * sizeof(Vertex) > m_BufferSize || currentTexID == 32)
				{
					draw();
					resetBuffer();
					currentTexID = 0;
					Sprite::Sprite::getSprite(i)->unbind();
					Sprite::Sprite::getSprite(i)->bind(0);
				}
				addToBuffer(CreateQuad(obj.x, obj.y, obj.rotation, obj.size, currentTexID));
			}
			Sprite::Sprite::getSprite(i)->clearBuffer();
			currentTexID++;
		}
		draw();
		resetBuffer();
	}

	void Renderer::addToBuffer(const std::array<Vertex, 4> &vertices)
	{
		if(m_Offset + vertices.size() * sizeof(Vertex) > m_BufferSize)
			Log::error("Render buffer full!", LOGINFO);
		else
		{
			Vertex positions[4];
			memcpy(positions, vertices.data(), vertices.size() * sizeof(Vertex));

			m_VertexBuffer->addToBuffer(m_Offset, sizeof(positions), positions);

			m_Offset += sizeof(positions);
		}
	}

	void Renderer::resetBuffer()
	{
		m_VertexBuffer->clearBufferData();
		m_Offset = 0;
	}

}   // namespace Render
