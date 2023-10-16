#pragma once

#include <string>

#include "Log.h"
#include "Shader.h"
#include "ShaderEffectCarrier.h"

namespace Effect
{
	class ShaderEffect
	{
	  public:
		enum class Type
		{
			normal,
			includeOverlay,
			onlyOverlay
		};

	  protected:
		const std::string m_Name;

		bool simpleShader, spriteShader, textShader;

		Type m_Type;

	  public:
		ShaderEffect(const std::string &name, Type type, bool simpleShader, bool spriteShader, bool textShader)
			: m_Name(name), m_Type(type), simpleShader(simpleShader), spriteShader(spriteShader), textShader(textShader)
		{
		}
		virtual ~ShaderEffect() {}

		std::string  getName() { return m_Name; }
		Type         getType() { return m_Type; }
		virtual void setEffect(Shader &s) const = 0;

		bool forSimpleShader() { return simpleShader; }
		bool forSpriteShader() { return spriteShader; }
		bool forTextShader() { return textShader; }
	};

	class UniformVec4 : public ShaderEffect
	{
	  protected:
		glm::vec4 vec;

	  public:
		UniformVec4(const std::string &name, glm::vec4 vec, Type type, bool simpleShader, bool spriteShader, bool textShader)
			: ShaderEffect(name, type, simpleShader, spriteShader, textShader), vec(vec)
		{
		}
		virtual ~UniformVec4() override {}

		virtual void setEffect(Shader &s) const override
		{
			s.setUniform4f(m_Name, vec[0], vec[1], vec[2], vec[3]);
		}

		void setVec(glm::vec4 newVec) { vec = newVec; }
	};

	class UniformMat4 : public ShaderEffect
	{
	  protected:
		glm::mat4 mat;

	  public:
		UniformMat4(const std::string &name, glm::mat4 mat, Type type, bool simpleShader, bool spriteShader, bool textShader)
			: ShaderEffect(name, type, simpleShader, spriteShader, textShader), mat(mat)
		{
		}
		virtual ~UniformMat4() override {}

		virtual void setEffect(Shader &s) const override
		{
			s.setUniformMat4f(m_Name, mat);
		}
		void setMat(glm::mat4 newMat) { mat = newMat; }
	};

	// This is the class for managing the shader effects, their IDs and storing and sending the effects that are carried
	class ShaderEffectsManager
	{
	  public:
		ShaderEffectsManager(const ShaderEffectsManager &) = delete;
		~ShaderEffectsManager();

		static uint16_t sendShaderEffect(const std::string &s, glm::vec4 vec, bool simpleShader = true, bool spriteShader = true, bool textShader = true, bool includeOverlay = false)
		{
			return get().sendShaderEffectImpl(s, vec, simpleShader, spriteShader, textShader, includeOverlay);
		}
		static uint16_t sendShaderEffect(const std::string &s, glm::mat4 mat, bool simpleShader = true, bool spriteShader = true, bool textShader = true, bool includeOverlay = false)
		{
			return get().sendShaderEffectImpl(s, mat, simpleShader, spriteShader, textShader, includeOverlay);
		}
		static uint16_t sendOverlayEffect(const std::string &s, glm::vec4 vec, bool simpleShader = true, bool spriteShader = true, bool textShader = true)
		{
			return get().sendOverlayEffectImpl(s, vec, simpleShader, spriteShader, textShader);
		}
		static uint16_t sendOverlayEffect(const std::string &s, glm::mat4 mat, bool simpleShader = true, bool spriteShader = true, bool textShader = true)
		{
			return get().sendOverlayEffectImpl(s, mat, simpleShader, spriteShader, textShader);
		}

		static void          deleteShaderEffect(uint16_t id) { get().deleteShaderEffectImpl(id); }
		static ShaderEffect *getShaderEffect(uint16_t id) { return get().getShaderEffectImpl(id); }

		static uint16_t findShaderEffect(const std::string &s) { return get().findShaderEffectImpl(s); }

		static void updateShaderEffects() { get().updateShaderEffectsImpl(); }

		static ShaderEffectsManager &get()
		{
			static ShaderEffectsManager s_Instance;
			return s_Instance;
		}

	  private:
		static ShaderEffectsManager s_Instance;
		std::vector<ShaderEffect *> m_Effects;

		ShaderEffectsManager();

		uint16_t      sendShaderEffectImpl(const std::string &s, glm::vec4 vec, bool simpleShader, bool spriteShader, bool textShader, bool includeOverlay);
		uint16_t      sendShaderEffectImpl(const std::string &s, glm::mat4 mat, bool simpleShader, bool spriteShader, bool textShader, bool includeOverlay);
		uint16_t      sendOverlayEffectImpl(const std::string &s, glm::vec4 vec, bool simpleShader, bool spriteShader, bool textShader);
		uint16_t      sendOverlayEffectImpl(const std::string &s, glm::mat4 mat, bool simpleShader, bool spriteShader, bool textShader);
		void          deleteShaderEffectImpl(uint16_t id);
		ShaderEffect *getShaderEffectImpl(uint16_t id);
		uint16_t      findShaderEffectImpl(const std::string &s);

		void updateShaderEffectsImpl();
	};
}   // namespace Effect
