#include "ShaderEffect.h"

#include "Application.h"
#include "GLM.h"
#include <vector>

namespace Effect
{
	ShaderEffects::ShaderEffects()
	{
	}

	ShaderEffects::~ShaderEffects()
	{
		for(RenderShaderEffect *s : m_Effects)
			delete s;
	}

	// These functions handle the sending of an effect, by creating them, adding them to the cache and sending them through the layers
	uint16_t ShaderEffects::sendShaderEffectImpl(const std::string &s, glm::vec4 vec, bool includeOverlay)
	{
		RenderShaderEffect::Type type;
		if(includeOverlay)
			type = RenderShaderEffect::Type::includeOverlay;
		else
			type = RenderShaderEffect::Type::normal;

		UniformVec4 *e = new UniformVec4(s, vec, type);
		m_Effects.push_back(e);

		ShaderEffectCarrier messenger((uint16_t) m_Effects.size());
		Application::setEffect(&messenger, includeOverlay);

		return messenger.getID();
	}

	uint16_t ShaderEffects::sendShaderEffectImpl(const std::string &s, glm::mat4 mat, bool includeOverlay)
	{
		RenderShaderEffect::Type type;
		if(includeOverlay)
			type = RenderShaderEffect::Type::includeOverlay;
		else
			type = RenderShaderEffect::Type::normal;

		UniformMat4 *e = new UniformMat4(s, mat, type);
		m_Effects.push_back(e);

		ShaderEffectCarrier messenger((uint16_t) m_Effects.size());
		Application::setEffect(&messenger, includeOverlay);

		return messenger.getID();
	}

	// These send the effects through the overlays only and not all the layers
	uint16_t ShaderEffects::sendOverlayEffectImpl(const std::string &s, glm::vec4 vec)
	{
		RenderShaderEffect::Type type = RenderShaderEffect::Type::onlyOverlay;

		UniformVec4 *e = new UniformVec4(s, vec, type);
		m_Effects.push_back(e);

		ShaderEffectCarrier messenger((uint16_t) m_Effects.size());
		Application::setOverlayEffect(&messenger);

		return messenger.getID();
	}

	uint16_t ShaderEffects::sendOverlayEffectImpl(const std::string &s, glm::mat4 mat)
	{
		RenderShaderEffect::Type type = RenderShaderEffect::Type::onlyOverlay;

		UniformMat4 *e = new UniformMat4(s, mat, type);
		m_Effects.push_back(e);

		ShaderEffectCarrier messenger((uint16_t) m_Effects.size());
		Application::setOverlayEffect(&messenger);

		return messenger.getID();
	}

	// This manages deleting a shader effect from all the layers and its storage
	void ShaderEffects::deleteShaderEffectImpl(uint16_t id)
	{
		if(id > m_Effects.size() || id < 1)
		{
			Log::warning("Tried to delete effect outside of range");
			return;
		}

		RemoveShaderEffect messenger(id);
		Application::setEffect(&messenger, true);

		delete m_Effects[id - 1];
		m_Effects.erase(m_Effects.begin() + id - 1);
	}

	RenderShaderEffect *ShaderEffects::getShaderEffectImpl(uint16_t id)
	{
		if(id > m_Effects.size() || id < 1)
		{
			Log::warning("Tried to access effect outside of range");
			return nullptr;
		}
		return m_Effects[id - 1];
	}

	// This finds the id of a shader by use of its name
	uint16_t ShaderEffects::findShaderEffectImpl(const std::string &s)
	{
		for(uint16_t i = 0; i < m_Effects.size(); i++)
		{
			if(s == m_Effects[i]->getName())
				return i + 1;
		}
		Log::warning("Did not find shader Effect!");
		return 0;
	}

	void ShaderEffects::updateShaderEffectsImpl()
	{
		for(uint16_t i = 0; i < m_Effects.size(); i++)
		{
			ShaderEffectCarrier messenger(i + 1);
			switch(m_Effects[i]->getType())
			{
			case RenderShaderEffect::Type::normal:
				Application::setEffect(&messenger, false);
				break;
			case RenderShaderEffect::Type::includeOverlay:
				Application::setEffect(&messenger, true);
				break;
			case RenderShaderEffect::Type::onlyOverlay:
				Application::setOverlayEffect(&messenger);
				break;
			default:
				Log::warning("Unknown effect type");
			}
		}
	}
}   // namespace Effect