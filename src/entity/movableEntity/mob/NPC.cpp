#include "NPC.h"

#include <thread>
#include <vector>

#include "Application.h"
#include "FireStaff.h"
#include "KeyDefinitions.h"
#include "Level.h"
#include "MessageManager.h"
#include "RandomGen.h"
#include "Utils.h"
#include "Player.h"

NPC::NPC()
	: m_Name("Bob"), following(nullptr), attacking(nullptr), findingPath(false), isRunningAway(false)
{
	generateInventory();
}

NPC::NPC(float x, float y)
	: Mob(x, y), m_Name("Bob"), following(nullptr), attacking(nullptr), findingPath(false), isRunningAway(false)
{
	generateInventory();
}

NPC::NPC(float x, float y, Level *level)
	: Mob(x, y, level), m_Name("Bob"), following(nullptr), attacking(nullptr), findingPath(false), isRunningAway(false)
{
	generateInventory();
}

NPC::NPC(float x, float y, Level *level, uint16_t spriteID)
	: Mob(x, y, level, spriteID), m_Name("Bob"), following(nullptr), attacking(nullptr), findingPath(false), isRunningAway(false)
{
	generateInventory();
}

NPC::~NPC()
{
}

void NPC::generateInventory()
{
	pickUp(new FireStaff());
}
void NPC::findPath()
{
	Vec2f dest  = {following->getX(), following->getY()};
	Vec2f start = {x, y};
	if(!m_Level)
		Log::critical("Level is null", LOGINFO);
	std::vector<Vec2f> *path = m_Level->getPath(start, dest, getMovingCollisionBox());

	if(path->size() == 0)
	{
		Log::error("Path is empty!", LOGINFO);
		return;
	}

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

	findingPath = false;
	delete path;
}

void NPC::follow()
{
	if(!following)
	{
		Log::warning("Trying to follow a nullptr!");
		return;
	}
	float xDif        = following->getX() - x;
	float yDif        = following->getY() - y;
	float minDistAway = (TILE_SIZE / 3) * 2;
	if(!findingPath && (xDif < -minDistAway || xDif > minDistAway || yDif < -minDistAway || yDif > minDistAway))
	{
		findingPath = true;
		// std::thread t1(&NPC::findPath, this);
		// t1.detach();
		findPath();
	}
	else
		isMoving = false;
}

void NPC::attack()
{
	if(!attacking)
	{
		Log::warning("Trying to attack a nullptr!");
		return;
	}
	if(!isRunningAway && getStamina() == 0)
		isRunningAway = true;
	else if(isRunningAway && getStamina() > 3 * getMaxStamina() / 4)
		isRunningAway = false;

	bool moveInX = false;
	if(fabs(x - attacking->getX()) < fabs(y - attacking->getY()))
		moveInX = true;

	Vec2f ratio = {0, 0};
	if(x < attacking->getX() && x + m_Speed < attacking->getX())
	{
		if(moveInX)
			ratio.x += 1.0f;
		else if(abs(x - attacking->getX()) < 200)
			ratio.x -= 1.0f;
		else if(abs(x - attacking->getX()) > 250)
			ratio.x += 1.0f;
	}
	else if(x > attacking->getX() && x + m_Speed > attacking->getX())
	{
		if(moveInX)
			ratio.x -= 1.0f;
		else if(abs(x - attacking->getX()) < 200)
			ratio.x += 1.0f;
		else if(abs(x - attacking->getX()) > 250)
			ratio.x -= 1.0f;
	}
	else
	{
		if(y > attacking->getY())
			m_Dir = Direction::south;
		else
			m_Dir = Direction::north;
		useCurrentWeapon(true);
	}

	if(y < attacking->getY() && y + m_Speed < attacking->getY())
	{
		if(!moveInX)
			ratio.y += 1.0f;
		else if(abs(y - attacking->getY()) < 200)
			ratio.y -= 1.0f;
		else if(abs(y - attacking->getY()) > 250)
			ratio.y += 1.0f;
	}
	else if(y > attacking->getY() && y + m_Speed > attacking->getY())
	{
		if(!moveInX)
			ratio.y -= 1.0f;
		else if(abs(y - attacking->getY()) < 200)
			ratio.y += 1.0f;
		else if(abs(y - attacking->getY()) > 250)
			ratio.y -= 1.0f;
	}
	else
	{
		if(x > attacking->getX())
			m_Dir = Direction::west;
		else
			m_Dir = Direction::east;
		useCurrentWeapon(true);
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

void NPC::update()
{
	if(following && !findingPath)
	{
		follow();
	}
	else if(attacking && (int) attacking->getX() / (TILE_SIZE * ROOM_SIZE) == (int) x / (TILE_SIZE * ROOM_SIZE) && (int) attacking->getY() / (TILE_SIZE * ROOM_SIZE) == (int) y / (TILE_SIZE * ROOM_SIZE))
	{
		attack();
	}
	else
		isMoving = false;

	Mob::update();
}

void NPC::render()
{
	Mob::render();
}

#ifdef DEBUG
void NPC::imGuiRender()
{
}
#endif

bool NPC::eventCallback(const Event::Event &e)
{
	if(e.getType() == Event::EventType::mouseClicked)
	{
		const Event::MouseClickedEvent &ne = static_cast<const Event::MouseClickedEvent &>(e);

		Vec2f convPos = Application::getCamera()->convertWindowToLevel(ne.pos);

		Player *player = m_Level->getPlayer();
		if(doesPointIntersectWithBox(convPos, {x, y}, {{-width / 2, -height / 2}, {width / 2, height / 2}}) && distBetweenVec2f({player->getX(), player->getY() - player->getWidth() / 2}, {x, y}) < 5.0f * TILE_SIZE)
		{
			if(!attacking && !following)
			{
				Event::ChangeGUIActiveLayer e(InGameGUILayer::npcInteraction);
				Application::callEvent(e, true);

				MessageManager::sendMessage("NPC: Can I follow you?", MessageManager::Priority::Low);

				return true;
			}
			else if(following == m_Level->getPlayer())
			{
				Event::ChangeGUIActiveLayer e1(InGameGUILayer::npcInventory);
				Application::callEvent(e1, true);

				Event::ChestOpenedEvent e2(&m_Inventory, nullptr, GUIInventoryIDCode::inventory);
				Application::callEvent(e2, true);

				Event::ChestOpenedEvent e3(&m_Weapons, &m_CurrentWeapon, GUIInventoryIDCode::weapons);
				Application::callEvent(e3, true);
				return true;
			}

			return false;
		}
	}
	else if(e.getType() == Event::EventType::playerResponse && !following && !attacking)
	{
		const Event::PlayerResponse &ne = static_cast<const Event::PlayerResponse &>(e);

		if(ne.response == Event::PlayerResponse::Response::reject)
		{
			// TODO: Have this have a random change to attack the player
			int r = Random::getNum(0, 4);   // TODO: Base this upon stats?

			if(r == 1)
			{
				attacking = m_Level->getPlayer();

				Event::ShowAltTileEvent e(true);
				Application::callEvent(e);

				MessageManager::sendMessage("NPC: How rude!? I will kill you now!", MessageManager::Priority::Medium);
			}
			else
				MessageManager::sendMessage("NPC: Oh okay, fine be like that.", MessageManager::Priority::Low);
		}
		else
		{
			MessageManager::sendMessage("NPC: Yay! I know we will be best buds!", MessageManager::Priority::Low);
			// TODO: Make this go through the player to see if they can follow
			following = m_Level->getPlayer();
		}

		Event::ChangeGUIActiveLayer e(InGameGUILayer::overlay);
		Application::callEvent(e, true);

		return true;
	}
	return Mob::eventCallback(e);
}