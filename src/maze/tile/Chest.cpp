#include "Chest.h"

#include "KeyDefinitions.h"

#include "2dVec.h"
#include "Application.h"
#include "RandomGen.h"
#include "layer/level/Level.h"

#include "rendering/sprite/Sprite.h"

#include "entity/movableEntity/mob/Player.h"

#include "event/game/ChestOpened.h"
#include "event/input/Mouse.h"
#include "event/menu/ChangeGUIMenu.h"

Chest::Chest()
	: Tile(), m_Inventory(DEFAULT_INVENTORY_SIZE), m_State(Button::State::None), m_IsDud(true)
{
}

Chest::Chest(float x, float y, double rotation, Level *level, bool isDud)
	: Tile(x, y, rotation, Sprite::ID::tileBasicChest, true, level), m_Inventory(DEFAULT_INVENTORY_SIZE), m_State(Button::State::None), m_IsDud(isDud), m_HasOpened(false)
{
	// If it isn't a dud, it will generate an inventory
	if(!m_IsDud)
		generateInventory();
}

Chest::~Chest()
{
	// Everything is on the stack so it doesn't have to delete anything (and the ItemContainer handles the deletion of the items stored in it)
}

void Chest::generateInventory()
{
	// Generates a random number of items and uses the Random::getItem() to get a random item
	int numOfItems = Random::uniformDist(2, 10);
	for(int i = 0; i < numOfItems; i++)
		m_Inventory.push_back(Random::getItem());
}

void Chest::render()
{
	// Renders itself
	Tile::render();
	// If there is a mouse over it, it will render text over it
	if(m_State == Button::State::Hover && !Application::getIsPaused())
	{
		float       scale    = 35.0f;
		Vec2f       mousePos = Application::getCamera()->convertWindowToLevel(Event::getMousePos());
		std::string name     = "Chest";
		uint8_t     layer    = 6;
		Render::hoverText(name, mousePos.x, mousePos.y, scale, {1.0f, 1.0f, 1.0f, 1.0f}, {0.3f, 0.3f, 0.3f, 0.7f}, layer);
	}
}

void Chest::update()
{
	// Updates its state
	Vec2f mousePos = Application::getCamera()->convertWindowToLevel(Event::getMousePos());

	if(doesPointIntersectWithBox(mousePos, {x, y}, getCollisionBox()))
		m_State = Button::State::Hover;
	else
		m_State = Button::State::None;
}

bool Chest::eventCallback(const Event::Event &e)
{
	switch(e.getType())
	{
	case Event::EventType::MouseClicked:
	{
		// If the mouse has been clicked over its position, it will open an inventory for the chest
		const Event::MouseClickedEvent &ne = static_cast<const Event::MouseClickedEvent &>(e);

		Vec2f convPos = Application::getCamera()->convertWindowToLevel(ne.pos);

		Player *const player = m_Level->getPlayer();
		// Checks if the player is within range of clicking it
		if(doesPointIntersectWithBox(Application::getCamera()->convertWindowToLevel(ne.pos), {x, y}, getCollisionBox()) && distBetweenVec<Vec2f>({player->getX(), player->getY() - player->getWidth() / 2}, {x, y}) < 1.5f * TILE_SIZE)
		{
			if(!m_HasOpened)
			{
				player->getStats()->hasFoundChest();
				m_HasOpened = false;
			}
			// Sends out an event to change the menu
			Event::ChangeGUIMenuEvent e1(Event::ChangeGUIMenuEvent::Menu::ChestInventory);
			Application::callEvent(e1, Event::CallType::Overlay);

			// Sets out an event with a pointer to its inventory for the MIHM to pick up on and change to
			Event::ChestOpenedEvent e2(&m_Inventory, nullptr);
			Application::callEvent(e2, Event::CallType::Overlay);

			return true;
		}

		return false;
	}

	default:
		return Tile::eventCallback(e);
	}
}

#ifdef DEBUG
void Chest::imGuiRender()
{
}
#endif