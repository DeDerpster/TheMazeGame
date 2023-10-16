#pragma once

#include "RenderEffect.h"

#include "Renderer.h"
#include "Shader.h"

namespace Effect
{
	class RenderShaderEffect : public RenderEffect
	{
	  protected:
		std::string &m_Name;

	  public:
		RenderShaderEffect(std::string &name)
			: m_Name(name)
		{
		}
		virtual ~RenderShaderEffect() {}

		std::string        getName() { return m_Name; }
		virtual void       setEffect(Shader &s) const = 0;
		virtual EffectType getType() const override { return EffectType::ShaderEffect; }
	};

	class UniformVec4 : public RenderShaderEffect
	{
	  protected:
		glm::vec4 vec;

	  public:
		UniformVec4(std::string &name, glm::vec4 vec)
			: RenderShaderEffect(name), vec(vec)
		{
		}
		virtual ~UniformVec4() {}
		virtual void setEffect(Shader &s) const override
		{
			s.setUniform4f(m_Name, vec[0], vec[1], vec[2], vec[3]);
		}
	};

	class UniformMat4 : public RenderShaderEffect
	{
	  protected:
		glm::mat4 mat;

	  public:
		UniformMat4(std::string &name, glm::mat4 mat)
			: RenderShaderEffect(name), mat(mat)
		{
		}
		virtual ~UniformMat4() {}
		virtual void setEffect(Shader &s) const override
		{
			s.setUniformMat4f(m_Name, mat);
		}
	};
}   // namespace Effect
