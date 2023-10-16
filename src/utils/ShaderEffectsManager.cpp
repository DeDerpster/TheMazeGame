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
	uint16_t ShaderEffectsManager::sendEffectImpl(const std::string &s, glm::vec4 vec, Event::CallType callType, bool simpleShader, bool spriteShader, bool textShader)
	{
		UniformVec4 *e = new UniformVec4(s, vec, callType, simpleShader, spriteShader, textShader);
		m_Effects.push_back(e);

		Event::ApplyShaderEffectEvent messenger((uint16_t) m_Effects.size());
		Application::callEvent(messenger, callType);

		return messenger.id;
	}

	uint16_t ShaderEffectsManager::sendEffectImpl(const std::string &s, glm::mat4 mat, Event::CallType callType, bool simpleShader, bool spriteShader, bool textShader)
	{
		UniformMat4 *e = new UniformMat4(s, mat, callType, simpleShader, spriteShader, textShader);
		m_Effects.push_back(e);

		Event::ApplyShaderEffectEvent messenger((uint16_t) m_Effects.size());
		Application::callEvent(messenger, callType);

		return messenger.id;
	}

	// This manages deleting a shader effect from all the layers and its storage
	void ShaderEffectsManager::deleteShaderEffectImpl(uint16_t id)
	{
		if(id > m_Effects.size() || id < 1)
		{
			Log::warning("Tried to delete effect outside of range");
			return;
		}

		Event::RemoveShaderEffectEvent messenger(id);
		Application::callEvent(messenger, Event::CallType::All);

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
			Event::ApplyShaderEffectEvent messenger(i + 1);
			Application::callEvent(messenger, m_Effects[i]->getType());
		}
	}
}   // namespace Effect