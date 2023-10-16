#include "Player.h"

#include "Projectile.h"
#include "Tile.h"

#include "Weapon.h"

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
			if(Application::isKeyPressed(GLFW_KEY_W) || Application::isKeyPressed(GLFW_KEY_UP))
			{
				ratio.y += 1.0f;
			}
			if(Application::isKeyPressed(GLFW_KEY_S) || Application::isKeyPressed(GLFW_KEY_DOWN))
			{
				ratio.y -= 1.0f;
			}
			if(Application::isKeyPressed(GLFW_KEY_A) || Application::isKeyPressed(GLFW_KEY_LEFT))
			{
				ratio.x -= 1.0f;
			}
			if(Application::isKeyPressed(GLFW_KEY_D) || Application::isKeyPressed(GLFW_KEY_RIGHT))
			{
				ratio.x += 1.0f;
			}

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

bool Player::eventCallback(const Application::Event &e)
{
	if(e.getType() == Application::EventType::keyInput)
	{
		const Application::KeyboardEvent &ne = static_cast<const Application::KeyboardEvent &>(e);
		if(ne.key == GLFW_KEY_SPACE && (ne.action == GLFW_PRESS || ne.action == GLFW_REPEAT) && m_CurrentWeapon != -1)
			m_Weapons[m_CurrentWeapon]->attack(m_Level, *this, m_Dir, ne.action == GLFW_REPEAT);
		// m_Level->addEntity(new Projectile(x, y, 1000.0f, 10.0f, 10.0f, m_Dir, this, m_Level));
	}
	return false;
}

#ifdef DEBUG
void Player::imGuiRender()
{
	ImGui::Checkbox("Ghost mode", &isGhost);
	ImGui::SliderFloat("Player Speed", &m_Speed, 0.0f, 100.0f);
}
#endif