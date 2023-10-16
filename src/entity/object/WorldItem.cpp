#include "WorldItem.h"

#include "Application.h"
#include "KeyDefinitions.h"
#include "layer/MessageManager.h"
#include "layer/level/Level.h"

#include "entity/movableEntity/mob/Player.h"

#include "event/input/Mouse.h"

WorldItem::WorldItem(Item *item)
	: Entity(0.0f, 0.0f, TILE_SIZE / 2, nullptr, item->getSpriteID()), m_Item(item), m_State(Button::State::None) {}

WorldItem::WorldItem(float x, float y, Item *item)
	: Entity(x, y, TILE_SIZE / 2, nullptr, item->getSpriteID()), m_Item(item), m_State(Button::State::None) {}

WorldItem::WorldItem(float x, float y, Level *level, Item *item)
	: Entity(x, y, TILE_SIZE / 2, level, item->getSpriteID()), m_Item(item), m_State(Button::State::None) {}

WorldItem::~WorldItem()
{
	if(m_Item)
		delete m_Item;
}

void WorldItem::render()
{
	if(m_Item)
	{
		// Renders the object
		m_Item->render(x, y, 0.0f, width, 2);

		if(m_State == Button::State::Hover && !Application::getIsPaused())
		{
			// If the game is not paused the mouse is hovering over the object it will display item's name
			float scale    = 35.0f;
			Vec2f mousePos = Application::getCamera()->convertWindowToLevel(Event::getMousePos());

			uint8_t layer = 6;

			Render::hoverText(*m_Item->getName(), mousePos.x, mousePos.y, scale, {1.0f, 1.0f, 1.0f, 1.0f}, {0.3f, 0.3f, 0.3f, 0.7f}, layer);
		}
	}
}

void WorldItem::update()
{
	if(m_Item)
	{
		// Updates the state
		Vec2f mousePos = Application::getCamera()->convertWindowToLevel(Event::getMousePos());

		if(doesIntersectWith(mousePos))
			m_State = Button::State::Hover;
		else
			m_State = Button::State::None;
	}
}
bool WorldItem::eventCallback(const Event::Event &e)
{
	if(!m_Item)
		return false;

	switch(e.getType())
	{
	case Event::EventType::MouseClicked:
	{
		if(!m_Level)   // If the level is a nullptr it cannot access the player
		{
			Log::warning("Level is a nullptr so cannot pick up item!");
			return false;
		}

		const Event::MouseClickedEvent &ne = static_cast<const Event::MouseClickedEvent &>(e);

		Vec2f convPos = Application::getCamera()->convertWindowToLevel(ne.pos);

		Player *const player = m_Level->getPlayer();
		if(m_State == Button::State::Hover && ne.action == Event::Action::Press)
		{
			// Checks the player is in range to pick up the item
			if(distBetweenVec<Vec2f>({player->getX(), player->getY() - player->getWidth() / 2}, {x, y}) < 1.5f * TILE_SIZE)
			{
				// If the inventory is full it won't pick the item up
				if(player->pickUp(m_Item))
				{
					Log::variable("Player picked up", *m_Item->getName());
					m_Item = nullptr;
				}
				else
					MessageManager::sendMessage("Inventory full!", MessageManager::Priority::High);
			}
			else   // If the player is too far away it will display a message
				MessageManager::sendMessage("The item is too far away!", MessageManager::Priority::Medium);

			return true;
		}

		return false;
	}

	default:
		return Entity::eventCallback(e);
	}
}
#ifdef DEBUG
void WorldItem::imGuiRender()
{
}
#endif

Item *WorldItem::pickUp()
{
	// Will return the item pointer and change it to a nullptr
	Item *temp = m_Item;
	m_Item     = nullptr;
	return temp;
}

bool WorldItem::deleteMe()
{
	return !m_Item;
}

Item *WorldItem::peak() { return m_Item; }