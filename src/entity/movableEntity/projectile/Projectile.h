#include "MovableEntity.h"

class Projectile : public MovableEntity
{
  protected:
	Vec2f   m_StartPos;
	float   m_MaxDistance;
	float   m_Damage;
	Entity *spawner;
	bool    hasCollided;

  public:
	Projectile(float startX, float startY, float maxDistance, float damage, float speed, Direction dir, float width, float height, Entity *spawner, Level *level);
	virtual ~Projectile() {}
}