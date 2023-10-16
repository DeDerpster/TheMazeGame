#include "Chest.h"

#include "Application.h"
#include "FireStaff.h"
#include "Player.h"
#include "Sprite.h"
#include "Utils.h"

Chest::Chest()
	: Tile(), m_State(Button::State::None)
{
	generateInventory();
}

Chest::Chest(float x, float y, double rotation, Level *level)
	: Tile(x, y, rotation, CHEST, true, level), m_State(Button::State::None)
{
	generateInventory();
}

Chest::~Chest()
{
	for(Item *item : m_Inventory)
		delete item;
}

void Chest::generateInventory()
{
	m_Inventory.push_back(new FireStaff());
	m_Inventory.push_back(new FireStaff());
	m_Inventory.push_back(new FireStaff());
}

CollisionBox Chest::getCollisionBox()
{
	return {{-TILE_SIZE / 2, -TILE_SIZE / 2}, {TILE_SIZE / 2, TILE_SIZE / 2}};
}

void Chest::render()
{
	Render::sprite(x, y, 0.0f, TILE_SIZE, m_SpriteID);
	if(m_State == Button::State::Hover)
	{
		float        scale    = 35.0f;
		Vec2f        mousePos = Application::getCamera()->convertWindowToLevel(Event::getMousePos());
		std::string  name     = "Chest";
		CollisionBox box      = Render::getTextCollisionBox(name, scale);
		Render::rectangle(mousePos.x, mousePos.y + 4.0f + box.upperBound.y / 2, 0.0f, box.upperBound.x + 2.0f, box.upperBound.y + 4.0f, {0.3f, 0.3f, 0.3f, 0.7f});
		Render::text(name, mousePos.x, mousePos.y + 5.0f + box.upperBound.y / 2, scale, {1.0f, 1.0f, 1.0f, 1.0f}, true);
	}
}

void Chest::update()
{
	Vec2f mousePos = Application::getCamera()->convertWindowToLevel(Event::getMousePos());

	if(doesPointIntersectWithBox(mousePos, {x, y}, getCollisionBox()))
		m_State = Button::State::Hover;
	else
		m_State = Button::State::None;
}

bool Chest::eventCallback(const Event::Event &e)
{
	if(e.getType() == Event::EventType::mouseClicked && m_Level)
	{
		const Event::MouseClickedEvent &ne = static_cast<const Event::MouseClickedEvent &>(e);

		Vec2f convPos = Application::getCamera()->convertWindowToLevel(ne.pos);

		Player *player = m_Level->getPlayer();
		if(doesPointIntersectWithBox(Application::getCamera()->convertWindowToLevel(ne.pos), {x, y}, getCollisionBox()) && distBetweenVec2f({player->getX(), player->getY() - player->getWidth() / 2}, {x, y}) < 1.5f * TILE_SIZE)
		{
			m_Level->openChest(m_Inventory);
			return true;
		}
	}

	return Tile::eventCallback(e);
}

#ifdef DEBUG
void Chest::imGuiRender()
{
}
#endif