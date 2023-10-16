#include "WorldItem.h"

#include "Application.h"
#include "Player.h"

WorldItem::WorldItem(Item *item)
	: Entity(0.0f, 0.0f, {{-10.0f, -10.0f}, {10.0f, 10.0f}}, nullptr, item->getSpriteID()), m_Item(item) {}

WorldItem::WorldItem(float x, float y, Item *item)
	: Entity(x, y, {{-10.0f, -10.0f}, {10.0f, 10.0f}}, nullptr, item->getSpriteID()), m_Item(item) {}

WorldItem::WorldItem(float x, float y, Level *level, Item *item)
	: Entity(x, y, {{-10.0f, -10.0f}, {10.0f, 10.0f}}, level, item->getSpriteID()), m_Item(item) {}

WorldItem::~WorldItem()
{
	if(m_Item)
		delete m_Item;
}

void WorldItem::render()
{
	if(m_Item)
		m_Item->render(x, y, 0.0f, 50);

	// Render::Text::addToBuffer(*m_Item->getName(), x - Application::getCamera()->getX(), y - Application::getCamera()->getY(), 0.5, {0.0f, 0.0f, 0.0f, 1.0f});
}
void WorldItem::update() {}
bool WorldItem::eventCallback(const Event::Event &e)
{
	if(!m_Item)
		return false;
	if(e.getType() == Event::EventType::mouseClicked && m_Level)
	{
		const Event::MouseClickedEvent &ne = static_cast<const Event::MouseClickedEvent &>(e);

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