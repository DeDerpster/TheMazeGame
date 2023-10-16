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
	m_NorthAnimation = std::make_unique<Render::AnimatedSprite>(2);
	m_NorthAnimation->addSprite(PLAYER_NORTH_1);
	m_NorthAnimation->addSprite(PLAYER_NORTH_2);

	m_SouthAnimation = std::make_unique<Render::AnimatedSprite>(2);
	m_SouthAnimation->addSprite(PLAYER_SOUTH_1);
	m_SouthAnimation->addSprite(PLAYER_SOUTH_2);

	m_EastAnimation = std::make_unique<Render::AnimatedSprite>(2);
	m_EastAnimation->addSprite(PLAYER_EAST_1);
	m_EastAnimation->addSprite(PLAYER_EAST_2);

	m_WestAnimation = std::make_unique<Render::AnimatedSprite>(2);
	m_WestAnimation->addSprite(PLAYER_WEST_1);
	m_WestAnimation->addSprite(PLAYER_WEST_2);
}

void Follower::findPath()
{
	Vec2f dest  = {following->getX(), following->getY()};
	Vec2f start = {x, y};
	if(!m_Level)
		Log::critical("Level is null", LOGINFO);
	std::vector<Vec2f> *path = m_Level->getPath(start, dest, m_CollisionBox);

	if(path->size() == 0)
	{
		Log::error("Path is empty!", LOGINFO);
		return;
	}

	// float tempX = 0;
	// float tempY = 0;
	// Vec2f ratio = {0.0f, 0.0f};

	// for(Vec2f vec : path)
	// Render::Sprite::getSprite(DEBUG_CIRCLE)->render(vec.x, vec.y, 0.0f, Tile::TILE_SIZE * ((float) X_STEP / 100.0f));
	// Application::renderBuffers();
	float availiableDist = m_Speed;
	while(availiableDist > 0.0f && path->size() > 0)
	{
		float distToNext = distBetweenVec2f({x, y}, path->back());
		Vec2f distVec    = {path->back().x - x, path->back().y - y};
		if(distToNext < availiableDist)
		{
			availiableDist -= distToNext;
			move(distVec.x, distVec.y);
			path->pop_back();
		}
		else
		{
			availiableDist = 0.0f;
			float timesBy  = m_Speed / distToNext;
			move(distVec.x * timesBy, distVec.y * timesBy);
		}
	}

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
	delete path;
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
			// std::thread pathThread(&Follower::findPath, this);
			findPath();
			// pathThread.detach();
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
			Render::Sprite::getSprite(PLAYER_NORTH)->render(RENDER_ARGUMENTS);
			break;
		case Direction::SOUTH:
			Render::Sprite::getSprite(PLAYER_SOUTH)->render(RENDER_ARGUMENTS);
			break;
		case Direction::EAST:
			Render::Sprite::getSprite(PLAYER_EAST)->render(RENDER_ARGUMENTS);
			break;
		default:
			Render::Sprite::getSprite(PLAYER_WEST)->render(RENDER_ARGUMENTS);
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

bool Follower::eventCallback(const Application::Event &e)
{
	return false;
}