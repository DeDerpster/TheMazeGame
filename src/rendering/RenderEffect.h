#pragma once

#include "GLM.h"
#include "Renderer.h"
#include "Shader.h"

#include <string>

namespace Effect
{
	class RenderEffect
	{
	  protected:
		std::string &m_Name;

	  public:
		RenderEffect(std::string &name)
			: m_Name(name)
		{
		}
		virtual void const setEffect(Shader &s) const = 0;
	};

	class UniformVec4 : public RenderEffect
	{
	  protected:
		glm::vec4 vec;

	  public:
		UniformVec4(std::string &name, glm::vec4 vec)
			: RenderEffect(name), vec(vec)
		{
		}
		virtual void const setEffect(Shader &s) const override
		{
			s.SetUniform4f(m_Name, vec[0], vec[1], vec[2], vec[3]);
		}
	};

	class UniformMat4 : public RenderEffect
	{
	  protected:
		glm::mat4 mat;

	  public:
		UniformMat4(std::string &name, glm::mat4 mat)
			: RenderEffect(name), mat(mat)
		{
		}
		virtual void const setEffect(Shader &s) const override
		{
			s.SetUniformMat4f(m_Name, mat);
		}
	};
}   // namespace Effect