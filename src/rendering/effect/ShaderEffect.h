#pragma once

#include "RenderEffect.h"

#include "Log.h"
#include "Renderer.h"
#include "Shader.h"

namespace Effect
{
	// These are for all the different shader effects that could be applied
	class ShaderEffectCarrier : public RenderEffect
	{
	  protected:
		uint16_t m_ID;

	  public:
		ShaderEffectCarrier(uint16_t id)
			: m_ID(id)
		{
		}
		virtual ~ShaderEffectCarrier() {}

		uint16_t           getID() { return m_ID; }
		virtual EffectType getType() const override { return EffectType::shaderEffect; }
	};

	// This is for removing an effect from a layer
	class RemoveShaderEffect : public ShaderEffectCarrier
	{
	  public:
		RemoveShaderEffect(uint16_t id)
			: ShaderEffectCarrier(id)
		{
		}
		virtual ~RemoveShaderEffect() {}

		virtual EffectType getType() const override { return EffectType::removeShaderEffect; }
	};

	class RenderShaderEffect
	{
	  protected:
		const std::string m_Name;

	  public:
		RenderShaderEffect(const std::string &name)
			: m_Name(name)
		{
		}
		virtual ~RenderShaderEffect() {}

		std::string        getName() { return m_Name; }
		virtual void       setEffect(Shader &s) const = 0;
	};

	class UniformVec4 : public RenderShaderEffect
	{
	  protected:
		glm::vec4 vec;

	  public:
		UniformVec4(const std::string &name, glm::vec4 vec)
			: RenderShaderEffect(name), vec(vec)
		{
		}
		virtual ~UniformVec4() {}

		virtual void setEffect(Shader &s) const override
		{
			s.setUniform4f(m_Name, vec[0], vec[1], vec[2], vec[3]);
		}

		void setVec(glm::vec4 newVec) { vec = newVec; }
	};

	class UniformMat4 : public RenderShaderEffect
	{
	  protected:
		glm::mat4 mat;

	  public:
		UniformMat4(const std::string &name, glm::mat4 mat)
			: RenderShaderEffect(name), mat(mat)
		{
		}
		virtual ~UniformMat4() {}

		virtual void setEffect(Shader &s) const override
		{
			s.setUniformMat4f(m_Name, mat);
		}
		void setMat(glm::mat4 newMat) { mat = newMat; }
	};

	// This is the class for managing the shader effects, their IDs and storing and sending the effects that are carried
	class ShaderEffects
	{
	  public:
		ShaderEffects(const ShaderEffects &) = delete;
		~ShaderEffects();

		static uint16_t            sendShaderEffect(const std::string &s, glm::vec4 vec, bool includeOverlay = false) { return get().sendShaderEffectImpl(s, vec, includeOverlay); }
		static uint16_t            sendShaderEffect(const std::string &s, glm::mat4 mat, bool includeOverlay = false) { return get().sendShaderEffectImpl(s, mat, includeOverlay); }
		static uint16_t            sendOverlayEffect(const std::string &s, glm::vec4 vec) { return get().sendOverlayEffectImpl(s, vec); }
		static uint16_t            sendOverlayEffect(const std::string &s, glm::mat4 mat) { return get().sendOverlayEffectImpl(s, mat); }
		static void                deleteShaderEffect(uint16_t id) { get().deleteShaderEffectImpl(id); }
		static RenderShaderEffect *getShaderEffect(uint16_t id) { return get().getShaderEffectImpl(id); }

		static uint16_t findShaderEffect(const std::string &s) { return get().findShaderEffectImpl(s); }

		static ShaderEffects &get()
		{
			static ShaderEffects s_Instance;
			return s_Instance;
		}

	  private:
		static ShaderEffects              s_Instance;
		std::vector<RenderShaderEffect *> m_Effects;

		ShaderEffects();

		uint16_t            sendShaderEffectImpl(const std::string &s, glm::vec4 vec, bool includeOverlay);
		uint16_t            sendShaderEffectImpl(const std::string &s, glm::mat4 mat, bool includeOverlay);
		uint16_t            sendOverlayEffectImpl(const std::string &s, glm::vec4 vec);
		uint16_t            sendOverlayEffectImpl(const std::string &s, glm::mat4 mat);
		void                deleteShaderEffectImpl(uint16_t id);
		RenderShaderEffect *getShaderEffectImpl(uint16_t id);
		uint16_t            findShaderEffectImpl(const std::string &s);
	};
}   // namespace Effect
