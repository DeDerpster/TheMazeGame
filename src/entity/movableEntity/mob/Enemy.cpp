#include "Enemy.h"

#include "FireStaff.h"
#include "Player.h"
#include "Room.h"
#include "Tile.h"

Enemy::Enemy()
{
}

Enemy::Enemy(float x, float y)
	: Mob(x, y, 5.0f, nullptr, SPRITE_PLAYER)
{
	generateInventory();
}

Enemy::Enemy(float x, float y, Level *level)
	: Mob(x, y, 5.0f, level, SPRITE_PLAYER)
{
	generateInventory();
}

Enemy::Enemy(float x, float y, Level *level, uint16_t spriteID)
	: Mob(x, y, 5.0f, level, spriteID)
{
	generateInventory();
}

Enemy::~Enemy()
{
}

void Enemy::generateInventory()
{
	pickUp(new FireStaff());
}

void Enemy::update()
{
	Player *player = m_Level->getPlayer();
	if((int) player->getX() / (Tile::TILE_SIZE * ROOM_SIZE) == (int) x / (Tile::TILE_SIZE * ROOM_SIZE) && (int) player->getY() / (Tile::TILE_SIZE * ROOM_SIZE) == (int) y / (Tile::TILE_SIZE * ROOM_SIZE))
	{
		if(!isRunningAway && getStamina() == 0)
			isRunningAway = true;
		else if(isRunningAway && getStamina() > 3 * getMaxStamina() / 4)
			isRunningAway = false;

		bool moveInX = false;
		if(fabs(x - player->getX()) < fabs(y - player->getY()))
			moveInX = true;

		Vec2f ratio = {0, 0};
		if(x < player->getX() && x + m_Speed < player->getX())
		{
			if(moveInX)
				ratio.x += 1.0f;
			else if(abs(x - player->getX()) < 200)
				ratio.x -= 1.0f;
			else if(abs(x - player->getX()) > 250)
				ratio.x += 1.0f;
		}
		else if(x > player->getX() && x + m_Speed > player->getX())
		{
			if(moveInX)
				ratio.x -= 1.0f;
			else if(abs(x - player->getX()) < 200)
				ratio.x += 1.0f;
			else if(abs(x - player->getX()) > 250)
				ratio.x -= 1.0f;
		}
		else
		{
			if(y > player->getY())
				m_Dir = Direction::south;
			else
				m_Dir = Direction::north;
			m_Weapons[m_CurrentWeapon]->attack(m_Level, *this, m_Dir, true);
		}

		if(y < player->getY() && y + m_Speed < player->getY())
		{
			if(!moveInX)
				ratio.y += 1.0f;
			else if(abs(y - player->getY()) < 200)
				ratio.y -= 1.0f;
			else if(abs(y - player->getY()) > 250)
				ratio.y += 1.0f;
		}
		else if(y > player->getY() && y + m_Speed > player->getY())
		{
			if(!moveInX)
				ratio.y -= 1.0f;
			else if(abs(y - player->getY()) < 200)
				ratio.y += 1.0f;
			else if(abs(y - player->getY()) > 250)
				ratio.y -= 1.0f;
		}
		else
		{
			if(x > player->getX())
				m_Dir = Direction::west;
			else
				m_Dir = Direction::east;
			m_Weapons[m_CurrentWeapon]->attack(m_Level, *this, m_Dir, true);
		}

		if(ratio.x != 0.0f || ratio.y != 0.0f)
		{
			if(isRunningAway)   // TODO: PLS CHANGE THIS
				move({-ratio.x, -ratio.y});
			else
				move(ratio);
		}
		else
		{
			if(isRunningAway)
				move({-1.0f, -1.0f});
			else
				isMoving = false;
		}
	}
	else
		isMoving = false;
	Mob::update();
}

void Enemy::render()
{
	Mob::render();
}

bool Enemy::eventCallback(const Event::Event &e)
{
	return Mob::eventCallback(e);
}

#ifdef DEBUG
void Enemy::imGuiRender()
{
}
#endif