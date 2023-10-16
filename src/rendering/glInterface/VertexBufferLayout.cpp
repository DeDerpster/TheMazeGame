#include "VertexBufferLayout.h"

uint32_t BufferElement::getComponentCount() const
{
	switch(type)
	{
	case ShaderDataType::Float:
		return 1;
	case ShaderDataType::Float2:
		return 2;
	case ShaderDataType::Float3:
		return 3;
	case ShaderDataType::Float4:
		return 4;
	case ShaderDataType::Mat3:
		return 3;
	case ShaderDataType::Mat4:
		return 4;
	case ShaderDataType::Int:
		return 1;
	case ShaderDataType::Int2:
		return 2;
	case ShaderDataType::Int3:
		return 3;
	case ShaderDataType::Int4:
		return 4;
	case ShaderDataType::Bool:
		return 1;
	}

	Log::error("Unknown ShaderDataType!", LOGINFO);
	return 0;
}