#include "MenuObject.h"

MenuObject::MenuObject(float x, float y, float width, float height)
	: x(x), y(y), width(width), height(height), positionFunc([](float *, float *, float *, float *) {})
{
}

MenuObject::MenuObject(std::function<void(float *, float *, float *, float *)> posFunc)
	: positionFunc(posFunc)
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