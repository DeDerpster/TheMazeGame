#include "GUILayer.h"

GUILayer::GUILayer()
	: m_ConnectedLevel(nullptr)
{
}

GUILayer::GUILayer(Level *connectedLevel)
	: m_ConnectedLevel(connectedLevel)
{
}

GUILayer::~GUILayer()
{

}

void GUILayer::render()
{
	for(MenuObject *obj : m_Objects)
		obj->render();
}

void GUILayer::update()
{
	for(MenuObject *obj : m_Objects)
		obj->update();
}

bool GUILayer::eventCallback(const Event::Event &e)
{
	for(MenuObject *obj : m_Objects)
	{
		if(obj->eventCallback(e))
			return true;
	}
	return false;
}

void GUILayer::addMenuObject(MenuObject *object)
{
	m_Objects.push_back(object);
}

void GUILayer::setConnectedLevel(Level *level)
{
	m_ConnectedLevel = level;
}