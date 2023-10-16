#include "TransferObject.h"

#include "Event.h"
#include "GUILayer.h"

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
		container->getItem(index)->render(x, y, 0.0f, width, 9, true);
	}
}

bool TransferObject::eventCallback(const Event::Event &e)
{
	if(e.getType() == Event::EventType::itemTransfer)
	{
		const Event::ItemTransfer &ne = static_cast<const Event::ItemTransfer &>(e);

		index     = ne.index;
		container = ne.container;
		// TODO: Make it so it doesn't render item in other places

		return true;
	}
	if(container)
	{
		if(e.getType() == Event::EventType::mouseClicked)
		{
			GUILayer *layer = dynamic_cast<GUILayer *>(m_Layer);
			if(layer)
			{
				layer->transferObject(this);
			}
			else
				Log::warning("Transfer object on incorrect layer!");
			return true;
		}
		else if(e.getType() == Event::EventType::keyInput)
		{
			const Event::KeyboardEvent &ne = static_cast<const Event::KeyboardEvent &>(e);

			if(ne.key == GLFW_KEY_ESCAPE && (ne.action == GLFW_PRESS || ne.action == GLFW_REPEAT))
			{
				hasTransferred();
				return true;
			}
		}
	}
	return MenuObject::eventCallback(e);
}

void TransferObject::hasTransferred()
{
	container = nullptr;
	index     = 0;
}
