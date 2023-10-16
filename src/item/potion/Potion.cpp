#include "item/potion/Potion.h"

#include "RandomGen.h"
#include "entity/movableEntity/mob/stats/effects/Effect.h"

Potion::Potion(Type type)
	: Item(std::string(), Sprite::ID::errorID)
{
	// Goes through the different types and creates the basic ones
	switch(type)
	{
	case Type::Health:
		m_Name       = "Health Potion";
		m_SpriteID   = Sprite::ID::potionRed;
		m_EffectFunc = [](Mob *mob) {
			mob->getStats()->changeHealth(25);
		};

		break;
	case Type::HealthRegen:
		m_Name       = "Health Regen Potion";
		m_SpriteID   = Sprite::ID::potionRedRegen;
		m_EffectFunc = [](Mob *mob) {
			auto effect = [mob]() {
				mob->getStats()->changeHealth(2);
			};

			mob->getStats()->addEffect(StatsEffect(effect, 10));
		};

		break;
	case Type::HealthMagic:
		m_Name       = "Magic Health Potion";
		m_SpriteID   = Sprite::ID::potionRedMagic;
		m_EffectFunc = [](Mob *mob) {
			mob->getStats()->changeHealth(50);
		};

		break;
	case Type::HealthHuge:
		m_Name       = "Huge Health Potion";
		m_SpriteID   = Sprite::ID::potionRedHuge;
		m_EffectFunc = [](Mob *mob) {
			mob->getStats()->changeHealth(75);
		};

		break;
	case Type::Stamina:
		m_Name       = "Stamina Potion";
		m_SpriteID   = Sprite::ID::potionGreen;
		m_EffectFunc = [](Mob *mob) {
			mob->getStats()->changeStamina(25);
		};

		break;
	case Type::StaminaRegen:
		m_Name       = "Stamina Regen Potion";
		m_SpriteID   = Sprite::ID::potionGreenRegen;
		m_EffectFunc = [](Mob *mob) {
			auto effect = [mob]() {
				mob->getStats()->changeStamina(2);
			};

			mob->getStats()->addEffect(StatsEffect(effect, 10));
		};

		break;
	case Type::StaminaMagic:
		m_Name       = "Magic Stamina Potion";
		m_SpriteID   = Sprite::ID::potionGreenMagic;
		m_EffectFunc = [](Mob *mob) {
			mob->getStats()->changeStamina(50);
		};

		break;
	case Type::StaminaHuge:
		m_Name       = "Huge Stamina Potion";
		m_SpriteID   = Sprite::ID::potionGreenHuge;
		m_EffectFunc = [](Mob *mob) {
			mob->getStats()->changeStamina(75);
		};

		break;
	case Type::Book:
		m_Name       = "Book";
		// Uses random book sprite
		m_SpriteID   = SPRITE_BOOK_START + Random::uniformDist(0, SPRITE_BOOK_NUM - 1);
		m_EffectFunc = [](Mob *mob) {
			mob->getStats()->changeBoredom(25);
		};

		break;
	case Type::MagicBook:
		m_Name       = "Magic Book";
		// Uses random magic book sprite
		m_SpriteID   = SPRITE_MAGIC_BOOK_START + Random::uniformDist(0, SPRITE_MAGIC_BOOK_NUM - 1);
		m_EffectFunc = [](Mob *mob) {
			mob->getStats()->changeBoredom(50);
		};

		break;
	case Type::Food:
		m_Name       = "Food";
		// Uses random food sprite
		m_SpriteID   = SPRITE_FOOD_START + Random::uniformDist(0, SPRITE_FOOD_NUM - 1);
		m_EffectFunc = [](Mob *mob) {
			mob->getStats()->changeHealth(Random::uniformDist(5, 15));
		};

		break;
	default:
		Log::warning("Unknown potion!");
		m_Name       = "Error";
		m_EffectFunc = [](Mob *mob) {};
		break;
	}
}

Potion::Potion(const char *name, Sprite::ID spriteID, std::function<void(Mob *)> effect)
	: Item(name, spriteID), m_EffectFunc(effect)
{
}

Potion::~Potion()
{
}

void Potion::useOn(Mob *mob)
{
	// If the postion is used it will run the effect function
	m_EffectFunc(mob);
}
