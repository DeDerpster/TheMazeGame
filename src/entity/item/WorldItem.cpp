#include "WorldItem.h"

#include "Application.h"
#include "Player.h"

WorldItem::WorldItem(Item *item)
	: Entity(0.0f, 0.0f, {{-50.0f, -50.0f}, {50.0f, 50.0f}}, nullptr, item->getSpriteID()), m_Item(item) {}

WorldItem::WorldItem(float x, float y, Item *item)
	: Entity(x, y, {{-50.0f, -50.0f}, {50.0f, 50.0f}}, nullptr, item->getSpriteID()), m_Item(item) {}

WorldItem::WorldItem(float x, float y, Level *level, Item *item)
	: Entity(x, y, {{-50.0f, -50.0f}, {50.0f, 50.0f}}, level, item->getSpriteID()), m_Item(item) {}

WorldItem::~WorldItem()
{
	if(m_Item)
		delete m_Item;
}

void WorldItem::render()
{
	if(m_Item)
	{
		m_Item->render(x, y, 0.0f, 50);
		float scale = 35.0f;
		// float textWidth = Render::getTextWidth(*m_Item->getName(), scale);
		Render::text(*m_Item->getName(), x /* - textWidth / 2*/, y + m_CollisionBox.lowerBound.y, scale, {0.0f, 0.0f, 0.0f, 1.0f}, true);
	}
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