#include "Layer.h"

#include "Log.h"
#include "ShaderEffect.h"

Layer::Layer() {}
Layer::~Layer() {}

bool Layer::setEffect(Effect::RenderEffect *e)
{
	if(e->getType() == Effect::EffectType::ShaderEffect)
	{
		Effect::RenderShaderEffect *ne = static_cast<Effect::RenderShaderEffect *>(e);
		ne->setEffect(*m_Shader);
	}
	return false;
}

void Layer::imGuiRender()
{
}