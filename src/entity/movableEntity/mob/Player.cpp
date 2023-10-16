#include "Player.h"

#include "entity/movableEntity/projectile/Projectile.h"

#include "event/input/Keyboard.h"

Player::Player()
{
}

Player::Player(float x, float y)
	: Mob(x, y, false)
{
}

Player::Player(float x, float y, Level *level)
	: Mob(x, y, level, false)
{
}

Player::~Player()
{
}

void Player::update()
{
	if(isInControl)
	{
		// Sees what keys are being pressed and updates position accordingly
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
			// If space is pressed it will attack
			useCurrentWeapon(ne.action == Event::Action::Repeat);
			return true;
		}   // Changes active weapon based on keys 1, 2 and 3
		else if(ne.key == Event::KeyboardKey::Num1 && ne.action == Event::Action::Press && m_Weapons.size() > 0)
		{
			m_CurrentWeapon = 0;
			return true;
		}
		else if(ne.key == Event::KeyboardKey::Num2 && ne.action == Event::Action::Press && m_Weapons.size() > 1)
		{
			m_CurrentWeapon = 1;
			return true;
		}
		else if(ne.key == Event::KeyboardKey::Num3 && ne.action == Event::Action::Press && m_Weapons.size() > 2)
		{
			m_CurrentWeapon = 2;
			return true;
		}

		return false;
	}

	default:
		return Mob::eventCallback(e);
	}
}

// Calls a warning if trying to set the following or enemy of the player
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