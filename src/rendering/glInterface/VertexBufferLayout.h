#pragma once

#include "glDebug.h"
#include <vector>

#include "Log.h"
#include "Renderer.h"

enum class ShaderDataType
{
	None = 0,
	Float,
	Float2,
	Float3,
	Float4,
	Mat3,
	Mat4,
	Int,
	Int2,
	Int3,
	Int4,
	Bool
};

static uint32_t shaderDataTypeSize(ShaderDataType type)
{
	switch(type)
	{
	case ShaderDataType::Float:
		return 4;
	case ShaderDataType::Float2:
		return 4 * 2;
	case ShaderDataType::Float3:
		return 4 * 3;
	case ShaderDataType::Float4:
		return 4 * 4;
	case ShaderDataType::Mat3:
		return 4 * 3 * 3;
	case ShaderDataType::Mat4:
		return 4 * 4 * 4;
	case ShaderDataType::Int:
		return 4;
	case ShaderDataType::Int2:
		return 4 * 2;
	case ShaderDataType::Int3:
		return 4 * 3;
	case ShaderDataType::Int4:
		return 4 * 4;
	case ShaderDataType::Bool:
		return 1;
	}

	Log::error("Unknown ShaderDataType!", LOGINFO);

	return 0;
}

static GLenum shaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
	switch(type)
	{
	case ShaderDataType::Float:
		return GL_FLOAT;
	case ShaderDataType::Float2:
		return GL_FLOAT;
	case ShaderDataType::Float3:
		return GL_FLOAT;
	case ShaderDataType::Float4:
		return GL_FLOAT;
	case ShaderDataType::Mat3:
		return GL_FLOAT;
	case ShaderDataType::Mat4:
		return GL_FLOAT;
	case ShaderDataType::Int:
		return GL_INT;
	case ShaderDataType::Int2:
		return GL_INT;
	case ShaderDataType::Int3:
		return GL_INT;
	case ShaderDataType::Int4:
		return GL_INT;
	case ShaderDataType::Bool:
		return GL_BOOL;
	}

	Log::error("Unknown ShaderDataType!", LOGINFO);
	return 0;
}

struct BufferElement
{
	std::string    name;
	ShaderDataType type;
	uint32_t       size;
	size_t         offset;
	bool           normalized;

	BufferElement() = default;

	BufferElement(ShaderDataType type, const std::string &name, bool normalized = false)
		: name(name), type(type), size(shaderDataTypeSize(type)), offset(0), normalized(normalized)
	{
	}

	uint32_t getComponentCount() const;
};

class VertexBufferLayout
{
  private:
	std::vector<BufferElement> m_Elements;
	uint32_t                   m_Stride = 0;

	void calculateOffsetsAndStride()
	{
		size_t offset = 0;
		m_Stride      = 0;
		for(auto &element : m_Elements)
		{
			element.offset = offset;
			offset += element.size;
			m_Stride += element.size;
		}
	}

  public:
	VertexBufferLayout() {}

	VertexBufferLayout(std::initializer_list<BufferElement> elements)
		: m_Elements(elements)
	{
		calculateOffsetsAndStride();
	}

	uint32_t                          getStride() const { return m_Stride; }
	const std::vector<BufferElement> &getElements() const { return m_Elements; }

	std::vector<BufferElement>::iterator       begin() { return m_Elements.begin(); }
	std::vector<BufferElement>::iterator       end() { return m_Elements.end(); }
	std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
	std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
};