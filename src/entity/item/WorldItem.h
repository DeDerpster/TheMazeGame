#pragma once

#include "Entity.h"
#include "Item.h"
#include "Text.h"
#include "Tile.h"

class WorldItem : public Entity
{
  protected:
	Item *m_Item;

  public:
	WorldItem(Item *item)
		: Entity(0.0f, 0.0f, {{-10.0f, -10.0f}, {10.0f, 10.0f}}, nullptr, item->getSpriteID()), m_Item(item) {}

	WorldItem(float x, float y, Item *item)
		: Entity(x, y, {{-10.0f, -10.0f}, {10.0f, 10.0f}}, nullptr, item->getSpriteID()), m_Item(item) {}

	WorldItem(float x, float y, Level *level, Item *item)
		: Entity(x, y, {{-10.0f, -10.0f}, {10.0f, 10.0f}}, level, item->getSpriteID()), m_Item(item) {}

	virtual ~WorldItem()
	{
		if(m_Item)
			delete m_Item;
	}

	virtual void render() override
	{
		if(m_Item)
			m_Item->render(x, y, 0.0f, 50);

		// Render::Text::addToBuffer(*m_Item->getName(), x - Application::getCamera()->getX(), y - Application::getCamera()->getY(), 0.5, {0.0f, 0.0f, 0.0f, 1.0f});
	}
	virtual void update() override {}
	virtual bool eventCallback(const Application::Event &e) override
	{
		if(!m_Item)
			return false;
		if(e.getType() == Application::EventType::mouseClicked && m_Level)
		{
			const Application::MouseClickedEvent &ne = static_cast<const Application::MouseClickedEvent &>(e);

			Vec2f convPos = Application::getCamera()->convertWindowToLevel(ne.pos);

			// Log::variable("Mouse X", convPos.x);
			// Log::variable("Mouse Y", convPos.y);

			Player *player = m_Level->getPlayer();
			if(doesIntersectWith(Application::getCamera()->convertWindowToLevel(ne.pos)) && distBetweenVec2f({player->getX(), player->getY()}, {x, y}) < 5 * Tile::TILE_SIZE)
			{
				Log::info("Picked up");
				player->pickUp(m_Item);
				m_Item = nullptr;
				return true;
			}
		}
		return false;
	}
#ifdef DEBUG
	virtual void imGuiRender() override
	{
	}
#endif

	Item *pickUp()
	{
		Item *temp = m_Item;
		m_Item     = nullptr;
		return temp;
	}

	virtual bool deleteMe() override
	{
		return !m_Item;
	}

	Item *peak() { return m_Item; }
};