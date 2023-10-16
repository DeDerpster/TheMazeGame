#include "ShaderEffectsManager.h"

#include "Application.h"
#include <GLM.h>

#include <vector>

namespace Effect
{
	ShaderEffectsManager::ShaderEffectsManager()
	{
	}

	ShaderEffectsManager::~ShaderEffectsManager()
	{
		for(ShaderEffect *s : m_Effects)
			delete s;
	}

	// These functions handle the sending of an effect, by creating them, adding them to the cache and sending them through the layers
	uint16_t ShaderEffectsManager::sendShaderEffectImpl(const std::string &s, glm::vec4 vec, bool simpleShader, bool spriteShader, bool textShader, bool includeOverlay)
	{
		ShaderEffect::Type type;
		if(includeOverlay)
			type = ShaderEffect::Type::includeOverlay;
		else
			type = ShaderEffect::Type::normal;

		UniformVec4 *e = new UniformVec4(s, vec, type, simpleShader, spriteShader, textShader);
		m_Effects.push_back(e);

		ShaderEffectCarrier messenger((uint16_t) m_Effects.size());
		Application::setEffect(&messenger, includeOverlay);

		return messenger.getID();
	}

	uint16_t ShaderEffectsManager::sendShaderEffectImpl(const std::string &s, glm::mat4 mat, bool simpleShader, bool spriteShader, bool textShader, bool includeOverlay)
	{
		ShaderEffect::Type type;
		if(includeOverlay)
			type = ShaderEffect::Type::includeOverlay;
		else
			type = ShaderEffect::Type::normal;

		UniformMat4 *e = new UniformMat4(s, mat, type, simpleShader, spriteShader, textShader);
		m_Effects.push_back(e);

		ShaderEffectCarrier messenger((uint16_t) m_Effects.size());
		Application::setEffect(&messenger, includeOverlay);

		return messenger.getID();
	}

	// These send the effects through the overlays only and not all the layers
	uint16_t ShaderEffectsManager::sendOverlayEffectImpl(const std::string &s, glm::vec4 vec, bool simpleShader, bool spriteShader, bool textShader)
	{
		ShaderEffect::Type type = ShaderEffect::Type::onlyOverlay;

		UniformVec4 *e = new UniformVec4(s, vec, type, simpleShader, spriteShader, textShader);
		m_Effects.push_back(e);

		ShaderEffectCarrier messenger((uint16_t) m_Effects.size());
		Application::setOverlayEffect(&messenger);

		return messenger.getID();
	}

	uint16_t ShaderEffectsManager::sendOverlayEffectImpl(const std::string &s, glm::mat4 mat, bool simpleShader, bool spriteShader, bool textShader)
	{
		ShaderEffect::Type type = ShaderEffect::Type::onlyOverlay;

		UniformMat4 *e = new UniformMat4(s, mat, type, simpleShader, spriteShader, textShader);
		m_Effects.push_back(e);

		ShaderEffectCarrier messenger((uint16_t) m_Effects.size());
		Application::setOverlayEffect(&messenger);

		return messenger.getID();
	}

	// This manages deleting a shader effect from all the layers and its storage
	void ShaderEffectsManager::deleteShaderEffectImpl(uint16_t id)
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

	ShaderEffect *ShaderEffectsManager::getShaderEffectImpl(uint16_t id)
	{
		if(id > m_Effects.size() || id < 1)
		{
			Log::warning("Tried to access effect outside of range");
			return nullptr;
		}
		return m_Effects[id - 1];
	}

	// This finds the id of a shader by use of its name
	uint16_t ShaderEffectsManager::findShaderEffectImpl(const std::string &s)
	{
		for(uint16_t i = 0; i < m_Effects.size(); i++)
		{
			if(s == m_Effects[i]->getName())
				return i + 1;
		}
		Log::warning("Did not find shader Effect!");
		return 0;
	}

	void ShaderEffectsManager::updateShaderEffectsImpl()
	{
		for(uint16_t i = 0; i < m_Effects.size(); i++)
		{
			ShaderEffectCarrier messenger(i + 1);
			switch(m_Effects[i]->getType())
			{
			case ShaderEffect::Type::normal:
				Application::setEffect(&messenger, false);
				break;
			case ShaderEffect::Type::includeOverlay:
				Application::setEffect(&messenger, true);
				break;
			case ShaderEffect::Type::onlyOverlay:
				Application::setOverlayEffect(&messenger);
				break;
			default:
				Log::warning("Unknown effect type");
			}
		}
	}
}   // namespace Effect