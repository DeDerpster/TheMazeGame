#pragma once

#include <vector>

#include "effects/Effect.h"

class Stats
{
  private:
	// Health and stamina
	float m_Health, m_MaxHealth;
	float m_Stamina, m_MaxStamina;

	// Natural regen infomation
	uint16_t m_RegenDelay, m_MaxRegenDelay;

	// Strength and agility stats
	int m_Strength, m_Agility;

	// Other stats
	int      m_CombatAbility;
	int      m_Confidence;
	uint16_t m_Boredom;
	uint16_t m_Attractiveness;   // Effects how many follows a mob can have

	uint16_t m_MaxActiveWeapons;
	uint16_t m_MaxInventory;

	// Stores information for leveling up
	uint16_t m_ExpLevel;
	uint16_t m_Experience;

	std::vector<StatsEffect> m_Effects;   // Stores all the current effects

  public:
	Stats(bool randomise = true);
	virtual ~Stats();

	virtual bool update();

	void  dealDamage(float damage);
	void  changeHealth(float changeBy);
	void  changeMaxHealth(float changeBy);
	void  changeStamina(float changeBy);
	void  changeConfidence(float changeBy);
	void  changeBoredom(float changeBy);
	float getDamage(float minDamage, float maxDamage);
	void  hasHitTarget(float damageDealt);
	void  hasKilledTarget();
	void  hasMissedTarget();
	void  hasFoundChest();
	void  hasUsedWeapon();
	void  resetStats();

	void setAttractiveness(uint16_t attractiveness);

	int getWeaponDelay(int delay);

	bool     isDead();
	float    getHealth();
	float    getMaxHealth();
	float    getStamina();
	float    getMaxStamina();
	float    getMaxActiveWeapons();
	float    getMaxInventory();
	uint16_t getAttractiveness();

	const float *getHealthPointer();
	const float *getMaxHealthPointer();
	const float *getStaminaPointer();
	const float *getMaxStaminaPointer();

	void addEffect(StatsEffect &effect);
};