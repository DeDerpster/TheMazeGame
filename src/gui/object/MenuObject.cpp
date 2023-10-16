#include "MenuObject.h"

#include "Log.h"
#include "layer/GUILayer.h"

#include "event/menu/WindowResize.h"

MenuObject::MenuObject(float x, float y, float width, float height, Layer *layer)
	: x(x), y(y), width(width), height(height), positionFunc([](float *, float *, float *, float *) {}), m_Layer(layer)
{
	// Checks the type of layer given
	GUILayer *guilayer = dynamic_cast<GUILayer *>(layer);
	if(!guilayer)
		Log::critical("Wrong layer type given!", LOGINFO);
}

MenuObject::MenuObject(std::function<void(float *, float *, float *, float *)> posFunc, Layer *layer)
	: positionFunc(posFunc), m_Layer(layer)
{
	// If a postion function is given it will use that to set the x, y and width and height
	positionFunc(&x, &y, &width, &height);

	// Checks the type of layer given
	GUILayer *guilayer = dynamic_cast<GUILayer *>(layer);
	if(!guilayer)
		Log::critical("Wrong layer type given!", LOGINFO);
}

MenuObject::~MenuObject()
{
}

bool MenuObject::eventCallback(const Event::Event &e)
{
	// Checks for a window resize event (if one has happened it updates its coordinates)
	switch(e.getType())
	{
	case Event::EventType::WindowResize:
		positionFunc(&x, &y, &width, &height);

	default:
		return false;
	}
}