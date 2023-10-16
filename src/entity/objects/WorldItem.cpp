#include "WorldItem.h"

#include "Application.h"
#include "KeyDefinitions.h"
#include "Level.h"
#include "MessageManager.h"
#include "Player.h"

WorldItem::WorldItem(Item *item)
	: Entity(0.0f, 0.0f, TILE_SIZE / 2, nullptr, item->getSpriteID()), m_Item(item), m_State(Button::State::None) {}

WorldItem::WorldItem(float x, float y, float size, Item *item)
	: Entity(x, y, size, nullptr, item->getSpriteID()), m_Item(item), m_State(Button::State::None) {}

WorldItem::WorldItem(float x, float y, float size, Level *level, Item *item)
	: Entity(x, y, size, level, item->getSpriteID()), m_Item(item), m_State(Button::State::None) {}

WorldItem::~WorldItem()
{
	if(m_Item)
		delete m_Item;
}

void WorldItem::render()
{
	if(m_Item)
	{
		m_Item->render(x, y, 0.0f, width, 2);
		if(m_State == Button::State::Hover && !Application::getIsPaused())
		{
			float        scale    = 35.0f;
			Vec2f        mousePos = Application::getCamera()->convertWindowToLevel(Event::getMousePos());

			uint8_t layer = 6;

			Render::hoverText(*m_Item->getName(), mousePos.x, mousePos.y, scale, {1.0f, 1.0f, 1.0f, 1.0f}, {0.3f, 0.3f, 0.3f, 0.7f}, layer);
		}
	}
}

void WorldItem::update()
{
	if(m_Item)
	{
		Vec2f mousePos = Application::getCamera()->convertWindowToLevel(Event::getMousePos());

		if(mousePos.x > x + m_CollisionBox.lowerBound.x && mousePos.x < x + m_CollisionBox.upperBound.x && mousePos.y > y + m_CollisionBox.lowerBound.y && mousePos.y < y + m_CollisionBox.upperBound.y)
			m_State = Button::State::Hover;
		else
			m_State = Button::State::None;
	}
}
bool WorldItem::eventCallback(const Event::Event &e)
{
	if(!m_Item)
		return false;
	if(e.getType() == Event::EventType::mouseClicked && m_Level)
	{
		const Event::MouseClickedEvent &ne = static_cast<const Event::MouseClickedEvent &>(e);

		Vec2f convPos = Application::getCamera()->convertWindowToLevel(ne.pos);

		Player *player = m_Level->getPlayer();
		if(doesIntersectWith(convPos))
		{
			if(distBetweenVec2f({player->getX(), player->getY() - player->getWidth() / 2}, {x, y}) < 1.5f * TILE_SIZE)
			{
				if(player->pickUp(m_Item))
				{
					Log::info("Picked up");
					m_Item = nullptr;
				}
				else
					MessageManager::sendMessage("Inventory full!", MessageManager::Priority::High);
			}
			else
				MessageManager::sendMessage("The item is too far away!", MessageManager::Priority::Medium);

			return true;
		}
	}
	return Entity::eventCallback(e);
}
#ifdef DEBUG
void WorldItem::imGuiRender()
{
}
#endif

Item *WorldItem::pickUp()
{
	Item *temp = m_Item;
	m_Item     = nullptr;
	return temp;
}

bool WorldItem::deleteMe()
{
	return !m_Item;
}

Item *WorldItem::peak() { return m_Item; }