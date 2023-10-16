#include "NPC.h"

#include <thread>
#include <vector>

#include "Application.h"
#include "KeyDefinitions.h"
#include "RandomGen.h"
#include "Utils.h"
#include "layer/MessageManager.h"
#include "level/Level.h"

#include "Player.h"

#include "item/weapon/general/Boomerang.h"
#include "item/weapon/general/Bow.h"
#include "item/weapon/general/Crossbow.h"
#include "item/weapon/general/Sling.h"
#include "item/weapon/staff/DarkStaff.h"
#include "item/weapon/staff/FireStaff.h"
#include "item/weapon/staff/FrostStaff.h"

#include "event/game/ChestOpened.h"
#include "event/game/MazeMoved.h"
#include "event/game/PlayerResponse.h"
#include "event/game/ShowAlternatives.h"
#include "event/input/Mouse.h"
#include "event/menu/ChangeGUIMenu.h"

static float getRatioForAttacking(float pos, float ePos, float buffer)
{
	float dist = ePos - pos;
	if(std::fabs(dist) > buffer)
		return dist > 0 ? 1.0f : -1.0f;
	else
		return 0.0f;
}

static Sprite::ID getSpriteID(NPC::Type type, NPC::Race race)
{
	Sprite::ID id;
	switch(type)
	{
	case NPC::Type::Follower:
		id = Sprite::ID::followerFrost;
		break;
	case NPC::Type::Enemy:
		id = Sprite::ID::enemyFrost;
		break;

	default:
		Log::warning("Unknown npc type!");
		id = Sprite::ID::errorID;
		break;
	}

	switch(race)
	{
	case NPC::Race::Frost:
		id += SPRITE_FROST;
		break;
	case NPC::Race::Fire:
		id += SPRITE_FIRE;
		break;
	case NPC::Race::Dark:
		id += SPRITE_DARK;
		break;

	default:
		Log::warning("Unknown npc race!");
		break;
	}

	return id;
}

static Sprite::ID genSpriteID(NPC::Type type)
{
	int r = Random::getNum(0, 2);
	return getSpriteID(type, static_cast<NPC::Race>(r));
}

static NPC::Race getRace(NPC::Type type, Sprite::ID spriteID)
{
	switch(type)
	{
	case NPC::Type::Follower:
		spriteID -= Sprite::ID::followerFrost;
		break;
	case NPC::Type::Enemy:
		spriteID -= Sprite::ID::enemyFrost;
		break;
	default:
		Log::warning("Unknown npc type!");
		break;
	}

	switch(spriteID)
	{
	case SPRITE_FROST:
		return NPC::Race::Frost;
	case SPRITE_FIRE:
		return NPC::Race::Fire;
	case SPRITE_DARK:
		return NPC::Race::Dark;

	default:
		Log::warning("Unknown spriteID given!");
		break;
	}
	return NPC::Race::Frost;
}

NPC::NPC()
	: m_Name("Bob"), m_Attack(AttackMove::None), m_Center({0.0f, 0.0f}), m_NextPos({0.0f, 0.0f}), m_NextPosActive(false), m_TimeSinceMoved(0), m_WaitFor(0), isRunningAway(false)
{
	generateInventory(Race::Fire);
}

NPC::NPC(float x, float y, Level *level, Type type)
	: Mob(x, y, level, genSpriteID(type)), m_Name("Bob"), m_Attack(AttackMove::None), m_Center({x, y}), m_NextPos({0.0f, 0.0f}), m_NextPosActive(false), m_TimeSinceMoved(0), m_WaitFor(0), isRunningAway(false)
{
	generateInventory(getRace(type, m_SpriteID));
}

NPC::NPC(float x, float y, Level *level, Type type, Race race)
	: Mob(x, y, level, getSpriteID(type, race)), m_Name("Bob"), m_Attack(AttackMove::None), m_Center({x, y}), m_NextPos({0.0f, 0.0f}), m_NextPosActive(false), m_TimeSinceMoved(0), m_WaitFor(0), isRunningAway(false)
{
	generateInventory(race);
}

NPC::~NPC()
{
}

void NPC::generateInventory(Race race)
{
	int r = Random::getNum(0, 2);
	if(r == 0)
	{
		int s = Random::getNum(0, 3);
		switch(s)
		{
		case 0:
			pickUp(new Boomerang());
			break;
		case 1:
			pickUp(new Bow());
			break;
		case 2:
			pickUp(new Crossbow());
			break;
		default:
			pickUp(new Sling());
			break;
		}
	}
	else
	{
		switch(race)
		{
		case Race::Frost:
			pickUp(new FrostStaff());
			break;
		case Race::Fire:
			pickUp(new FireStaff());
			break;
		case Race::Dark:
			pickUp(new DarkStaff());
			break;

		default:
			Log::warning("Unknown race when generating weapons!");
			break;
		}
	}
}

void NPC::findPath(Vec2f dest, float speed)
{
	Vec2f start = {x, y};
	if(!m_Level)
		Log::critical("Level is null", LOGINFO);
	std::vector<Vec2f> *path = m_Level->getPath(start, dest, getMovingCollisionBox());

	// path->pop_back();
	if(path->size() == 0 || (path->size() == 1 && distBetweenVec2f({x, y}, path->back()) < speed))
	{
		isMoving = false;
		return;
	}

	float availiableDist = speed;
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
			float timesBy  = speed / distToNext;
			move(distVec.x * timesBy, distVec.y * timesBy);
		}
	}

	delete path;
}

void NPC::follow()
{
	if(!m_Following)
	{
		Log::warning("Trying to follow a nullptr!");
		return;
	}

	float minDistAway = (TILE_SIZE / 3) * 2;
	if(distBetweenVec2f({m_Following->getX(), m_Following->getY()}, {x, y}) > minDistAway)
		findPath({m_Following->getX(), m_Following->getY()}, m_Speed);
	else
		isMoving = false;
}

void NPC::roam()
{
	if(!m_NextPosActive)
		generateNextPos();

	float minDistAway = TILE_SIZE / 3;

	if(distBetweenVec2f(m_NextPos, {x, y}) > minDistAway)
		findPath({m_NextPos.x, m_NextPos.y}, m_Speed / 2);
	else
		isMoving = false;

	// This is here just to make sure that if the point cannot be achieved then it picks a new one
	m_TimeSinceMoved++;
	if(m_TimeSinceMoved > m_WaitFor)
		generateNextPos();
}

void NPC::generateNextPos()
{
	m_TimeSinceMoved  = 0;
	m_WaitFor         = Random::getNum(120, 500);
	m_NextPosActive   = true;
	float range       = TILE_SIZE * 2;
	float xPercentage = Random::getNum(-100, 100) / 100.0f;
	float yPercentage = Random::getNum(-100, 100) / 100.0f;
	m_NextPos         = {m_Center.x + range * xPercentage, m_Center.y + range * yPercentage};
}

void NPC::attack()
{
	if(!m_Enemy)
	{
		Log::warning("Trying to attack a nullptr!");
		return;
	}

	// This determines what it should do next
	m_TimeSinceMoved++;
	if(m_TimeSinceMoved < 0)
		return;

	if(m_CurrentWeapon == -1 || distBetweenVec2f({x, y}, {m_Enemy->getX(), m_Enemy->getY()}) < TILE_SIZE && m_Attack != AttackMove::GoToPoint)
		m_Attack = AttackMove::RunAway;
	else if(getHealth() > 0.8 * getMaxHealth())
		m_Attack = AttackMove::Attack;
	else if(m_TimeSinceMoved > m_WaitFor)
	{
		if(getStamina() < 0.2 * getMaxStamina() && getHealth() < 0.2 * getMaxHealth())
		{
			m_Attack  = AttackMove::RunAway;   // TODO: Check for potions and use them
			m_WaitFor = Random::getNum(150, 500);
		}
		else if(getStamina() < 0.2 * getMaxStamina())
		{
			m_Attack = AttackMove::Dodge;

			m_WaitFor = Random::getNum(200, 500);
		}
		else if(Random::getNum(0, 3) == 0)
		{
			m_Attack = AttackMove::GoToPoint;
			// Sets the center of the room
			m_Center = {(float) ((int) x / ROOM_PIXEL_SIZE) * ROOM_PIXEL_SIZE + ROOM_PIXEL_SIZE / 2, (float) ((int) y / ROOM_PIXEL_SIZE) * ROOM_PIXEL_SIZE + ROOM_PIXEL_SIZE / 2};

			m_NextPosActive   = true;
			float range       = TILE_SIZE * 1.5f;
			float xPercentage = Random::getNum(-100, 100) / 100.0f;
			float yPercentage = Random::getNum(-100, 100) / 100.0f;
			m_NextPos         = {m_Center.x + range * xPercentage, m_Center.y + range * yPercentage};

			m_WaitFor = 240;
		}
		else
		{
			m_Attack  = AttackMove::Attack;
			m_WaitFor = Random::getNum(200, 500);
		}
		m_TimeSinceMoved = -10;
	}

	isMoving = false;

	// The NPC attacking
	switch(m_Attack)
	{
	case AttackMove::Dodge:
	{
		// TODO: Take stamina into account etc
		float range      = TILE_SIZE * 2;
		auto [dir, proj] = m_Level->getDirOfProjInRange(x, y, range);
		if(proj)
		{
			float xa = 0;
			float ya = 0;
			if(dir == Direction::north || dir == Direction::south)
			{
				float difX = x - m_Enemy->getX();

				xa = difX > 0 ? m_Speed : -m_Speed;
			}
			else
			{
				float difY = y - m_Enemy->getY();

				ya = difY > 0 ? m_Speed : -m_Speed;
			}

			move(xa, ya);
		}
		break;
	}

	case AttackMove::RunAway:
	{
		float difX = x - m_Enemy->getX();
		float difY = y - m_Enemy->getY();
		if(std::fabs(difX) < 10.0f)
			difX += difX > 0 ? 20.0f : -20.0f;
		if(std::fabs(difY) < 10.0f)
			difY += difY > 0 ? 20.0f : -20.0f;
		if(canMove({difX, difY}))
			move({difX, difY});
		else
			goToPointInRoom();
		break;
	}

	case AttackMove::RunAwayAlongX:
	{
		float difX = x - m_Enemy->getX();
		if(canMove({difX, 0.0f}))
			move({difX, 0.0f});
		else
			goToPointInRoom();
		break;
	}

	case AttackMove::RunAwayAlongY:
	{
		float difY = y - m_Enemy->getY();
		if(canMove({0.0f, difY}))
			move({0.0f, difY});
		else
			goToPointInRoom();
		break;
	}

	case AttackMove::GoToPoint:
	{
		float xDif        = m_NextPos.x - x;
		float yDif        = m_NextPos.y - y;
		float minDistAway = TILE_SIZE / 3;

		if((xDif < -minDistAway || xDif > minDistAway || yDif < -minDistAway || yDif > minDistAway))
			findPath({m_NextPos.x, m_NextPos.y}, m_Speed * (getStamina() / getMaxStamina()) / 2);
		else
			m_Attack = AttackMove::None;

		break;
	}

	default:   // Attack
	{
		// TODO: Check if friend is in line of fire
		float buffer = TILE_SIZE / 4.0f;
		Vec2f ratio  = {getRatioForAttacking(x, m_Enemy->getX(), buffer), getRatioForAttacking(y, m_Enemy->getY(), buffer)};

		if((ratio.y == 0.0f || ratio.x == 0.0f))   // Attacks if can
		{
			if(m_Weapons[m_CurrentWeapon]->canUse())
			{
				if(ratio.x == 0.0f)
				{

					if(y > m_Enemy->getY())
						m_Dir = Direction::south;
					else
						m_Dir = Direction::north;
				}
				else
				{
					if(x > m_Enemy->getX())
						m_Dir = Direction::west;
					else
						m_Dir = Direction::east;
				}

				useCurrentWeapon(true);
			}
		}
		else
		{
			float distX = std::fabs(ratio.x - m_Enemy->getX());
			float distY = std::fabs(ratio.y - m_Enemy->getX());

			if(distX > distY)
			{
				if(distX < TILE_SIZE)   // This checks that the distance is within a given range and corrects accordingly
				{
					m_TimeSinceMoved = 0;
					m_WaitFor        = 60;
					m_Attack         = AttackMove::RunAwayAlongX;
				}
				else if(std::fabs(ratio.x - m_Enemy->getX()) < 2 * TILE_SIZE)
					ratio.x = 0.0f;
			}
			else
			{
				if(distY < TILE_SIZE)
				{
					m_TimeSinceMoved = 0;
					m_WaitFor        = 60;
					m_Attack         = AttackMove::RunAwayAlongX;
				}
				else if(distY < 2 * TILE_SIZE)
					ratio.y = 0.0f;
			}

			if(m_Attack == AttackMove::Attack)
				move(ratio);
		}
		break;
	}
	}
}

void NPC::update()
{
	if(m_Enemy && (int) m_Enemy->getX() / (TILE_SIZE * ROOM_SIZE) == (int) x / (TILE_SIZE * ROOM_SIZE) && (int) m_Enemy->getY() / (TILE_SIZE * ROOM_SIZE) == (int) y / (TILE_SIZE * ROOM_SIZE))
		attack();
	else
	{
		if(m_Attack != AttackMove::None)
			m_Attack = AttackMove::None;

		if(m_Following)
			follow();
		else
			roam();
	}

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

void NPC::goToPointInRoom()
{

	m_Attack         = AttackMove::GoToPoint;
	m_TimeSinceMoved = 0;
	// Sets the center of the room
	m_Center = {(float) ((int) x / ROOM_PIXEL_SIZE) * ROOM_PIXEL_SIZE + ROOM_PIXEL_SIZE / 2, (float) ((int) y / ROOM_PIXEL_SIZE) * ROOM_PIXEL_SIZE + ROOM_PIXEL_SIZE / 2};

	m_NextPosActive   = true;
	float range       = TILE_SIZE * 1.5f;
	float xPercentage = Random::getNum(-100, 100) / 100.0f;
	float yPercentage = Random::getNum(-100, 100) / 100.0f;
	m_NextPos         = {m_Center.x + range * xPercentage, m_Center.y + range * yPercentage};
}

void NPC::setFollowing(Mob *following)
{
	m_Center = {x, y};
	Mob::setFollowing(following);
}

bool NPC::eventCallback(const Event::Event &e)
{
	switch(e.getType())
	{
	case Event::EventType::MouseClicked:
	{
		const Event::MouseClickedEvent &ne = static_cast<const Event::MouseClickedEvent &>(e);

		Vec2f convPos = Application::getCamera()->convertWindowToLevel(ne.pos);

		Player *player = m_Level->getPlayer();
		if(doesPointIntersectWithBox(convPos, {x, y}, {{-width / 2, -height / 2}, {width / 2, height / 2}}) && distBetweenVec2f({player->getX(), player->getY() - player->getWidth() / 2}, {x, y}) < 5.0f * TILE_SIZE)
		{
			if(!m_Enemy && !m_Following)
			{
				Event::ChangeGUIActiveLayerEvent e(InGameGUILayer::npcInteraction);
				Application::callEvent(e, Event::CallType::Overlay);

				MessageManager::sendMessage("NPC: Can I follow you?", MessageManager::Priority::Low);

				return true;
			}
			else if(m_Following == m_Level->getPlayer())
			{
				Event::ChangeGUIActiveLayerEvent e1(InGameGUILayer::npcInventory);
				Application::callEvent(e1, Event::CallType::Overlay);

				Event::ChestOpenedEvent e2(&m_Inventory, nullptr, GUIInventoryIDCode::inventory);
				Application::callEvent(e2, Event::CallType::Overlay);

				Event::ChestOpenedEvent e3(&m_Weapons, &m_CurrentWeapon, GUIInventoryIDCode::weapons);
				Application::callEvent(e3, Event::CallType::Overlay);
				return true;
			}

			return false;
		}

		return false;
	}

	case Event::EventType::PlayerResponse:
	{
		if(m_Following || m_Enemy)
			return false;

		const Event::PlayerResponseEvent &ne = static_cast<const Event::PlayerResponseEvent &>(e);
		if(ne.response == Event::PlayerResponseEvent::Response::reject)
		{
			int r = Random::getNum(0, 4);

			if(r == 1)
			{
				m_Enemy = m_Level->getPlayer();

				Event::ShowAltTileEvent e(true);
				Application::callEvent(e);

				MessageManager::sendMessage("NPC: How rude!? I will kill you now!", MessageManager::Priority::Medium);
			}
			else
				MessageManager::sendMessage("NPC: Oh okay, fine be like that.", MessageManager::Priority::Low);
		}
		else
		{
			if(m_Level->getPlayer()->addFollower(this))
			{
				MessageManager::sendMessage("NPC: Yay! I know we will be best buds!", MessageManager::Priority::Low);
				m_Following = m_Level->getPlayer();
			}
			else
				MessageManager::sendMessage("NPC: Looks like you already have a follower", MessageManager::Priority::Medium);
		}

		Event::ChangeGUIActiveLayerEvent e(InGameGUILayer::overlay);
		Application::callEvent(e, Event::CallType::Overlay);

		return true;
	}

	case Event::EventType::MazeMoved:
	{
		const Event::MazeMovedEvent &ne = static_cast<const Event::MazeMovedEvent &>(e);

		m_Center.x += ne.changeX;
		m_Center.y += ne.changeY;
		if(m_NextPosActive)
		{
			m_NextPos.x += ne.changeX;
			m_NextPos.y += ne.changeY;
		}
	}

	default:
		return Mob::eventCallback(e);
	}
}