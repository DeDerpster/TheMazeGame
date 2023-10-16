#pragma once

#include <memory>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace Render
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
}   // namespace Render