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
	class Renderer
	{
	  private:
		int          m_Offset;       // Stores the offset to the end of the used buffer
		unsigned int m_BufferSize;   // Stores the size of the buffer
		unsigned int maxVertices;    // Stores the maximum amount of vertices the renderer can store

		// These are the variables needed to render
		std::unique_ptr<VertexArray>  m_VAO;
		std::unique_ptr<IndexBuffer>  m_IndexBuffer;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;

		void addToBuffer(const std::array<Vertex, 4> &vertices);   // Adds an array of 4 vertices to the buffer (a square)
		void draw() const;                                         // This draws all elements in the vertex buffer - but does NOT reset it
		void resetBuffer();                                        // Resets the buffer, so that the buffer does not overflow

	  public:
		Renderer(unsigned int maxVertices);
		~Renderer();

		void clear() const;   // Resets the window
		void render();        // This goes through all the sprites and renders anything added to their buffer
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
}   // namespace Render
