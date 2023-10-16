#include "GUIStack.h"

#include "Application.h"

GUIStack::GUIStack(Level *level)
	: m_ActiveLayer(0)
{
	m_Layers.push_back(new GUILayer(GUILayer::Type::GameOverlay, level));
	m_Layers.push_back(new GUILayer(GUILayer::Type::PlayerInventory, level));
	m_Layers.push_back(new GUILayer(GUILayer::Type::ChestInventory, level));
}

GUIStack::~GUIStack()
{
	for(GUILayer *layer : m_Layers)
		delete layer;
}

void GUIStack::render()
{
	if(m_ActiveLayer != -1)
		m_Layers[m_ActiveLayer]->render();
}

void GUIStack::update()
{
	if(m_ActiveLayer != -1)
		m_Layers[m_ActiveLayer]->update();
}

bool GUIStack::eventCallback(const Event::Event &e)
{
	if(e.getType() == Event::EventType::changeGUILayer)
	{
		const Event::ChangeGUIActiveLayer &ne = static_cast<const Event::ChangeGUIActiveLayer &>(e);

		m_ActiveLayer = static_cast<int>(ne.layer);

		if(ne.layer == InGameGUILayer::overlay)
			Application::setIsPaused(false);
		else
			Application::setIsPaused(true);

		return true;
	}
	else if(e.getType() == Event::EventType::windowResize)
	{
		for(GUILayer *layer : m_Layers)
			layer->eventCallback(e);
	}
	else if(m_ActiveLayer != 0)
		return m_Layers[m_ActiveLayer]->eventCallback(e);

	return false;
}
