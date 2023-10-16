#include "entity/movableEntity/projectile/Projectile.h"

#include "KeyDefinitions.h"
#include "RandomGen.h"
#include "layer/level/Level.h"
#include "rendering/sprite/Sprite.h"

#include "spawner/particle/ParticleSpawner.h"

#include "event/game/MobDied.h"

#define defaultBox           \
	{                        \
		{45, 45}, { 60, 55 } \
	}

Projectile::Projectile(float startX, float startY, float damage, Direction dir, Entity *spawner, Level *level, Type type)
	: MovableEntity(startX, startY, TILE_SIZE / 2, 7.0f, dir, defaultBox, level, Sprite::ID::errorID), m_StartPos({startX, startY}), m_MaxDistance(7 * TILE_SIZE), m_Damage(damage), m_Rotation(directionToRotation(dir)), m_RotationSpeed(0.0f), spawner(spawner), hasCollided(false)
{
	float    minSpeed;
	float    maxSpeed;
	uint16_t size;

	// Stores the variables for creating a spawner
	uint16_t  spawnerLifetime;
	uint16_t  spawnRate;
	uint16_t  minLife;
	uint16_t  maxLife;
	float     minSize;
	float     maxSize;
	uint16_t  groupSize;
	glm::vec4 colour;

	// Sets the default variables for all the types of projectiles
	switch(type)
	{
	case Type::Arrow:
		m_SpriteID          = Sprite::ID::projectileArrow;
		m_Speed             = 15.0f;
		m_MaxDistance       = 10 * TILE_SIZE;
		m_CollisionBox      = {{45, 45}, {50, 50}};

		minSpeed        = 2.5f;
		maxSpeed        = 10.0f;
		spawnerLifetime = 5;
		spawnRate       = 2;
		minLife         = 5;
		maxLife         = 15;
		minSize         = 2.0f;
		maxSize         = 7.0f;
		groupSize       = 2;
		colour          = {0.471, 0.518f, 0.671f, 1.0f};

		break;

	case Type::Boomerang:
		m_SpriteID          = Sprite::ID::weaponBoomerang;
		m_Speed             = 12.5f;
		m_MaxDistance       = 10 * TILE_SIZE;
		m_RotationSpeed     = Random::uniformDist(3, 7) / 10.0f;
		m_MaxDistance       = 4 * TILE_SIZE;

		minSpeed        = 0.5f;
		maxSpeed        = 6.0f;
		spawnerLifetime = 6;
		spawnRate       = 1;
		minLife         = 2;
		maxLife         = 10;
		minSize         = 4.0f;
		maxSize         = 15.0f;
		groupSize       = 2;
		colour          = {1.0f, 0.749f, 0.212f, 1.0f};

		break;

	case Type::Dark:
		m_SpriteID          = Sprite::ID::projectileDark;
		m_Speed             = 10.0f;

		minSpeed        = 0.0f;
		maxSpeed        = 2.0f;
		spawnerLifetime = 5;
		spawnRate       = 1;
		minLife         = 10;
		maxLife         = 30;
		minSize         = 4.0f;
		maxSize         = 10.0f;
		groupSize       = 4;
		colour          = {0.216f, 0.216f, 0.216f, 1.0f};

		break;

	case Type::Fire:
		m_SpriteID          = Sprite::ID::projectileFire;
		m_Speed             = 12.5f;

		minSpeed        = 0.5f;
		maxSpeed        = 8.0f;
		spawnerLifetime = 5;
		spawnRate       = 1;
		minLife         = 5;
		maxLife         = 15;
		minSize         = 7.0f;
		maxSize         = 15.0f;
		groupSize       = 3;
		colour          = {0.929f, 0.541f, 0.0f, 1.0f};

		break;

	case Type::Frost:
		m_SpriteID          = Sprite::ID::projectileFrost;
		m_Speed             = 9.0f;

		minSpeed        = 0.0f;
		maxSpeed        = 4.0f;
		spawnerLifetime = 5;
		spawnRate       = 1;
		minLife         = 5;
		maxLife         = 20;
		minSize         = 5.0f;
		maxSize         = 12.0f;
		groupSize       = 4;
		colour          = {0.447f, 0.773f, 0.835f, 1.0f};

		break;

	case Type::Gold:
		m_SpriteID          = Sprite::ID::projectileGold;
		m_Speed             = 15.0f;

		minSpeed        = 4.0f;
		maxSpeed        = 10.0f;
		spawnerLifetime = 5;
		spawnRate       = 1;
		minLife         = 3;
		maxLife         = 10;
		minSize         = 3.0f;
		maxSize         = 9.0f;
		groupSize       = 4;
		colour          = {0.886f, 0.773f, 0.478f, 1.0f};

		break;

	case Type::Nature:
		m_SpriteID          = Sprite::ID::projectileNature;
		m_Speed             = 9.0f;

		minSpeed        = 0.5f;
		maxSpeed        = 3.0f;
		spawnerLifetime = 6;
		spawnRate       = 2;
		minLife         = 2;
		maxLife         = 10;
		minSize         = 10.0f;
		maxSize         = 20.0f;
		groupSize       = 2;
		colour          = {0.682f, 0.867f, 0.502f, 1.0f};

		break;

	case Type::Rock:
		m_SpriteID          = Sprite::ID::projectileRock;
		m_Speed             = 9.0f;
		width               = Random::uniformDist(2, 6) * TILE_SIZE / 20;
		height              = Random::uniformDist(2, 6) * TILE_SIZE / 20;
		m_Rotation          = Random::uniformDist(-30, 30) / 10.0f;
		m_RotationSpeed     = Random::uniformDist(-5, 5) / 20.0f;
		m_MaxDistance       = 3 * TILE_SIZE;

		minSpeed        = 0.5f;
		maxSpeed        = 6.0f;
		spawnerLifetime = 5;
		spawnRate       = 1;
		minLife         = 5;
		maxLife         = 15;
		{
			uint16_t size = width > height ? width : height;
			minSize       = size / 12;
			maxSize       = size / 3;
		}
		groupSize = 4;
		colour    = {0.243f, 0.216f, 0.361f, 1.0f};

		break;

	default:
		Log::warning("Unknown projectile!");
		m_Speed        = 7.0f;
		m_SpriteID     = Sprite::ID::errorID;
		m_CollisionBox = defaultBox;
		width          = TILE_SIZE / 2;
		height         = TILE_SIZE / 2;
		m_MaxDistance  = 7 * TILE_SIZE;

		minSpeed        = 0.0f;
		maxSpeed        = 0.0f;
		spawnerLifetime = 10;
		spawnRate       = 5;
		minLife         = 5;
		maxLife         = 15;
		minSize         = 50.0f;
		maxSize         = 100.0f;
		groupSize       = 4;
		colour          = {0.0f, 0.0f, 0.0f, 1.0f};

		break;
	}

	// Creates the collision function with the variables from the switch statement
	m_CollisionFunction = [spawnerLifetime, spawnRate, minLife, maxLife, minSize, maxSize, minSpeed, maxSpeed, groupSize, colour](float x, float y, Direction dir, Level *level) {
		switch(dir)
		{
		case Direction::north:
			y += 40;
			break;
		case Direction::south:
			y -= 40;
			break;
		case Direction::east:
			x += 30;
			break;
		case Direction::west:
			x -= 50;
			break;

		default:
			break;
		}
		level->addSpawner(new ParticleSpawner(x, y, level, spawnerLifetime, spawnRate, minLife, maxLife, revertDirection(dir), minSpeed, maxSpeed, minSize, maxSize, groupSize, colour));
	};
}

Projectile::Projectile(float startX, float startY, float size, float damage, Direction dir, Entity *spawner, Level *level)
	: MovableEntity(startX, startY, size, 7.0f, dir, defaultBox, level, Sprite::ID::projectileFire), m_StartPos({startX, startY}), m_MaxDistance(7 * TILE_SIZE), m_Damage(damage), spawner(spawner), hasCollided(false), m_Rotation(directionToRotation(dir)), m_RotationSpeed(0.0f)
{
}

Projectile::Projectile(float startX, float startY, float size, float damage, float speed, Direction dir, Entity *spawner, Level *level, CollisionBox box)
	: MovableEntity(startX, startY, size, speed, dir, box, level, Sprite::ID::projectileFire), m_StartPos({startX, startY}), m_MaxDistance(7 * TILE_SIZE), m_Damage(damage), spawner(spawner), hasCollided(false), m_Rotation(directionToRotation(dir)), m_RotationSpeed(0.0f)
{
}

Projectile::Projectile(float startX, float startY, float size, float maxDistance, float damage, float speed, Direction dir, Entity *spawner, Level *level, CollisionBox box, std::function<void(float, float, Direction, Level *)> collisionFunc)
	: MovableEntity(startX, startY, size, speed, dir, box, level, Sprite::ID::projectileFire), m_StartPos({startX, startY}), m_MaxDistance(maxDistance), m_Damage(damage), spawner(spawner), hasCollided(false), m_CollisionFunction(collisionFunc), m_Rotation(directionToRotation(dir)), m_RotationSpeed(0.0f)
{
}

void Projectile::update()
{
	if(!hasCollided)
	{
		m_Rotation += m_RotationSpeed;

		if(distBetweenVec({x, y}, m_StartPos) > m_MaxDistance)
			hasCollided = true;

		// Calculates the direction it is going in
		float xs = 0;
		float ys = 0;
		if(m_Dir == Direction::north)
			ys = m_Speed;
		else if(m_Dir == Direction::south)
			ys = -m_Speed;
		else if(m_Dir == Direction::east)
			xs = m_Speed;
		else
			xs = -m_Speed;

		// Checks if it has collided with any entities
		Entity *colE = m_Level->entityCollisionDetection(x + xs, y + ys, getEntityBox());
		// Can use simple collision detection because it is only going in one direction
		if(!isGhost && (m_Level->collisionDetection(x + xs, y + ys, m_CollisionBox) || (colE != nullptr && colE != spawner)))
		{
			Mob *mSpawner = dynamic_cast<Mob *>(spawner);

			if(colE)
			{
				// If it collded with a mob it will the spawner it hit the target as well as do damage on the mob
				Mob *mob = dynamic_cast<Mob *>(colE);
				if(mob)
					mob->dealDamage(m_Damage);

				if(mSpawner)
					mSpawner->getStats()->hasHitTarget(m_Damage);
			}
			else if(mSpawner)   // Tells the mob that fired it that it missed the target
				mSpawner->getStats()->hasMissedTarget();

			// Calls the collision function
			m_CollisionFunction(x, y, m_Dir, m_Level);
			hasCollided = true;
		}
		else
		{
			// Moves in the given direction
			isMoving = true;
			x += xs;
			y += ys;
		}
	}
}

bool Projectile::eventCallback(const Event::Event &e)
{
	switch (e.getType())
	{
	case Event::EventType::MobDied:
	{
		// Checks for mob died event
		const Event::MobDiedEvent &ne = static_cast<const Event::MobDiedEvent &>(e);
		if(ne.mob == spawner)   // Checks to see if it is the same mob as its spawner
		{
			// Will remove itself
			spawner     = nullptr;
			hasCollided = true;
		}

		return false;
	}

	default:
		return MovableEntity::eventCallback(e);
	}

}

void Projectile::render()
{
	// Renders the projectile
	uint8_t layer = 4;
	Render::sprite(x, y, m_Rotation, width, height, m_SpriteID, layer);
}

void Projectile::changeX(float changeBy)
{
	x += changeBy;
	m_StartPos.x += changeBy;
}
void Projectile::changeY(float changeBy)
{
	y += changeBy;
	m_StartPos.y += changeBy;
}
