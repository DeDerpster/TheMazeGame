#include "Layer.h"

#include "Log.h"
#include "ShaderEffectsManager.h"

#include "event/effect/ShaderEffect.h"

Layer::Layer() {}
Layer::~Layer() {}

bool Layer::eventCallback(const Event::Event &e)
{
	// The layer only checks for shader effects changes and updates the shader effects id list accordingly
	switch(e.getType())
	{
	case Event::EventType::ApplyShaderEffect:
	{
		// This adds an effect to the current effect lists
		const Event::ApplyShaderEffectEvent &ne = static_cast<const Event::ApplyShaderEffectEvent &>(e);
		m_ShaderEffectsIDs.push_back(ne.id);

		return false;
	}

	case Event::EventType::RemoveShaderEffect:
	{
		// If there is a removed effect it will remove the effect and will update all the ids that are above it (as the ids will shift down 1 when an effect is removed)
		const Event::RemoveShaderEffectEvent &ne = static_cast<const Event::RemoveShaderEffectEvent &>(e);
		for(auto it = m_ShaderEffectsIDs.begin(); it != m_ShaderEffectsIDs.end();)
		{
			if(*it == ne.id)
				it = m_ShaderEffectsIDs.erase(it);
			else
			{
				if(*it > ne.id)
					(*it)--;
				++it;
			}
		}
		return false;
	}

	default:
		return false;
	}
}

#ifdef DEBUG
void Layer::imGuiRender()
{
}
#endif