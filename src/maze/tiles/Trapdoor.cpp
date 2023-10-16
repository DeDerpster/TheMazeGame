#include "Trapdoor.h"

#include "Application.h"
#include "FireStaff.h"
#include "Player.h"
#include "Sprite.h"
#include "Utils.h"

Trapdoor::Trapdoor()
	: Tile(), m_State(Button::State::None)
{
}

Trapdoor::Trapdoor(float x, float y, double rotation, Level *level)
	: Tile(x, y, rotation, BASIC_TRAPDOOR, false, level), m_State(Button::State::None)
{
}

Trapdoor::~Trapdoor()
{
}

CollisionBox Trapdoor::getCollisionBox()
{
	return {{-TILE_SIZE / 2, -TILE_SIZE / 2}, {TILE_SIZE / 2, TILE_SIZE / 2}};
}

void Trapdoor::render()
{
	Render::sprite(x, y, rotation, TILE_SIZE, m_SpriteID);
	if(m_State == Button::State::Hover)
	{
		float        scale    = 35.0f;
		Vec2f        mousePos = Application::getCamera()->convertWindowToLevel(Event::getMousePos());
		std::string  name     = "Trapdoor";
		Render::hoverText(name, mousePos.x, mousePos.y, scale, {1.0f, 1.0f, 1.0f, 1.0f}, {0.3f, 0.3f, 0.3f, 0.7f});
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
	if(e.getType() == Event::EventType::mouseClicked && m_Level)
	{
		const Event::MouseClickedEvent &ne = static_cast<const Event::MouseClickedEvent &>(e);

		Vec2f convPos = Application::getCamera()->convertWindowToLevel(ne.pos);

		Player *player = m_Level->getPlayer();
		if(doesPointIntersectWithBox(Application::getCamera()->convertWindowToLevel(ne.pos), {x, y}, getCollisionBox()) && distBetweenVec2f({player->getX(), player->getY() - player->getWidth() / 2}, {x, y}) < 1.5f * TILE_SIZE)
		{
			m_Level->endLevel();
			return true;
		}
	}

	return Tile::eventCallback(e);
}

#ifdef DEBUG
void Trapdoor::imGuiRender()
{
}
#endif