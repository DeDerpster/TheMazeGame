#include "Layer.h"

#include "Log.h"
#include "ShaderEffect.h"

Layer::Layer() {}
Layer::~Layer() {}

bool Layer::setEffect(Effect::RenderEffect *e)
{
	if(e->getType() == Effect::EffectType::shaderEffect)
	{
		Effect::ShaderEffectCarrier *ne = static_cast<Effect::ShaderEffectCarrier *>(e);
		m_ShaderEffectsIDs.push_back(ne->getID());
	}
	else if(e->getType() == Effect::EffectType::removeShaderEffect)
	{
		Effect::RemoveShaderEffect *ne = static_cast<Effect::RemoveShaderEffect *>(e);
		for(auto it = m_ShaderEffectsIDs.begin(); it != m_ShaderEffectsIDs.end();)
		{
			if(*it == ne->getID())
				it = m_ShaderEffectsIDs.erase(it);
			else
			{
				if(*it > ne->getID())
					(*it)--;
				++it;
			}
		}
	}
	return false;
}

void Layer::imGuiRender()
{
}