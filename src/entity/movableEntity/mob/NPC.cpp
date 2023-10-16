#include "NPC.h"

#include <thread>
#include <vector>

#include "Application.h"
#include "KeyDefinitions.h"
#include "RandomGen.h"
#include "layer/MessageManager.h"
#include "layer/level/Level.h"

#include "Player.h"

#include "item/potion/Potion.h"
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

// Returns either 1, -1, or 0 to show what direction the NPC needs to walk in to get to a point
static float getRatioForAttacking(float pos, float ePos, float buffer)
{
	float dist = ePos - pos;
	if(std::fabs(dist) > buffer)
		return dist > 0 ? 1.0f : -1.0f;
	else
		return 0.0f;
}

// Returns the sprite ID from a given race and type
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

// Generates a random sprite ID from a given race
static Sprite::ID genSpriteID(NPC::Type type)
{
	int r = Random::uniformDist(0, 2);
	return getSpriteID(type, static_cast<NPC::Race>(r));
}

// Gets the race from a type and spriteID
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
	: m_Name("Bob"), m_Attack(AttackMove::None), m_Center({0.0f, 0.0f}), m_NextPos({0.0f, 0.0f}), m_NextPosActive(false), m_TimeSinceMoved(0), m_WaitFor(0)
{
	generateInventory(Race::Fire);
}

NPC::NPC(float x, float y, Level *level, Type type)
	: Mob(x, y, level, genSpriteID(type)), m_Name("Bob"), m_Attack(AttackMove::None), m_Center({x, y}), m_NextPos({0.0f, 0.0f}), m_NextPosActive(false), m_TimeSinceMoved(0), m_WaitFor(0)
{
	generateInventory(getRace(type, m_SpriteID));
}

NPC::NPC(float x, float y, Level *level, Type type, Race race)
	: Mob(x, y, level, getSpriteID(type, race)), m_Name("Bob"), m_Attack(AttackMove::None), m_Center({x, y}), m_NextPos({0.0f, 0.0f}), m_NextPosActive(false), m_TimeSinceMoved(0), m_WaitFor(0)
{
	generateInventory(race);
}

NPC::~NPC()
{
}

void NPC::generateInventory(Race race)
{
	// Generates the inventory (just a weapon depending on the race)
	int r = Random::uniformDist(0, 2);
	if(r == 0)
	{
		// Has a 1/3rd chance for the enemy to have a regular weapon
		int s = Random::uniformDist(0, 3);
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
		// Determines the weapon from the race
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
	if(!m_Level)
	{
		Log::critical("Level is null", LOGINFO);
		return;
	}

	// Gets the path list from the level
	Vec2f               start = {x, y};
	std::vector<Vec2f> *path  = m_Level->getPath(start, dest, getMovingCollisionBox());

	// Will return if in small range or the path size is 0
	if(path->size() == 0 || (path->size() == 1 && distBetweenVec({x, y}, path->back()) < speed))
	{
		isMoving = false;
		delete path;
		return;
	}

	// Will use all of the speed to get as far as possible
	float availableDist = speed;
	while(availableDist > 0.0f && path->size() > 0)
	{
		float distToNext = distBetweenVec({x, y}, path->back());
		Vec2f distVec    = {path->back().x - x, path->back().y - y};

		// If the distance to the next node is smaller than the available then it will pop the node from the list and continue to the next node
		if(distToNext < availableDist)
		{
			availableDist -= distToNext;
			move(distVec.x, distVec.y);
			path->pop_back();
		}
		else
		{
			// If the distance to the next node is smaller than the available distance it will use all of it to get as close to the next node as possible
			availableDist = 0.0f;
			float timesBy = speed / distToNext;
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

	// Checks how far away it should be
	float minDistAway = m_Following->getFollowDistance(this);

	// Will find a path to its follower if the distance is greater than the minDist
	if(distBetweenVec<Vec2f>({m_Following->getX(), m_Following->getY()}, {x, y}) > minDistAway)
		findPath({m_Following->getX(), m_Following->getY()}, m_Speed);
	else
		isMoving = false;
}

void NPC::roam()
{
	// Generates the next position it will go to
	if(!m_NextPosActive)
		generateNextPos();

	float minDistAway = TILE_SIZE / 3;

	// If it is further away than the minimum distance it will get the shortest path to that
	if(distBetweenVec(m_NextPos, {x, y}) > minDistAway)
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
	// Resets variables
	m_TimeSinceMoved = 0;
	m_WaitFor        = Random::uniformDist(120, 500);
	m_NextPosActive  = true;

	// Generates random position in the room
	float range       = TILE_SIZE * 2;
	float xPercentage = Random::uniformDist(-100, 100) / 100.0f;
	float yPercentage = Random::uniformDist(-100, 100) / 100.0f;

	m_NextPos = {m_Center.x + range * xPercentage, m_Center.y + range * yPercentage};
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

	// If the NPC does not have a weapon or is too close to the enemy it will run away
	if(m_CurrentWeapon == -1 || distBetweenVec<Vec2f>({x, y}, {m_Enemy->getX(), m_Enemy->getY()}) < TILE_SIZE && m_Attack != AttackMove::GoToPoint)
		m_Attack = AttackMove::RunAway;
	else if(m_Stats.getHealth() > 0.8 * m_Stats.getMaxHealth())   // If its health is at 80% it will continue to attack
		m_Attack = AttackMove::Attack;
	else if(m_TimeSinceMoved > m_WaitFor)
	{
		// If it has been using the same attack move for a while it will change up its tactic
		// If the stamina and health is very low it will run away
		if(m_Stats.getStamina() < 0.2 * m_Stats.getMaxStamina() && m_Stats.getHealth() < 0.2 * m_Stats.getMaxHealth())
		{
			m_Attack  = AttackMove::RunAway;
			m_WaitFor = Random::uniformDist(150, 500);
		}
		else if(m_Stats.getStamina() < 0.2 * m_Stats.getMaxStamina())
		{
			// If the stamina is very low it will start to try and dodge oncoming attacks
			m_Attack = AttackMove::Dodge;

			m_WaitFor = Random::uniformDist(200, 500);
		}
		else if(Random::uniformDist(0, 3) == 0)
		{
			// Will randomise if it will go to a different point in the room to change up its attack
			goToPointInRoom();
		}
		else
		{
			// Otherwise it will continue to attack the player
			m_Attack  = AttackMove::Attack;
			m_WaitFor = Random::uniformDist(200, 500);
		}

		// sets it to -10 to have a slight delay between changing strategies
		m_TimeSinceMoved = -20;
	}

	isMoving = false;

	// The NPC attacking
	switch(m_Attack)
	{
	case AttackMove::Dodge:
	{
		// Gets the closes projectile in range and will move in a direction to avoid it
		float       range = TILE_SIZE * 2;
		Projectile *proj  = m_Level->getProjInRange(x, y, range);
		if(proj)
		{
			Direction dir = proj->getDirection();
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
		// If it is running away it will go in the opposite direction to the enemy
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

		if(m_TimeSinceMoved > 10 && m_TimeSinceMoved % 30 == 0)
		{
			// If the time since moved has been long enough it will look for any potions in its inventory and use them
			for(Item *item : m_Inventory)
			{
				Potion *potion = dynamic_cast<Potion *>(item);

				if(potion)
				{
					potion->useOn(this);
					m_TimeSinceMoved += 30;

					break;
				}
			}
		}

		break;
	}

	case AttackMove::RunAwayAlongX:
	{
		// If it is running away along the x axis it will move in that direction
		float difX = x - m_Enemy->getX();
		if(canMove({difX, 0.0f}))
			move({difX, 0.0f});
		else
			goToPointInRoom();

		if(m_TimeSinceMoved > 10 && m_TimeSinceMoved % 30 == 0)
		{
			// If the time since moved has been long enough it will look for any potions in its inventory and use them
			for(Item *item : m_Inventory)
			{
				Potion *potion = dynamic_cast<Potion *>(item);

				if(potion)
				{
					potion->useOn(this);   // TODO: Make it so that this is deleted and in a function
					m_TimeSinceMoved += 30;

					break;
				}
			}
		}

		break;
	}

	case AttackMove::RunAwayAlongY:
	{
		// Runs away from the player on the Y axis only
		float difY = y - m_Enemy->getY();
		if(canMove({0.0f, difY}))
			move({0.0f, difY});
		else
			goToPointInRoom();

		if(m_TimeSinceMoved > 10 && m_TimeSinceMoved % 30 == 0)
		{
			// If the time since moved has been long enough it will look for any potions in its inventory and use them
			for(Item *item : m_Inventory)
			{
				Potion *potion = dynamic_cast<Potion *>(item);

				if(potion)
				{
					potion->useOn(this);   // TODO: Same thing as above, have it run a function here
					m_TimeSinceMoved += 30;

					break;
				}
			}
		}

		break;
	}

	case AttackMove::GoToPoint:
	{
		// Finds the shortest path to the point
		float xDif        = m_NextPos.x - x;
		float yDif        = m_NextPos.y - y;
		float minDistAway = TILE_SIZE / 3;

		if(distBetweenVec(m_NextPos, {x, y}) > minDistAway)
			findPath({m_NextPos.x, m_NextPos.y}, m_Speed * (m_Stats.getStamina() / m_Stats.getMaxStamina()) / 2);
		else
			m_Attack = AttackMove::None;

		break;
	}

	default:   // Attack
	{
		// Buffer stores how close it can be to the actual coordinates
		float buffer = TILE_SIZE / 4.0f;
		// Stores the direction it has to go in
		Vec2f ratio = {getRatioForAttacking(x, m_Enemy->getX(), buffer), getRatioForAttacking(y, m_Enemy->getY(), buffer)};

		if((ratio.y == 0.0f || ratio.x == 0.0f))   // Means it will be able to attack
		{
			if(m_Weapons[m_CurrentWeapon]->canUse())   // Checks if it can use its weapon
			{
				// Changes the direction so that it is facing the enemy
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

				// Checks if the person the NPC is following is in the way (if it is it will cancel the attack and go to another point in the room)
				// NOTE: Does not look at who is following it because it adds an interesting dynamic to the came, as tho the masters, if you will, do not care about their followers
				bool cancel = false;
				if(m_Following)
				{
					switch(m_Dir)
					{
					case Direction::north:
						cancel = m_Following->getY() > y && fabs(x - m_Following->getX()) > TILE_SIZE / 2.0f;
						break;
					case Direction::south:
						cancel = m_Following->getY() < y && fabs(x - m_Following->getX()) > TILE_SIZE / 2.0f;
						break;
					case Direction::east:
						cancel = m_Following->getX() > x && fabs(y - m_Following->getY()) > TILE_SIZE / 2.0f;
						break;
					case Direction::west:
						cancel = m_Following->getX() < x && fabs(y - m_Following->getY()) > TILE_SIZE / 2.0f;
						break;

					default:
						break;
					}
				}

				if(cancel)
					goToPointInRoom();
				else
					useCurrentWeapon(true);   // Attacks using its weapon
			}
		}
		else
		{
			// Walks in the direction to get to a position where it can attack
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
				if(distY < TILE_SIZE)   // This checks that the distance is within a given range and corrects accordingly
				{
					m_TimeSinceMoved = 0;
					m_WaitFor        = 60;
					m_Attack         = AttackMove::RunAwayAlongX;
				}
				else if(distY < 2 * TILE_SIZE)
					ratio.y = 0.0f;
			}

			// Moves if it is attacking
			if(m_Attack == AttackMove::Attack)
				move(ratio);
		}

		break;
	}
	}
}

void NPC::update()
{
	// If in the same room as the enemy it will attack
	if(m_Enemy && (int) m_Enemy->getX() / (TILE_SIZE * ROOM_SIZE) == (int) x / (TILE_SIZE * ROOM_SIZE) && (int) m_Enemy->getY() / (TILE_SIZE * ROOM_SIZE) == (int) y / (TILE_SIZE * ROOM_SIZE))
		attack();
	else
	{
		// Otherwise resets the attack move
		if(m_Attack != AttackMove::None)
			m_Attack = AttackMove::None;

		// If it is following a mob it will follow the mob
		if(m_Following)
			follow();
		else
			roam();   // Otherwise it will rome the current room it is in
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
	// Sets the attack move to go to a point
	m_Attack         = AttackMove::GoToPoint;
	m_TimeSinceMoved = 0;   //Rests time since moved

	// Sets the center of the room
	m_Center = {(float) ((int) x / ROOM_PIXEL_SIZE) * ROOM_PIXEL_SIZE + ROOM_PIXEL_SIZE / 2, (float) ((int) y / ROOM_PIXEL_SIZE) * ROOM_PIXEL_SIZE + ROOM_PIXEL_SIZE / 2};

	// Gets a random position in the room
	m_NextPosActive   = true;
	float range       = TILE_SIZE * 1.5f;
	float xPercentage = Random::uniformDist(-100, 100) / 100.0f;
	float yPercentage = Random::uniformDist(-100, 100) / 100.0f;
	m_NextPos         = {m_Center.x + range * xPercentage, m_Center.y + range * yPercentage};
}

void NPC::setFollowing(Mob *following)
{
	// Updates the center and start following the given mob
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

		// Checks teh player is in range and the mouse is inside the visual hit box
		Player *const player = m_Level->getPlayer();
		if(doesPointIntersectWithBox(convPos, {x, y}, {{-width / 2, -height / 2}, {width / 2, height / 2}}) && distBetweenVec<Vec2f>({player->getX(), player->getY() - player->getWidth() / 2}, {x, y}) < 5.0f * TILE_SIZE)
		{
			// If it is a random NPC it will open a NPC interaction menu to allow the player to get them to follow them
			if(!m_Enemy && !m_Following)
			{
				Event::ChangeGUIMenuEvent e(Event::ChangeGUIMenuEvent::Menu::NPCInteraction);
				Application::callEvent(e, Event::CallType::Overlay);

				MessageManager::sendMessage("NPC: Can I follow you?", MessageManager::Priority::Low);

				return true;
			}
			else if(m_Following == player)
			{
				// If the NPC is following the player it opens an NPCInventory which allows the player to stores potions and different items in their inventory
				Event::ChangeGUIMenuEvent e1(Event::ChangeGUIMenuEvent::Menu::NPCInventory);
				Application::callEvent(e1, Event::CallType::Overlay);

				Event::ChestOpenedEvent e2(&m_Inventory, nullptr);
				Application::callEvent(e2, Event::CallType::Overlay);

				Event::ChestOpenedEvent e3(&m_Weapons, &m_CurrentWeapon);
				Application::callEvent(e3, Event::CallType::Overlay);
				return true;
			}

			return false;
		}

		return false;
	}

	case Event::EventType::PlayerResponse:
	{
		if(m_Following || m_Enemy)   // Ignores if it is attacking or following another mob
			return false;

		const Event::PlayerResponseEvent &ne = static_cast<const Event::PlayerResponseEvent &>(e);
		if(ne.response == Event::PlayerResponseEvent::Response::Reject)
		{
			// If the player has rejected will randomise whether it will attack the player
			int r = Random::uniformDist(0, 4);

			if(r == 0)
			{
				// Sets the enemy
				m_Enemy = m_Level->getPlayer();

				// Locks the room down
				Event::ShowAltTileEvent e(true);
				Application::callEvent(e);

				MessageManager::sendMessage("NPC: How rude!? I will kill you now!", MessageManager::Priority::Medium);
			}
			else
				MessageManager::sendMessage("NPC: Oh okay, fine be like that.", MessageManager::Priority::Low);
		}
		else
		{
			// If the player has accepted it will start following the player if they can
			if(m_Level->getPlayer()->addFollower(this))
			{
				MessageManager::sendMessage("NPC: Yay! I know we will be best buds!", MessageManager::Priority::Low);
				m_Following = m_Level->getPlayer();
			}
			else   // If they cant follow the player it will display a message accordingly
				MessageManager::sendMessage("NPC: Looks like you already have a follower", MessageManager::Priority::Medium);
		}

		// Changes the menu back to the overlay
		Event::ChangeGUIMenuEvent e(Event::ChangeGUIMenuEvent::Menu::Overlay);
		Application::callEvent(e, Event::CallType::Overlay);

		return true;
	}

	default:
		return Mob::eventCallback(e);
	}
}

// Updates the center and next postion accordingly
void NPC::changeX(float changeBy)
{
	m_Center.x += changeBy;
	if(m_NextPosActive)
		m_NextPos.x += changeBy;

	Entity::changeX(changeBy);
}

void NPC::changeY(float changeBy)
{
	m_Center.y += changeBy;
	if(m_NextPosActive)
		m_NextPos.y += changeBy;

	Entity::changeY(changeBy);
}
