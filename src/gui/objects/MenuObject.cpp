#include "MenuObject.h"

#include "GUILayer.h"
#include "Log.h"

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
	if(e.getType() == Event::EventType::windowResize)
	{
		positionFunc(&x, &y, &width, &height);
	}
	return false;
}