#include "Projectile.h"

#include "Entity.h"
#include "KeyDefinitions.h"
#include "Level.h"
#include "ParticleSpawner.h"
#include "Sprite.h"
#include "RandomGen.h"

#define defaultBox           \
	{                        \
		{45, 45}, { 60, 55 } \
	}

Projectile::Projectile(float startX, float startY, float damage, Direction dir, Entity *spawner, Level *level, Type type)
	: MovableEntity(startX, startY, TILE_SIZE / 2, 7.0f, dir, defaultBox, level, Sprite::ID::errorID), m_StartPos({startX, startY}), m_MaxDistance(7 * TILE_SIZE), m_Damage(damage), spawner(spawner), hasCollided(false), m_Rotation(directionToRotation(dir)), m_RotationSpeed(0.0f)
{
	switch(type)
	{
	case Type::Arrow:
		m_SpriteID          = Sprite::ID::projectileArrow;
		m_Speed             = 15.0f;
		m_MaxDistance       = 10 * TILE_SIZE;
		m_CollisionBox      = {{45, 45}, {50, 50}};
		m_CollisionFunction = [](float x, float y, Direction dir, Level *level) {
			float xMinSpeed = -10.0f;
			float xMaxSpeed = 10.0f;
			float yMinSpeed = -10.0f;
			float yMaxSpeed = 10.0f;
			if(dir == Direction::north)
			{
				yMaxSpeed = 2.5f;
				y += 40;
			}
			else if(dir == Direction::south)
			{
				yMinSpeed = -2.5f;
				y -= 40;
			}
			else if(dir == Direction::east)
			{
				xMaxSpeed = 2.5f;
				x += 30;
			}
			else
			{
				xMinSpeed = -2.5f;
				x -= 50;
			}

			uint16_t  spawnerLifetime = 5;
			uint16_t  spawnRate       = 2;
			uint16_t  minLife         = 5;
			uint16_t  maxLife         = 15;
			float     minSize         = 2.0f;
			float     maxSize         = 7.0f;
			uint16_t  groupSize       = 2;
			glm::vec4 colour          = {0.471, 0.518f, 0.671f, 1.0f};

			level->addSpawner(new ParticleSpawner(x, y, level, spawnerLifetime, spawnRate, minLife, maxLife, xMinSpeed, xMaxSpeed, yMinSpeed, yMaxSpeed, minSize, maxSize, groupSize, colour));
		};

		break;
	case Type::Boomerang:
		m_SpriteID          = Sprite::ID::weaponBoomerang;
		m_Speed             = 12.5f;
		m_MaxDistance       = 10 * TILE_SIZE;
		m_RotationSpeed     = Random::getNum(3, 7) / 10.0f;
		m_MaxDistance       = 4 * TILE_SIZE;
		m_CollisionFunction = [](float x, float y, Direction dir, Level *level) {
			float xMinSpeed = -6.0f;
			float xMaxSpeed = 6.0f;
			float yMinSpeed = -6.0f;
			float yMaxSpeed = 6.0f;
			if(dir == Direction::north)
			{
				yMaxSpeed = 0.5f;
				y += 40;
			}
			else if(dir == Direction::south)
			{
				yMinSpeed = -0.5f;
				y -= 40;
			}
			else if(dir == Direction::east)
			{
				xMaxSpeed = 0.5f;
				x += 30;
			}
			else
			{
				xMinSpeed = -0.5f;
				x -= 50;
			}

			uint16_t  spawnerLifetime = 6;
			uint16_t  spawnRate       = 1;
			uint16_t  minLife         = 2;
			uint16_t  maxLife         = 10;
			float     minSize         = 4.0f;
			float     maxSize         = 15.0f;
			uint16_t  groupSize       = 2;
			glm::vec4 colour          = {1.0f, 0.749f, 0.212f, 1.0f};

			level->addSpawner(new ParticleSpawner(x, y, level, spawnerLifetime, spawnRate, minLife, maxLife, xMinSpeed, xMaxSpeed, yMinSpeed, yMaxSpeed, minSize, maxSize, groupSize, colour));
		};

		break;
	case Type::Dark:
		m_SpriteID          = Sprite::ID::projectileDark;
		m_Speed             = 10.0f;
		m_CollisionFunction = [](float x, float y, Direction dir, Level *level) {
			float xMinSpeed = -2.0f;
			float xMaxSpeed = 2.0f;
			float yMinSpeed = -2.0f;
			float yMaxSpeed = 2.0f;
			if(dir == Direction::north)
			{
				yMaxSpeed = 0.0f;
				y += 40;
			}
			else if(dir == Direction::south)
			{
				yMinSpeed = 0.0f;
				y -= 40;
			}
			else if(dir == Direction::east)
			{
				xMaxSpeed = 0.0f;
				x += 30;
			}
			else
			{
				xMinSpeed = 0.0f;
				x -= 50;
			}

			uint16_t  spawnerLifetime = 5;
			uint16_t  spawnRate       = 1;
			uint16_t  minLife         = 10;
			uint16_t  maxLife         = 30;
			float     minSize         = 4.0f;
			float     maxSize         = 10.0f;
			uint16_t  groupSize       = 4;
			glm::vec4 colour          = {0.216f, 0.216f, 0.216f, 1.0f};

			level->addSpawner(new ParticleSpawner(x, y, level, spawnerLifetime, spawnRate, minLife, maxLife, xMinSpeed, xMaxSpeed, yMinSpeed, yMaxSpeed, minSize, maxSize, groupSize, colour));
		};

		break;
	case Type::Fire:
		m_SpriteID          = Sprite::ID::projectileFire;
		m_Speed             = 12.5f;
		m_CollisionFunction = [](float x, float y, Direction dir, Level *level) {
			float xMinSpeed = -8.0f;
			float xMaxSpeed = 8.0f;
			float yMinSpeed = -8.0f;
			float yMaxSpeed = 8.0f;
			if(dir == Direction::north)
			{
				yMaxSpeed = 0.5f;
				y += 40;
			}
			else if(dir == Direction::south)
			{
				yMinSpeed = -0.5f;
				y -= 40;
			}
			else if(dir == Direction::east)
			{
				xMaxSpeed = 0.5f;
				x += 30;
			}
			else
			{
				xMinSpeed = -0.5f;
				x -= 50;
			}

			uint16_t  spawnerLifetime = 5;
			uint16_t  spawnRate       = 1;
			uint16_t  minLife         = 5;
			uint16_t  maxLife         = 15;
			float     minSize         = 7.0f;
			float     maxSize         = 15.0f;
			uint16_t  groupSize       = 3;
			glm::vec4 colour          = {0.929f, 0.541f, 0.0f, 1.0f};

			level->addSpawner(new ParticleSpawner(x, y, level, spawnerLifetime, spawnRate, minLife, maxLife, xMinSpeed, xMaxSpeed, yMinSpeed, yMaxSpeed, minSize, maxSize, groupSize, colour));
		};

		break;
	case Type::Frost:
		m_SpriteID          = Sprite::ID::projectileFrost;
		m_Speed             = 9.0f;
		m_CollisionFunction = [](float x, float y, Direction dir, Level *level) {
			float xMinSpeed = -4.0f;
			float xMaxSpeed = 4.0f;
			float yMinSpeed = -4.0f;
			float yMaxSpeed = 4.0f;
			if(dir == Direction::north)
			{
				yMaxSpeed = 0.0f;
				y += 40;
			}
			else if(dir == Direction::south)
			{
				yMinSpeed = 0.0f;
				y -= 40;
			}
			else if(dir == Direction::east)
			{
				xMaxSpeed = 0.0f;
				x += 30;
			}
			else
			{
				xMinSpeed = 0.0f;
				x -= 50;
			}

			uint16_t  spawnerLifetime = 5;
			uint16_t  spawnRate       = 1;
			uint16_t  minLife         = 5;
			uint16_t  maxLife         = 20;
			float     minSize         = 5.0f;
			float     maxSize         = 12.0f;
			uint16_t  groupSize       = 4;
			glm::vec4 colour          = {0.447f, 0.773f, 0.835f, 1.0f};

			level->addSpawner(new ParticleSpawner(x, y, level, spawnerLifetime, spawnRate, minLife, maxLife, xMinSpeed, xMaxSpeed, yMinSpeed, yMaxSpeed, minSize, maxSize, groupSize, colour));
		};

		break;
	case Type::Gold:
		m_SpriteID          = Sprite::ID::projectileGold;
		m_Speed             = 15.0f;
		m_CollisionFunction = [](float x, float y, Direction dir, Level *level) {
			float xMinSpeed = -10.0f;
			float xMaxSpeed = 10.0f;
			float yMinSpeed = -10.0f;
			float yMaxSpeed = 10.0f;
			if(dir == Direction::north)
			{
				yMaxSpeed = 4.0f;
				y += 40;
			}
			else if(dir == Direction::south)
			{
				yMinSpeed = -4.0f;
				y -= 40;
			}
			else if(dir == Direction::east)
			{
				xMaxSpeed = 4.0f;
				x += 30;
			}
			else
			{
				xMinSpeed = -4.0f;
				x -= 50;
			}

			uint16_t  spawnerLifetime = 5;
			uint16_t  spawnRate       = 1;
			uint16_t  minLife         = 3;
			uint16_t  maxLife         = 10;
			float     minSize         = 3.0f;
			float     maxSize         = 9.0f;
			uint16_t  groupSize       = 4;
			glm::vec4 colour          = {0.886f, 0.773f, 0.478f, 1.0f};

			level->addSpawner(new ParticleSpawner(x, y, level, spawnerLifetime, spawnRate, minLife, maxLife, xMinSpeed, xMaxSpeed, yMinSpeed, yMaxSpeed, minSize, maxSize, groupSize, colour));
		};

		break;
	case Type::Nature:
		m_SpriteID          = Sprite::ID::projectileNature;
		m_Speed             = 9.0f;
		m_CollisionFunction = [](float x, float y, Direction dir, Level *level) {
			float xMinSpeed = -3.0f;
			float xMaxSpeed = 3.0f;
			float yMinSpeed = -3.0f;
			float yMaxSpeed = 3.0f;
			if(dir == Direction::north)
			{
				yMaxSpeed = 0.5f;
				y += 40;
			}
			else if(dir == Direction::south)
			{
				yMinSpeed = -0.5f;
				y -= 40;
			}
			else if(dir == Direction::east)
			{
				xMaxSpeed = 0.5f;
				x += 30;
			}
			else
			{
				xMinSpeed = -0.5f;
				x -= 50;
			}

			uint16_t  spawnerLifetime = 6;
			uint16_t  spawnRate       = 2;
			uint16_t  minLife         = 2;
			uint16_t  maxLife         = 10;
			float     minSize         = 10.0f;
			float     maxSize         = 20.0f;
			uint16_t  groupSize       = 2;
			glm::vec4 colour          = {0.682f, 0.867f, 0.502f, 1.0f};

			level->addSpawner(new ParticleSpawner(x, y, level, spawnerLifetime, spawnRate, minLife, maxLife, xMinSpeed, xMaxSpeed, yMinSpeed, yMaxSpeed, minSize, maxSize, groupSize, colour));
		};

		break;
	case Type::Rock:
		m_SpriteID          = Sprite::ID::projectileRock;
		m_Speed             = 9.0f;
		width               = Random::getNum(2, 6) * TILE_SIZE / 20;
		height              = Random::getNum(2, 6) * TILE_SIZE / 20;
		m_Rotation          = Random::getNum(-30, 30) / 10.0f;
		m_RotationSpeed     = Random::getNum(-5, 5) / 20.0f;
		m_MaxDistance       = 3 * TILE_SIZE;
		m_CollisionFunction = [this](float x, float y, Direction dir, Level *level) {
			float xMinSpeed = -6.0f;
			float xMaxSpeed = 6.0f;
			float yMinSpeed = -6.0f;
			float yMaxSpeed = 6.0f;
			if(dir == Direction::north)
			{
				yMaxSpeed = 0.5f;
				y += 40;
			}
			else if(dir == Direction::south)
			{
				yMinSpeed = -0.5f;
				y -= 40;
			}
			else if(dir == Direction::east)
			{
				xMaxSpeed = 0.5f;
				x += 30;
			}
			else
			{
				xMinSpeed = -0.5f;
				x -= 50;
			}

			uint16_t  size            = width > height ? width : height;

			uint16_t  spawnerLifetime = 5;
			uint16_t  spawnRate       = 1;
			uint16_t  minLife         = 5;
			uint16_t  maxLife         = 15;
			float     minSize         = size / 12;
			float     maxSize         = size / 3;
			uint16_t  groupSize       = 4;
			glm::vec4 colour          = {0.243f, 0.216f, 0.361f, 1.0f};

			level->addSpawner(new ParticleSpawner(x, y, level, spawnerLifetime, spawnRate, minLife, maxLife, xMinSpeed, xMaxSpeed, yMinSpeed, yMaxSpeed, minSize, maxSize, groupSize, colour));
		};

		break;

	default:
		Log::warning("Unknown projectile!");
		m_Speed        = 7.0f;
		m_SpriteID     = Sprite::ID::errorID;
		m_CollisionBox = defaultBox;
		width          = TILE_SIZE / 2;
		height         = TILE_SIZE / 2;
		m_MaxDistance  = 7 * TILE_SIZE;

		break;
	}
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

		if(distBetweenVec2f({x, y}, m_StartPos) > m_MaxDistance)
			hasCollided = true;
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
		Entity *colE = m_Level->entityCollisionDetection(x + xs, y + ys, getEntityBox());
		if(!isGhost && (m_Level->collisionDetection(x + xs, y + ys, m_CollisionBox) || (colE != nullptr && colE != spawner)))
		{
			Mob *mSpawner = dynamic_cast<Mob *>(spawner);
			if(colE)
			{
				Mob *mob = dynamic_cast<Mob *>(colE);
				if(mob)
					mob->dealDamage(m_Damage);
				if(mSpawner)
					mSpawner->hasHitTarget(m_Damage);
			}
			else if(mSpawner)
				mSpawner->hasMissedTarget();
			m_CollisionFunction(x, y, m_Dir, m_Level);
			hasCollided = true;
		}
		else
		{
			isMoving = true;
			x += xs;
			y += ys;
		}
	}
}

bool Projectile::eventCallback(const Event::Event &e)
{
	if(e.getType() == Event::EventType::mobDied)
	{
		const Event::MobDied &ne = static_cast<const Event::MobDied &>(e);
		if(ne.mob == spawner)
		{
			spawner     = nullptr;
			hasCollided = true;
		}
	}
	return MovableEntity::eventCallback(e);
}

void Projectile::render()
{
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
