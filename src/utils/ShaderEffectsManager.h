#pragma once

#include <string>

#include "event/effect/ShaderEffect.h"
#include "rendering/glInterface/Shader.h"
#include "utils/Log.h"

namespace Effect
{
	// This is the effect that stores the information needed to apply it to a shader and is therefore stored by the shader effects manager
	class ShaderEffect
	{
	  protected:
		const std::string m_Name;   // Stores the name of the variable in the shader

		bool simpleShader, spriteShader, textShader;   // Stores which shaders it applies to

		Event::CallType m_Type;   // Stores the type of effect (this is to allow it to be reapplied when the layers change)

	  public:
		ShaderEffect(const std::string &name, Event::CallType type, bool simpleShader, bool spriteShader, bool textShader)
			: m_Name(name), m_Type(type), simpleShader(simpleShader), spriteShader(spriteShader), textShader(textShader)
		{
		}
		virtual ~ShaderEffect() {}

		std::string     getName() { return m_Name; }
		Event::CallType getType() { return m_Type; }
		virtual void    setEffect(Shader &s) const = 0;

		bool forSimpleShader() { return simpleShader; }
		bool forSpriteShader() { return spriteShader; }
		bool forTextShader() { return textShader; }
	};

	class UniformVec4 : public ShaderEffect   // This is for a 4d vector
	{
	  protected:
		glm::vec4 vec;

	  public:
		UniformVec4(const std::string &name, glm::vec4 vec, Event::CallType type, bool simpleShader, bool spriteShader, bool textShader)
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

	class UniformMat4 : public ShaderEffect   // This is for a 4d matrix
	{
	  protected:
		glm::mat4 mat;

	  public:
		UniformMat4(const std::string &name, glm::mat4 mat, Event::CallType type, bool simpleShader, bool spriteShader, bool textShader)
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

		static uint16_t sendEffect(const std::string &s, glm::vec4 vec, Event::CallType callType = Event::CallType::Normal, bool simpleShader = true, bool spriteShader = true, bool textShader = true)
		{
			return get().sendEffectImpl(s, vec, callType, simpleShader, spriteShader, textShader);
		}
		static uint16_t sendEffect(const std::string &s, glm::mat4 mat, Event::CallType callType = Event::CallType::Normal, bool simpleShader = true, bool spriteShader = true, bool textShader = true)
		{
			return get().sendEffectImpl(s, mat, callType, simpleShader, spriteShader, textShader);
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

		uint16_t      sendEffectImpl(const std::string &s, glm::vec4 vec, Event::CallType callType, bool simpleShader, bool spriteShader, bool textShader);
		uint16_t      sendEffectImpl(const std::string &s, glm::mat4 mat, Event::CallType callType, bool simpleShader, bool spriteShader, bool textShader);
		void          deleteShaderEffectImpl(uint16_t id);
		ShaderEffect *getShaderEffectImpl(uint16_t id);
		uint16_t      findShaderEffectImpl(const std::string &s);

		void updateShaderEffectsImpl();
	};
}   // namespace Effect
