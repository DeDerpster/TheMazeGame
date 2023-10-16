#include "Trapdoor.h"

#include "2dVec.h"
#include "Application.h"
#include "layer/level/Level.h"

#include "rendering/sprite/Sprite.h"

#include "entity/movableEntity/mob/Player.h"

#include "event/game/ShowAlternatives.h"
#include "event/input/Mouse.h"

Trapdoor::Trapdoor()
	: Tile(), m_State(Button::State::None)
{
}

Trapdoor::Trapdoor(float x, float y, double rotation, Level *level)
	: Tile(x, y, rotation, Sprite::ID::tileBasicTrapdoor, false, level), m_State(Button::State::None)
{
}

Trapdoor::~Trapdoor()
{
}

void Trapdoor::render()
{
	Tile::render();

	// If the mouse is hovering over the trapdoor it will display its name
	if(m_State == Button::State::Hover)
	{
		float       scale    = 35.0f;
		Vec2f       mousePos = Application::getCamera()->convertWindowToLevel(Event::getMousePos());
		std::string name     = "Trapdoor";
		uint8_t     layer    = 6;
		Render::hoverText(name, mousePos.x, mousePos.y, scale, {1.0f, 1.0f, 1.0f, 1.0f}, {0.3f, 0.3f, 0.3f, 0.7f}, layer);
	}
}

void Trapdoor::update()
{
	Vec2f mousePos = Application::getCamera()->convertWindowToLevel(Event::getMousePos());

	if(doesPointIntersectWithBox(mousePos, {x, y}, getCollisionBox()))
		m_State = Button::State::Hover;
	else
		m_State = Button::State::None;
}

bool Trapdoor::eventCallback(const Event::Event &e)
{
	switch(e.getType())
	{
	case Event::EventType::MouseClicked:
	{   // Checks for a mouse clicked event
		if(!m_Level || m_Locked)
			return false;

		const Event::MouseClickedEvent &ne = static_cast<const Event::MouseClickedEvent &>(e);

		Vec2f convPos = Application::getCamera()->convertWindowToLevel(ne.pos);

		// Checks the player is in proximity
		Player *const player = m_Level->getPlayer();
		if(doesPointIntersectWithBox(Application::getCamera()->convertWindowToLevel(ne.pos), {x, y}, getCollisionBox()) && distBetweenVec<Vec2f>({player->getX(), player->getY() - player->getWidth() / 2}, {x, y}) < 1.5f * TILE_SIZE)
		{
			// Calls the endLevel function
			m_Level->endLevel();
			return true;
		}

		return false;
	}

	case Event::EventType::ShowAltTile:
	{
		const Event::ShowAltTileEvent &ne = static_cast<const Event::ShowAltTileEvent &>(e);

		m_Locked = ne.showAlt;
	}

	default:
		return Tile::eventCallback(e);
	}
}

#ifdef DEBUG
void Trapdoor::imGuiRender()
{
}
#endif