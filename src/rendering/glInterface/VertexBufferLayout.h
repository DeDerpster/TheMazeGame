#pragma once

#include "glDebug.h"
#include <vector>

#include "Log.h"

// These are for differentiating between the different types
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

// This will convert each type to the corresponding openGL type
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

// This stores all the information needed about a single element
struct BufferElement
{
	std::string    name;
	ShaderDataType type;
	uint32_t       size;
	size_t         offset;
	bool           normalized;

	BufferElement() = default;

	BufferElement(ShaderDataType type, const std::string &name, bool normalized = false);

	uint32_t getComponentCount() const;
};

// This will store a list of buffer elements so that they can be accessed and set correctly
class VertexBufferLayout
{
  private:
	std::vector<BufferElement> m_Elements;
	uint32_t                   m_Stride = 0;

	void calculateOffsetsAndStride()
	{
		// Calculates and sets the offsets for each element
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

	// Function to allow interaction with the vector without using 'getElements'
	std::vector<BufferElement>::iterator       begin() { return m_Elements.begin(); }
	std::vector<BufferElement>::iterator       end() { return m_Elements.end(); }
	std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
	std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
};