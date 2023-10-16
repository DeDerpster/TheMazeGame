#include "TransferObject.h"

#include "Application.h"

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
		// Will update the its position based on the mouse
		Vec2f mousePos = Event::getMousePos();

		x = mousePos.x;
		y = mousePos.y;
	}
}

void TransferObject::render()
{
	if(container)
	{
		// Renders if an object is being transferred
		container->getItem(index)->render(x, y, 0.0f, width * Application::getGUIScale(), 9, true, true);
	}
}

bool TransferObject::eventCallback(const Event::Event &e)
{
	switch(e.getType())
	{
	case Event::EventType::ItemTransfer:
	{
		// If there is an item transfer event it will update its index and container
		if(container)
			return true;
		const Event::ItemTransferEvent &ne = static_cast<const Event::ItemTransferEvent &>(e);

		index     = ne.index;
		container = ne.container;
		// Will make it so that the object must be forced to be rendered (which will stop it being rendered in the MIHMs)
		container->getItem(index)->setForcedRender(true);

		// Updates its position (So there is no frames where it is in the wrong position)
		update();

		return true;
	}

	case Event::EventType::MouseClicked:
	{
		if(!container)
			return false;

		const Event::MouseClickedEvent &ne = static_cast<const Event::MouseClickedEvent &>(e);

		if(ne.button == Event::MouseButton::LeftButton && ne.action == Event::Action::Press)
		{
			// If there is a click it will send itself to the layer to be transferred (if possible)
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

		// If escape is clicked it will cancel the transfer
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

void TransferObject::removeItem()
{
	// This will remove an item from the container (and reset its variables)
	container->getItem(index)->setForcedRender(false);
	container->removeItem(index);
	container = nullptr;
	index     = 0;
}

void TransferObject::hasTransferred()
{
	// Will reset the variables
	if(container->size() == 0)
		Log::error("The item has been transferred before resetting variables!", LOGINFO);
	else
		container->getItem(index)->setForcedRender(false);
	container = nullptr;
	index     = 0;
}
