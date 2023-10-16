#include "Stats.h"

#include "2dVec.h"
#include "Log.h"
#include "RandomGen.h"

#include <math.h>

#define MAXLEVEL 15

Stats::Stats(bool randomise)
	: m_Health(100),
	  m_MaxHealth(100),
	  m_Stamina(100),
	  m_MaxStamina(100),
	  m_RegenDelay(0),
	  m_MaxRegenDelay(60),
	  m_Strength(5),
	  m_Agility(5),
	  m_CombatAbility(5),
	  m_Boredom(0),
	  m_Confidence(100),
	  m_Attractiveness(1),
	  m_MaxActiveWeapons(3),
	  m_MaxInventory(20),   // NOTE: This should not be changed
	  m_ExpLevel(1),
	  m_Experience(0)
{
	if(randomise)
	{
		m_MaxHealth = Random::uniformDist(50, 125);
		m_Health    = m_MaxHealth;

		m_MaxStamina = Random::uniformDist(75, 150);
		m_Stamina    = m_MaxStamina;

		m_Strength      = Random::binomialDist(10, 0.3);
		m_Agility       = Random::binomialDist(10, 0.2);
		m_CombatAbility = Random::binomialDist(10, 0.2);
	}
}

Stats::~Stats()
{
}

bool Stats::update()
{
	for(auto it = m_Effects.begin(); it != m_Effects.end();)
	{
		it->update();
		if(it->deleteMe())
			it = m_Effects.erase(it);
		else
			++it;
	}

	if(m_RegenDelay > 0)
		m_RegenDelay--;
	else
	{
		changeHealth(1);
		changeStamina(5);
		m_RegenDelay = 10;
	}

	if(m_ExpLevel >= 20)
		m_Experience = 0;
	else if(m_Experience > 100 * m_ExpLevel)
	{
		m_MaxHealth += 25;
		m_MaxStamina += 50;

		if(m_Agility < 20)
			m_Agility += 1;
		if(m_Strength < 20)
			m_Strength += 1;
		if(m_Strength < 20)
			m_CombatAbility += 1;

		if(m_ExpLevel % 3 == 0 && m_Attractiveness < 3)
			m_Attractiveness++;

		m_Experience -= 100 * m_ExpLevel;
		m_ExpLevel += 1;

		return true;
	}

	return false;
}

void Stats::dealDamage(float damage)
{
	int r = Random::uniformDist(0, 100);
	if(r > m_Strength + m_Agility + m_CombatAbility)
		m_Experience++;
	else if(r > (m_Strength + m_Agility + m_CombatAbility) / 2)
	{
		damage = damage / 2.0f;

		m_Experience += 5;
	}
	else
	{
		damage = 0.0f;

		m_Experience += 20;
	}

	if(m_Health < damage)
		m_Health = 0;
	else
		m_Health -= damage;

	m_RegenDelay = 100;
}

void Stats::changeHealth(float changeBy)
{
	if(changeBy < 0 && m_Health < -changeBy)
		m_Health = 0;
	else if(m_Health + changeBy > m_MaxHealth)
		m_Health = m_MaxHealth;
	else
		m_Health += changeBy;
}

void Stats::changeMaxHealth(float changeBy)
{
	m_MaxHealth += changeBy;
}

void Stats::changeStamina(float changeBy)
{
	if(changeBy < 0 && m_Stamina < -changeBy)
		m_Stamina = 0;
	else if(m_Stamina + changeBy > m_MaxStamina)
		m_Stamina = m_MaxStamina;
	else
		m_Stamina += changeBy;
}

void Stats::changeConfidence(float changeBy)
{
	if(m_Confidence + changeBy < 0)
		m_Confidence = 0;
	else if(m_Confidence + changeBy > 100)
		m_Confidence = 100;
	else
		m_Confidence += changeBy;
}

void Stats::changeBoredom(float changeBy)
{
	if(m_Boredom + changeBy < 0)
		m_Boredom = 0;
	else if(m_Boredom + changeBy > 100)
		m_Boredom = 100;
	else
		m_Boredom += changeBy;
}

float Stats::getDamage(float minDamage, float maxDamage)
{
	if(maxDamage < minDamage)
		Log::info("Max damage is bigger then min damage! What are you doing with your life?");

	float range = maxDamage - minDamage;

	int    trials      = 100;
	double probability = ((double) (2 * m_CombatAbility + m_Strength) / (3 * MAXLEVEL)) * (((double) m_Confidence + 100.0f) / 200.0f) * 3 / (m_Boredom + 3);

	int   randomNum  = Random::binomialDist(trials, probability);
	float percentage = (float) randomNum / trials;

	return minDamage + range * percentage;
}

void Stats::hasHitTarget(float damageDealt)
{
	changeConfidence(1 + damageDealt / 10);
	changeBoredom(-5);
	changeStamina(-10);
	m_Experience += 2 * damageDealt;
}

void Stats::hasKilledTarget()
{
	m_Experience += 50;
}

void Stats::hasMissedTarget()
{
	changeConfidence(-2);
	changeStamina(-10);
	m_Experience += 3;
}

void Stats::hasFoundChest()
{
	m_Experience += 20;
}

void Stats::hasUsedWeapon()
{
	m_RegenDelay = m_MaxRegenDelay;
}

int Stats::getWeaponDelay(int delay)
{
	float percentage = 1.0f - (2 * ((float) m_Agility / MAXLEVEL) + ((float) m_Stamina / m_MaxStamina)) / 4;
	return delay * percentage;
}

bool     Stats::isDead() { return m_Health <= 0; }
float    Stats::getHealth() { return m_Health; }
float    Stats::getMaxHealth() { return m_MaxHealth; }
float    Stats::getStamina() { return m_Stamina; }
float    Stats::getMaxStamina() { return m_MaxStamina; }
float    Stats::getMaxActiveWeapons() { return m_MaxActiveWeapons; }
float    Stats::getMaxInventory() { return m_MaxInventory; }
uint16_t Stats::getAttractiveness() { return m_Attractiveness; }

const float *Stats::getHealthPointer() { return &m_Health; }
const float *Stats::getMaxHealthPointer() { return &m_MaxHealth; }
const float *Stats::getStaminaPointer() { return &m_Stamina; }
const float *Stats::getMaxStaminaPointer() { return &m_MaxStamina; }

void Stats::addEffect(const StatsEffect &effect)
{
	m_Effects.push_back(effect);
}

void Stats::resetStats()
{
	m_Health = m_MaxHealth;
}

void Stats::setAttractiveness(uint16_t attractiveness)
{
	m_Attractiveness = attractiveness;
}