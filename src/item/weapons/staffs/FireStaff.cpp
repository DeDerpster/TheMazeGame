#include "FireStaff.h"

#include "KeyDefinitions.h"

FireStaff::FireStaff()
	: Weapon("Fire Staff", 10.0f, 20, ITEM_STICK) {}
FireStaff::~FireStaff() {}

void FireStaff::attack(Level *level, Mob &e, Direction dir, bool hold)
{
	if(m_Cooldown == 0)
	{
		float        damage        = e.getDamage(0.0f, m_Damage);
		CollisionBox box           = {{25, 20},
                            {60, 20}};
		float        speed         = 12.5f;
		auto         collisionFunc = [](float x, float y, Direction dir, Level *level) {
            float xMinSpeed = -8.0f;
            float xMaxSpeed = 8.0f;
            float yMinSpeed = -8.0f;
            float yMaxSpeed = 8.0f;
            if(dir == Direction::north)
            {
                yMaxSpeed = 0.5f;
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

            Effect::ParticleSpawnerObject spawner(new ParticleSpawner(x, y, level, spawnerLifetime, spawnRate, minLife, maxLife, xMinSpeed, xMaxSpeed, yMinSpeed, yMaxSpeed, minSize, maxSize, groupSize, colour));
            Application::setEffect(&spawner);
		};
		float maxDistance = 700.0f;
		level->addProjectile(new Projectile(e.getX(), e.getY(), TILE_SIZE / 2, maxDistance, damage, speed, dir, &e, level, box, collisionFunc));
		m_Cooldown = e.getWeaponDelay(m_CooldownMax);
		e.hasUsedWeapon();
	}
}