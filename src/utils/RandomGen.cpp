#include "RandomGen.h"

#include <time.h>

#include "Log.h"

#include "item/potion/Potion.h"
#include "item/weapon/general/Boomerang.h"
#include "item/weapon/general/Bow.h"
#include "item/weapon/general/Crossbow.h"
#include "item/weapon/general/Sling.h"
#include "item/weapon/staff/DarkStaff.h"
#include "item/weapon/staff/EarthStaff.h"
#include "item/weapon/staff/FireStaff.h"
#include "item/weapon/staff/FrostStaff.h"
#include "item/weapon/staff/GoldStaff.h"

Random::Random()
{
	generator.seed(reverseNum(time(NULL)));
	Log::info("Initialised random number engine");
}

int Random::getNumImpl(int min, int max)
{
	if(min == max || min > max)
	{
		Log::warning("Min and Max are the same or max is less than min!");
		return max;
	}
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(generator);
}

int Random::getWeightedNumImpl(std::vector<float> nums)
{
	std::discrete_distribution<int> distribution(nums.begin(), nums.end());
	return distribution(generator);
}

Item *Random::getItemImpl()
{
	Item *item;
	int   r = getNum(0, 2);
	if(r == 0)
	{
		int r1 = getNum(0, 100);
		if(r1 < 15)
			item = new Boomerang();
		else if(r1 < 30)
			item = new Bow();
		else if(r1 < 40)
			item = new Crossbow();
		else if(r1 < 45)
			item = new DarkStaff();
		else if(r1 < 55)
			item = new EarthStaff();
		else if(r1 < 70)
			item = new FireStaff();
		else if(r1 < 80)
			item = new FrostStaff();
		else if(r1 < 83)
			item = new GoldStaff();
		else
			item = new Sling();
	}
	else
	{
		int          r1 = getNum(0, 8);
		Potion::Type type;
		if(r1 < 2)
		{
			int temp = r1 * POTION_SPRITES;
			int r2   = getNum(0, 9);
			if(r2 > 8)
				temp += 3;
			else if(r2 > 6)
				temp += 2;
			else if(r2 > 3)
				temp += 1;

			type = static_cast<Potion::Type>(temp);
		}
		else if(r1 < 4)
		{
			int r2 = getNum(0, 2);
			if(r2 == 0)
				type = Potion::Type::MagicBook;
			else
				type = Potion::Type::Book;
		}
		else
			type = Potion::Type::Food;

		item = new Potion(type);
	}

	return item;
}

int Random::reverseNum(int num)   // This returns a number in reverse
{
	int reverse = 0, rem;
	while(num != 0)
	{
		rem     = num % 10;
		reverse = reverse * 10 + rem;
		num /= 10;
	}
	return reverse;
}