#pragma once

class StatsMob
{
  private:
	uint16_t m_Stat_Health, m_Stat_MaxHealth;
	uint16_t m_Stat_Stamina, m_Stat_MaxStamina;

	uint16_t m_Stat_RegenDelay, m_Stat_MaxRegenDelay;

	int m_Stat_Strength, m_Stat_Agility;

	int      m_Stat_CombatAbility;
	int      m_Stat_Confidence;
	uint16_t m_Stat_Boredom;
	uint16_t m_Stat_Attractiveness;

  public:
	StatsMob();
	virtual ~StatsMob();

	virtual void update();

	void  dealDamage(int damage);
	void  changeHealth(int changeBy);
	void  changeStamina(int changeBy);
	void  changeConfidence(int changeBy);
	void  changeBoredom(int changeBy);
	float getDamage(float minDamage, float maxDamage);
	void  hasHitTarget(int damageDealt);
	void  hasMissedTarget();
	void  hasUsedWeapon();
	void  resetStats();

	int getWeaponDelay(int delay);

	bool     isDead();
	uint16_t getHeath();
	uint16_t getMaxHeath();
	uint16_t getStamina();
	uint16_t getMaxStamina();
	uint16_t getAttractiveness();
};