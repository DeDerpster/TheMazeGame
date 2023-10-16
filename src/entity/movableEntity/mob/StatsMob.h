#pragma once

class StatsMob
{
  private:
	float m_Stat_Health, m_Stat_MaxHealth;
	float m_Stat_Stamina, m_Stat_MaxStamina;

	uint16_t m_Stat_RegenDelay, m_Stat_MaxRegenDelay;

	int m_Stat_Strength, m_Stat_Agility;

	int      m_Stat_CombatAbility;
	int      m_Stat_Confidence;
	uint16_t m_Stat_Boredom;
	uint16_t m_Stat_Attractiveness;
	uint16_t m_Stat_MaxActiveWeapons;
	uint16_t m_Stat_MaxInventory;

  public:
	StatsMob();
	virtual ~StatsMob();

	virtual void update();

	void  dealDamage(float damage);
	void  changeHealth(float changeBy);
	void  changeMaxHealth(float changeBy);
	void  changeStamina(float changeBy);
	void  changeConfidence(float changeBy);
	void  changeBoredom(float changeBy);
	float getDamage(float minDamage, float maxDamage);
	void  hasHitTarget(float damageDealt);
	void  hasMissedTarget();
	void  hasUsedWeapon();
	void  resetStats();

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
};