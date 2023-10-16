#include "GUIStack.h"

#include "Application.h"
#include "level/Maze.h"

#include "event/menu/ChangeGUIMenu.h"

GUIStack::GUIStack(Level *level)
	: m_ActiveLayer(0)
{
	if(dynamic_cast<Maze *>(level))
	{
		// Set up to create one for the maze
		m_Layers.push_back(new GUILayer(GUILayer::Type::GameOverlay, level));
		m_Layers.push_back(new GUILayer(GUILayer::Type::PlayerInventory, level));
		m_Layers.push_back(new GUILayer(GUILayer::Type::ChestInventory, level));
		m_Layers.push_back(new GUILayer(GUILayer::Type::NPCInventory, level));
		m_Layers.push_back(new GUILayer(GUILayer::Type::NPCInteraction, level));
		m_Layers.push_back(new GUILayer(GUILayer::Type::ExitMenu, level));
		m_Layers.push_back(new GUILayer(GUILayer::Type::PlayerDeath, level));
		m_Layers.push_back(new GUILayer(GUILayer::Type::PlayerWin, level));
	}
	else
	{
		Log::warning("Unknown level created stack!");
		m_ActiveLayer = -1;
	}
}

GUIStack::~GUIStack()
{
	// Deletes all the layers
	for(GUILayer *layer : m_Layers)
		delete layer;
}

void GUIStack::render()
{
	// Renders the current active layer
	if(m_ActiveLayer != -1)
		m_Layers[m_ActiveLayer]->render();
}

void GUIStack::update()
{
	// Updates the current active layer
	if(m_ActiveLayer != -1)
		m_Layers[m_ActiveLayer]->update();
}

bool GUIStack::eventCallback(const Event::Event &e)
{
	switch(e.getType())
	{
	case Event::EventType::ChangeGUIMenu:
	{
		// If the event is a change GUI it will change the GUI to a given menu
		const Event::ChangeGUIMenuEvent &ne = static_cast<const Event::ChangeGUIMenuEvent &>(e);

		m_ActiveLayer = static_cast<int>(ne.menu);

		if(ne.menu == Event::ChangeGUIMenuEvent::Menu::Overlay)
			Application::setIsPaused(false);
		else
			Application::setIsPaused(true);

		return true;
	}

	case Event::EventType::WindowResize:
	{
		// If a window resize event happens it calls the event on all the layers
		for(GUILayer *layer : m_Layers)
			layer->eventCallback(e);

		return false;
	}

	default:
		// Otherwise it will call the event on the active layer
		if(m_Layers[m_ActiveLayer]->eventCallback(e))
			return true;

		return Layer::eventCallback(e);
	}
}
