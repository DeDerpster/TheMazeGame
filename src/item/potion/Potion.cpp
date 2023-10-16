#include "item/potion/Potion.h"

#include "RandomGen.h"

Potion::Potion(Type type)
	: Item(std::string(), Sprite::ID::errorID)
{
	// TODO: ADD REGEN!
	switch(type)
	{
	case Type::Health:
		m_Name       = "Health Potion";
		m_SpriteID   = Sprite::ID::potionRed;
		m_EffectFunc = [](Mob *mob) {
			mob->changeHealth(25);
		};

		break;
	case Type::HealthRegen:
		m_Name       = "Health Regen Potion";
		m_SpriteID   = Sprite::ID::potionRedRegen;
		m_EffectFunc = [](Mob *mob) {
			mob->changeHealth(25);
		};

		break;
	case Type::HealthMagic:
		m_Name       = "Magic Health Potion";
		m_SpriteID   = Sprite::ID::potionRedMagic;
		m_EffectFunc = [](Mob *mob) {
			mob->changeHealth(50);
		};

		break;
	case Type::HealthHuge:
		m_Name       = "Huge Health Potion";
		m_SpriteID   = Sprite::ID::potionRedHuge;
		m_EffectFunc = [](Mob *mob) {
			mob->changeHealth(75);
		};

		break;
	case Type::Stamina:
		m_Name       = "Stamina Potion";
		m_SpriteID   = Sprite::ID::potionGreen;
		m_EffectFunc = [](Mob *mob) {
			mob->changeStamina(25);
		};

		break;
	case Type::StaminaRegen:
		m_Name       = "Stamina Regen Potion";
		m_SpriteID   = Sprite::ID::potionGreenRegen;
		m_EffectFunc = [](Mob *mob) {
			mob->changeStamina(25);
		};

		break;
	case Type::StaminaMagic:
		m_Name       = "Magic Stamina Potion";
		m_SpriteID   = Sprite::ID::potionGreenMagic;
		m_EffectFunc = [](Mob *mob) {
			mob->changeStamina(50);
		};

		break;
	case Type::StaminaHuge:
		m_Name       = "Huge Stamina Potion";
		m_SpriteID   = Sprite::ID::potionGreenHuge;
		m_EffectFunc = [](Mob *mob) {
			mob->changeStamina(75);
		};

		break;
	case Type::Book:
		m_Name       = "Book";
		m_SpriteID   = SPRITE_BOOK_START + Random::getNum(0, SPRITE_BOOK_NUM - 1);
		m_EffectFunc = [](Mob *mob) {
			mob->changeBoredom(25);
		};

		break;
	case Type::MagicBook:
		m_Name       = "Magic Book";
		m_SpriteID   = SPRITE_MAGIC_BOOK_START + Random::getNum(0, SPRITE_MAGIC_BOOK_NUM - 1);
		m_EffectFunc = [](Mob *mob) {
			mob->changeBoredom(50);
		};

		break;
	case Type::Food:
		m_SpriteID   = SPRITE_FOOD_START + Random::getNum(0, SPRITE_FOOD_NUM - 1);
		m_Name       = "Food";
		m_EffectFunc = [](Mob *mob) {
			mob->changeHealth(Random::getNum(5, 15));
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
	m_EffectFunc(mob);
}
