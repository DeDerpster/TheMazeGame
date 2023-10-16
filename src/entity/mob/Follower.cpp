#include "Follower.h"
#include "Application.h"
#include "Tile.h"

#include <thread>

Follower::Follower()
	: m_Name("Bob")
{
	setupAnimations();
}

Follower::Follower(float x, float y)
	: Mob(x, y), m_Name("Bob")
{
	setupAnimations();
}

Follower::Follower(float x, float y, Level *level)
	: Mob(x, y, level), m_Name("Bob")
{
	setupAnimations();
}

Follower::~Follower()
{
}

void Follower::setupAnimations()
{
	Log::info("Setting up Animations for follower");
	m_NorthAnimation = std::make_unique<Sprite::AnimatedSprite>(2);
	m_NorthAnimation->addSprite(PLAYER_NORTH_1);
	m_NorthAnimation->addSprite(PLAYER_NORTH_2);

	m_SouthAnimation = std::make_unique<Sprite::AnimatedSprite>(2);
	m_SouthAnimation->addSprite(PLAYER_SOUTH_1);
	m_SouthAnimation->addSprite(PLAYER_SOUTH_2);

	m_EastAnimation = std::make_unique<Sprite::AnimatedSprite>(2);
	m_EastAnimation->addSprite(PLAYER_EAST_1);
	m_EastAnimation->addSprite(PLAYER_EAST_2);

	m_WestAnimation = std::make_unique<Sprite::AnimatedSprite>(2);
	m_WestAnimation->addSprite(PLAYER_WEST_1);
	m_WestAnimation->addSprite(PLAYER_WEST_2);
}

void Follower::findPath()
{
	Vec2f dest  = {following->getX(), following->getY()};
	Vec2f start = {x, y};
	if(!m_Level)
		Log::critical("Level is null", LOGINFO);
	std::vector<Vec2f> path = m_Level->getPath(start, dest, m_CollisionBox);

	if(path.size() == 0)
	{
		Log::error("Path is empty!", LOGINFO);
		return;
	}

	float tempX = 0;
	float tempY = 0;

	// for(Vec2f vec : path)
	// Sprite::Sprite::getSprite(DEBUG_CIRCLE)->render(vec.x, vec.y, 0.0f, Tile::TILE_SIZE * ((float) X_STEP / 100.0f));
	// Application::renderBuffers();
	if(path.back().y > y)
	{
		if((float) path.back().y - y > m_Speed)
			tempY += m_Speed;
		else
			tempY += path.back().y - y;
	}
	if(path.back().y < y)
	{
		if(y - (float) path.back().y > m_Speed)
			tempY -= m_Speed;
		else
			tempY -= y - path.back().y;
	}
	if(path.back().x < x)
	{
		if(x - path.back().x > m_Speed)
			tempX -= m_Speed;
		else
			tempX -= x - path.back().x;
	}
	if(path.back().x > x)
	{
		if(path.back().x - x > m_Speed)
			tempX += m_Speed;
		else
			tempX += path.back().x - x;
	}

	if(tempX != 0 || tempY != 0)
	{
		move(tempX, tempY);
		if(x == path.back().x && y == path.back().y)
		{
			// Log::info("Removing last vec in list");
			path.pop_back();
		}
	}
	else
		isMoving = false;

	if(isMoving)
	{
		switch(m_Dir)
		{
		case Direction::NORTH:
			m_NorthAnimation->update();
			break;
		case Direction::SOUTH:
			m_SouthAnimation->update();
			break;
		case Direction::EAST:
			m_EastAnimation->update();
			break;
		default:
			m_WestAnimation->update();
			break;
		}
	}
	findingPath = false;
}

void Follower::update()
{
	if(following && !findingPath)
	{
		float xDif        = following->getX() - x;
		float yDif        = following->getY() - y;
		float minDistAway = (Tile::TILE_SIZE / 3) * 2;
		if(xDif < -minDistAway || xDif > minDistAway || yDif < -minDistAway || yDif > minDistAway)
		{
			findingPath = true;
			std::thread pathThread(&Follower::findPath, this);
			pathThread.detach();
		}
		else
			isMoving = false;
	}
}

void Follower::render()
{
#define RENDER_ARGUMENTS x, y, 0.0f, Tile::TILE_SIZE * 1.25f
	if(isMoving)
	{
		switch(m_Dir)
		{
		case Direction::NORTH:
			m_NorthAnimation->render(RENDER_ARGUMENTS);
			break;
		case Direction::SOUTH:
			m_SouthAnimation->render(RENDER_ARGUMENTS);
			break;
		case Direction::EAST:
			m_EastAnimation->render(RENDER_ARGUMENTS);
			break;
		default:
			m_WestAnimation->render(RENDER_ARGUMENTS);
			break;
		}
	}
	else
	{
		switch(m_Dir)
		{
		case Direction::NORTH:
			Sprite::Sprite::getSprite(PLAYER_NORTH)->render(RENDER_ARGUMENTS);
			break;
		case Direction::SOUTH:
			Sprite::Sprite::getSprite(PLAYER_SOUTH)->render(RENDER_ARGUMENTS);
			break;
		case Direction::EAST:
			Sprite::Sprite::getSprite(PLAYER_EAST)->render(RENDER_ARGUMENTS);
			break;
		default:
			Sprite::Sprite::getSprite(PLAYER_WEST)->render(RENDER_ARGUMENTS);
			break;
		}
	}

	// Log::variable("path.size()", (int) path.size());
}

#ifdef DEBUG
void Follower::imGuiRender()
{
}
#endif

void Follower::eventCallback(Application::Event &e)
{
}