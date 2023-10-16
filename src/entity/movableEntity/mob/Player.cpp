#include "Player.h"

#include "entity/movableEntity/projectile/Projectile.h"

#include "event/input/Keyboard.h"

Player::Player()
{
}

Player::Player(float x, float y)
	: Mob(x, y)
{
}

Player::Player(float x, float y, Level *level)
	: Mob(x, y, level)
{
}

Player::~Player()
{
}

void Player::update()
{
	if(isInControl)
	{
		Vec2f ratio = {0, 0};
		if(Event::isKeyPressed(Event::KeyboardKey::W) || Event::isKeyPressed(Event::KeyboardKey::Up))
			ratio.y += 1.0f;
		if(Event::isKeyPressed(Event::KeyboardKey::S) || Event::isKeyPressed(Event::KeyboardKey::Down))
			ratio.y -= 1.0f;
		if(Event::isKeyPressed(Event::KeyboardKey::A) || Event::isKeyPressed(Event::KeyboardKey::Left))
			ratio.x -= 1.0f;
		if(Event::isKeyPressed(Event::KeyboardKey::D) || Event::isKeyPressed(Event::KeyboardKey::Right))
			ratio.x += 1.0f;

		if(ratio.x != 0 || ratio.y != 0)
			move(ratio);
		else
			isMoving = false;

		Mob::update();
	}
}

void Player::render()
{
	Mob::render();
}

bool Player::eventCallback(const Event::Event &e)
{
	switch(e.getType())
	{
	case Event::EventType::KeyInput:
	{
		const Event::KeyboardEvent &ne = static_cast<const Event::KeyboardEvent &>(e);
		if(ne.key == Event::KeyboardKey::Space && (ne.action == Event::Action::Press || ne.action == Event::Action::Repeat) && m_CurrentWeapon != -1)
		{
			useCurrentWeapon(ne.action == Event::Action::Repeat);
			return true;
		}

		return false;
	}

	default:
		return Mob::eventCallback(e);
	}
}

void Player::setFollowing(Mob *following)
{
	Log::warning("Trying to set player follower!");
}

void Player::setEnemy(Mob *enemy)
{
	Log::warning("Trying to set player enemy!");
}

#ifdef DEBUG
void Player::imGuiRender()
{
	ImGui::Checkbox("Ghost mode", &isGhost);
	ImGui::SliderFloat("Player Speed", &m_Speed, 0.0f, 100.0f);
}
#endif