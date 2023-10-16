#pragma once

#include <array>
#include <memory>
#include <vector>

#include "IndexBuffer.h"
// #include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"

namespace Render
{
	struct Vec2
	{
		float x, y;
	};

	struct Vertex
	{
		glm::vec2 position;
		Vec2      texCoords;
		float     texID;
	};

	static std::array<Vertex, 4> CreateQuad(float x, float y, double rotation, float size, int texID)
	{
		glm::mat2 rotationMatrix({glm::cos(rotation), -glm::sin(rotation)}, {glm::sin(rotation), glm::cos(rotation)});
		float     halfSize = size / 2;

		glm::vec2(x - halfSize, y - halfSize);

		Vertex v0;
		// glm::vec2 vec = rotationMatrix * glm::vec2(-halfSize, -halfSize) + glm::vec2(x, y);
		v0.position = rotationMatrix * glm::vec2(-halfSize, -halfSize) + glm::vec2(x, y);
		//v0.position  = {x - halfSize, y - halfSize};
		v0.texCoords = {0.0f, 0.0f};
		v0.texID     = texID;
		Vertex v1;
		// glm::vec2 vec1 = rotationMatrix * glm::vec2(halfSize, -halfSize) + glm::vec2(x, y);
		v1.position = rotationMatrix * glm::vec2(halfSize, -halfSize) + glm::vec2(x, y);
		//v1.position  = {x + halfSize, y - halfSize};
		v1.texCoords = {1.0f, 0.0f};
		v1.texID     = texID;
		Vertex v2;
		// glm::vec2 vec2 = rotationMatrix * glm::vec2(halfSize, halfSize) + glm::vec2(x, y);
		v2.position = rotationMatrix * glm::vec2(halfSize, halfSize) + glm::vec2(x, y);
		// v2.position  = {x + halfSize, y + halfSize};
		v2.texCoords = {1.0f, 1.0f};
		v2.texID     = texID;
		Vertex v3;
		//glm::vec2 vec3 = rotationMatrix * glm::vec2(-halfSize, halfSize) + glm::vec2(x, y);
		v3.position = rotationMatrix * glm::vec2(-halfSize, halfSize) + glm::vec2(x, y);
		// v3.position  = {x - halfSize, y + halfSize};
		v3.texCoords = {0.0f, 1.0f};
		v3.texID     = texID;

		return {v0, v1, v2, v3};
	}

	class Renderer
	{
	  private:
		int                           m_Offset;
		unsigned int                  m_BufferSize;
		unsigned int                  maxVertices;
		std::unique_ptr<VertexArray>  m_VAO;
		std::unique_ptr<IndexBuffer>  m_IndexBuffer;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;

	  public:
		Renderer(unsigned int maxVertices);
		~Renderer();
		void clear() const;
		void draw() const;
		void render();
		void addToBuffer(const std::array<Vertex, 4> &vertices);
		void resetBuffer();
	};
}   // namespace Render
