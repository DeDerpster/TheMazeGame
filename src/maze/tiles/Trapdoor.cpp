#include "Trapdoor.h"

#include "Application.h"
#include "FireStaff.h"
#include "Level.h"
#include "Player.h"
#include "Sprite.h"
#include "Utils.h"
#include "Player.h"

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

CollisionBox Trapdoor::getCollisionBox()
{
	return {{-TILE_SIZE / 2, -TILE_SIZE / 2}, {TILE_SIZE / 2, TILE_SIZE / 2}};
}

void Trapdoor::render()
{
	uint8_t layer = 0;
	Render::sprite(x, y, rotation, TILE_SIZE, m_SpriteID, layer);
	if(m_State == Button::State::Hover)
	{
		float        scale    = 35.0f;
		Vec2f        mousePos = Application::getCamera()->convertWindowToLevel(Event::getMousePos());
		std::string  name     = "Trapdoor";
		uint8_t      layer    = 6;
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