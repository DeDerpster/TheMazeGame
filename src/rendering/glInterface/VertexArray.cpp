#include "glDebug.h"

#include "VertexArray.h"

#include "Log.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}
VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
{
	// This sets the layout of the buffer, with the given VertexBufferLayout
	bind();
	vb.bind();
	const auto & elements = layout.getElements();
	for(uint32_t i = 0; i < elements.size(); i++)
	{
		// Goes through each element and records the position of it and the type
		const auto &element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.getComponentCount(), shaderDataTypeToOpenGLBaseType(element.type), element.normalized, layout.getStride(), (const void *) element.offset));
	}
	vb.unbind();
}

void VertexArray::bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}
void VertexArray::unbind() const
{
	GLCall(glBindVertexArray(0));
}
