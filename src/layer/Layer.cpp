#include "Layer.h"

#include "Log.h"
#include "ShaderEffectsManager.h"

#include "event/effect/ShaderEffect.h"

Layer::Layer() {}
Layer::~Layer() {}

bool Layer::eventCallback(const Event::Event &e)
{
	switch(e.getType())
	{
	case Event::EventType::ApplyShaderEffect:
	{
		const Event::ApplyShaderEffectEvent &ne = static_cast<const Event::ApplyShaderEffectEvent &>(e);
		m_ShaderEffectsIDs.push_back(ne.id);

		return false;
	}

	case Event::EventType::RemoveShaderEffect:
	{
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