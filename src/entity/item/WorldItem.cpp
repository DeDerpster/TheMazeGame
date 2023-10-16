#include "WorldItem.h"

#include "Application.h"
#include "Player.h"
#include "Tile.h"

WorldItem::WorldItem(Item *item)
	: Entity(0.0f, 0.0f, Tile::TILE_SIZE / 2, nullptr, item->getSpriteID()), m_Item(item), m_State(Button::State::None) {}

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
		m_Item->render(x, y, 0.0f, width);
		if(m_State == Button::State::Hover)
		{
			float scale = 35.0f;
			Vec2f        mousePos = Application::getCamera()->convertWindowToLevel(Event::getMousePos());
			CollisionBox box      = Render::getTextCollisionBox(*m_Item->getName(), scale);
			Render::rectangle(mousePos.x, mousePos.y + 4.0f + box.upperBound.y / 2, 0.0f, box.upperBound.x + 2.0f, box.upperBound.y + 4.0f, {0.3f, 0.3f, 0.3f, 0.7f});
			Render::text(*m_Item->getName(), mousePos.x, mousePos.y + 5.0f + box.upperBound.y / 2, scale, {1.0f, 1.0f, 1.0f, 1.0f}, true);
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
		if(doesIntersectWith(Application::getCamera()->convertWindowToLevel(ne.pos)) && distBetweenVec2f({player->getX(), player->getY()}, {x, y}) < 5 * Tile::TILE_SIZE)
		{
			Log::info("Picked up");
			player->pickUp(m_Item);
			m_Item = nullptr;
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