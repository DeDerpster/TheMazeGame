#pragma once

#include <array>
#include <memory>
#include <vector>

#include "IndexBuffer.h"
#include "Texture.h"
#include "Utils.h"
#include "VertexArray.h"

namespace Render   // Puts this in its own namespace
{
	class SmartBuffer
	{
	  private:
		int      m_Offset;
		uint32_t m_BufferSize;
		uint32_t maxVertices;

		std::unique_ptr<VertexArray>  m_VAO;
		std::unique_ptr<IndexBuffer>  m_IndexBuffer;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;

	  public:
		SmartBuffer(uint32_t maxVertices, void (*layoutCreator)(VertexBufferLayout &));
		~SmartBuffer();

		bool addToBuffer(const void *vertices, uint32_t size);
		void resetBuffer();
		void draw() const;

		uint32_t getBufferSize() { return m_BufferSize; }
		bool     canStore(uint32_t size) { return m_Offset + size <= m_BufferSize; }
		bool     isEmpty() { return m_Offset == 0; }
	};

	// This creates a quad of vertices (a square) from given coordinates and rotation...
	static std::array<Vertex, 4> CreateQuad(float x, float y, double rotation, float size, int texID)
	{
		// Creates a 2d rotation matrix, so that the object can be rotated
		glm::mat2 rotationMatrix({glm::cos(rotation), -glm::sin(rotation)}, {glm::sin(rotation), glm::cos(rotation)});

		// This centers the object
		float halfSize = size / 2;

		// Creates 4 vertices that create the square
		Vertex v0(
			rotationMatrix * glm::vec2(-halfSize, -halfSize) + glm::vec2(x, y),
			{0.0f, 0.0f},
			texID);

		Vertex v1(
			rotationMatrix * glm::vec2(halfSize, -halfSize) + glm::vec2(x, y),
			{1.0f, 0.0f},
			texID);

		Vertex v2(
			rotationMatrix * glm::vec2(halfSize, halfSize) + glm::vec2(x, y),
			{1.0f, 1.0f},
			texID);

		Vertex v3(
			rotationMatrix * glm::vec2(-halfSize, halfSize) + glm::vec2(x, y),
			{0.0f, 1.0f},
			texID);

		return {v0, v1, v2, v3};
	}

	static std::array<glm::vec2, 4> CreateVecQuad(float x, float y, double rotation, float size)
	{
		// Creates a 2d rotation matrix, so that the object can be rotated
		glm::mat2 rotationMatrix({glm::cos(rotation), -glm::sin(rotation)}, {glm::sin(rotation), glm::cos(rotation)});

		// This centers the object
		float halfSize = size / 2;

		// Creates 4 vertices that create the square
		glm::vec2 v0 = rotationMatrix * glm::vec2(-halfSize, -halfSize) + glm::vec2(x, y);
		glm::vec2 v1 = rotationMatrix * glm::vec2(halfSize, -halfSize) + glm::vec2(x, y);
		glm::vec2 v2 = rotationMatrix * glm::vec2(halfSize, halfSize) + glm::vec2(x, y);
		glm::vec2 v3 = rotationMatrix * glm::vec2(-halfSize, halfSize) + glm::vec2(x, y);

		return {v0, v1, v2, v3};
	}
}   // namespace Render
