#include "StatBar.h"

#include "Renderer.h"

StatBar::StatBar(float x, float y, float width, float height, Layer *layer, const float *stat, const float *statMax)
	: MenuObject(x, y, width, height, layer), m_Stat(stat), m_StatMax(statMax), m_BackgroundColour({1.0f, 0.0f, 0.0f, 1.0f})
{
}

StatBar::StatBar(std::function<void(float *, float *, float *, float *)> posFunc, Layer *layer, const float *stat, const float *statMax)
	: MenuObject(posFunc, layer), m_Stat(stat), m_StatMax(statMax), m_BackgroundColour({1.0f, 0.0f, 0.0f, 1.0f})
{
}

StatBar::StatBar(float x, float y, float width, float height, Layer *layer, const float *stat, const float *statMax, glm::vec4 backgroundColour)
	: MenuObject(x, y, width, height, layer), m_Stat(stat), m_StatMax(statMax), m_BackgroundColour(backgroundColour)
{
}

StatBar::StatBar(std::function<void(float *, float *, float *, float *)> posFunc, Layer *layer, const float *stat, const float *statMax, glm::vec4 backgroundColour)
	: MenuObject(posFunc, layer), m_Stat(stat), m_StatMax(statMax), m_BackgroundColour(backgroundColour)
{
}

StatBar::~StatBar()
{
	// NOTE: m_Stat and m_StatMax should be references to variables stored elsewhere, thus they should not be deleted here!
}

void StatBar::update()
{
}

void StatBar::render()
{
	Render::rectangle(x, y, 0.0f, width * (*m_Stat) / (*m_StatMax), height, m_BackgroundColour, true, true);
}

bool StatBar::eventCallback(const Event::Event &e)
{
	return MenuObject::eventCallback(e);
}

void StatBar::setStat(float *stat, float *statMax)
{
	m_Stat    = stat;
	m_StatMax = statMax;
}
