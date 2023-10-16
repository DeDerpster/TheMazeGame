#include "StatsMob.h"

#include "Log.h"
#include "RandomGen.h"
#include "Utils.h"
#include "math.h"

#define MAXLEVEL           15
#define BINOMIAL_PRECISION 10

StatsMob::StatsMob()
	: m_Stat_Health(100),
	  m_Stat_MaxHealth(100),
	  m_Stat_Stamina(100),
	  m_Stat_MaxStamina(100),
	  m_Stat_RegenDelay(0),
	  m_Stat_MaxRegenDelay(60),
	  m_Stat_Strength(5),
	  m_Stat_Agility(5),
	  m_Stat_CombatAbility(5),
	  m_Stat_Boredom(0),
	  m_Stat_Confidence(100),
	  m_Stat_Attractiveness(1),
	  m_Stat_MaxActiveWeapons(3),
	  m_Stat_MaxInventory(20)   // NOTE: This should not be changed
{
}

StatsMob::~StatsMob()
{
}

void StatsMob::update()
{
	if(m_Stat_RegenDelay > 0)
		m_Stat_RegenDelay--;
	else
	{
		changeHealth(1);
		changeStamina(5);
		m_Stat_RegenDelay = 10;
	}
}

void StatsMob::dealDamage(float damage)
{
	// TODO: Have strength and agility lower this
	if(m_Stat_Health < damage)
		m_Stat_Health = 0;
	else
		m_Stat_Health -= damage;

	m_Stat_RegenDelay = 100;
}

void StatsMob::changeHealth(float changeBy)
{
	if(changeBy < 0 && m_Stat_Health < -changeBy)
		m_Stat_Health = 0;
	else if(m_Stat_Health + changeBy > m_Stat_MaxHealth)
		m_Stat_Health = m_Stat_MaxHealth;
	else
		m_Stat_Health += changeBy;
}

void StatsMob::changeMaxHealth(float changeBy)
{
	m_Stat_MaxHealth += changeBy;
}

void StatsMob::changeStamina(float changeBy)
{
	if(changeBy < 0 && m_Stat_Stamina < -changeBy)
		m_Stat_Stamina = 0;
	else if(m_Stat_Stamina + changeBy > m_Stat_MaxStamina)
		m_Stat_Stamina = m_Stat_MaxStamina;
	else
		m_Stat_Stamina += changeBy;
}

void StatsMob::changeConfidence(float changeBy)
{
	if(m_Stat_Confidence + changeBy < 0)
		m_Stat_Confidence = 0;
	else if(m_Stat_Confidence + changeBy > 100)
		m_Stat_Confidence = 100;
	else
		m_Stat_Confidence += changeBy;
}

void StatsMob::changeBoredom(float changeBy)
{
	if(m_Stat_Boredom + changeBy < 0)
		m_Stat_Boredom = 0;
	else if(m_Stat_Boredom + changeBy > 100)
		m_Stat_Boredom = 100;
	else
		m_Stat_Boredom += changeBy;
}

float StatsMob::getDamage(float minDamage, float maxDamage)
{
	if(maxDamage < minDamage)
		Log::info("Max damage is bigger then min damage! What are you doing with your life?");

	float  range       = maxDamage - minDamage;
	double probability = ((double) (2 * m_Stat_CombatAbility + m_Stat_Strength) / (3 * MAXLEVEL)) * (((double) m_Stat_Confidence + 100.0f) / 200.0f) * 3 / (m_Stat_Boredom + 3);

	std::vector<float> weights;
	for(int i = 0; i < BINOMIAL_PRECISION + 1; i++)
		weights.push_back(factorial(BINOMIAL_PRECISION) / (factorial(i) * factorial(BINOMIAL_PRECISION - i)) * pow(probability, i) * pow(1 - probability, BINOMIAL_PRECISION - i));

	uint32_t randomNum  = Random::getWeightedNum(weights);
	float    percentage = (float) randomNum / BINOMIAL_PRECISION;

	return minDamage + range * percentage;
}

void StatsMob::hasHitTarget(float damageDealt)
{
	changeConfidence(1 + damageDealt / 10);
	changeBoredom(-5);
	changeStamina(-10);
}

void StatsMob::hasMissedTarget()
{
	changeConfidence(-2);
	changeStamina(-10);
}

void StatsMob::hasUsedWeapon()
{
	m_Stat_RegenDelay = m_Stat_MaxRegenDelay;
}

int StatsMob::getWeaponDelay(int delay)
{
	float percentage = 1.0f - (2 * ((float) m_Stat_Agility / MAXLEVEL) + ((float) m_Stat_Stamina / m_Stat_MaxStamina)) / 4;
	return delay * percentage;
}

bool     StatsMob::isDead() { return m_Stat_Health == 0; }
float    StatsMob::getHeath() { return m_Stat_Health; }
float    StatsMob::getMaxHeath() { return m_Stat_MaxHealth; }
float    StatsMob::getStamina() { return m_Stat_Stamina; }
float    StatsMob::getMaxStamina() { return m_Stat_MaxStamina; }
float    StatsMob::getMaxActiveWeapons() { return m_Stat_MaxActiveWeapons; }
float    StatsMob::getMaxInventory() { return m_Stat_MaxInventory; }
uint16_t StatsMob::getAttractiveness() { return m_Stat_Attractiveness; }

const float *StatsMob::getHealthPointer() { return &m_Stat_Health; }
const float *StatsMob::getMaxHealthPointer() { return &m_Stat_MaxHealth; }
const float *StatsMob::getStaminaPointer() { return &m_Stat_Stamina; }
const float *StatsMob::getMaxStaminaPointer() { return &m_Stat_MaxStamina; }

void StatsMob::resetStats()
{
	m_Stat_Health = m_Stat_MaxHealth;
}