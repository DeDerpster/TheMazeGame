#include "MenuObject.h"

#include "Log.h"
#include "layer/GUILayer.h"

#include "event/menu/WindowResize.h"

MenuObject::MenuObject(float x, float y, float width, float height, Layer *layer)
	: x(x), y(y), width(width), height(height), positionFunc([](float *, float *, float *, float *) {}), m_Layer(layer)
{
	GUILayer *guilayer = dynamic_cast<GUILayer *>(layer);
	if(!guilayer)
		Log::critical("Wrong layer type given!", LOGINFO);
}

MenuObject::MenuObject(std::function<void(float *, float *, float *, float *)> posFunc, Layer *layer)
	: positionFunc(posFunc), m_Layer(layer)
{
	positionFunc(&x, &y, &width, &height);
}

MenuObject::~MenuObject()
{
}

bool MenuObject::eventCallback(const Event::Event &e)
{
	switch(e.getType())
	{
	case Event::EventType::WindowResize:
		positionFunc(&x, &y, &width, &height);

	default:
		return false;
	}
}