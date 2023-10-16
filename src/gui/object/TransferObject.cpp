#include "TransferObject.h"

#include "layer/GUILayer.h"

#include "event/input/Keyboard.h"
#include "event/input/Mouse.h"
#include "event/menu/Transfer.h"

#include <vector>

TransferObject::TransferObject(float width, float height, Layer *layer)
	: MenuObject(0.0f, 0.0f, width, height, layer), index(0), container(nullptr)
{
}

TransferObject::~TransferObject()
{
}

void TransferObject::update()
{
	if(container)
	{
		Vec2f mousePos = Event::getMousePos();

		x = mousePos.x;
		y = mousePos.y;
	}
}

void TransferObject::render()
{
	if(container)
	{
		container->getItem(index)->render(x, y, 0.0f, width, 9, true, true);
	}
}

bool TransferObject::eventCallback(const Event::Event &e)
{
	switch(e.getType())
	{
	case Event::EventType::ItemTransfer:
	{
		if(container)
			return true;
		const Event::ItemTransferEvent &ne = static_cast<const Event::ItemTransferEvent &>(e);

		index     = ne.index;
		container = ne.container;
		container->getItem(index)->setForcedRender(true);
		update();

		return true;
	}

	case Event::EventType::MouseClicked:
	{
		if(!container)
			return false;

		const Event::MouseClickedEvent &ne = static_cast<const Event::MouseClickedEvent &>(e);

		if(ne.button == Event::MouseButton::leftButton && ne.action == Event::Action::Press)
		{
			GUILayer *layer = dynamic_cast<GUILayer *>(m_Layer);
			if(layer)
				layer->transferObject(this);
			else
				Log::warning("Transfer object on incorrect layer!");

			return true;
		}

		return false;
	}

	case Event::EventType::KeyInput:
	{
		if(!container)
			return false;

		const Event::KeyboardEvent &ne = static_cast<const Event::KeyboardEvent &>(e);

		if(ne.key == Event::KeyboardKey::Escape && (ne.action == Event::Action::Press || ne.action == Event::Action::Repeat))
		{
			hasTransferred();
			return true;
		}

		return false;
	}

	default:
		return MenuObject::eventCallback(e);
	}
}

void TransferObject::hasTransferred()
{
	container->getItem(index)->setForcedRender(false);
	container = nullptr;
	index     = 0;
}
