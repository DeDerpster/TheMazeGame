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
	// This initialises the generator with a seed that is the time reversed
	generator.seed(reverseNum(time(NULL)));
	Log::info("Initialised random number engine");
}

int Random::uniformDistImpl(int min, int max)
{
	// Checks if the numbers are indeed the minimum and maximum
	if(min == max || min > max)
	{
		Log::warning("Min and Max are the same or max is less than min!");
		return max;
	}

	// Creates a distribution with the minimum and maximum
	std::uniform_int_distribution<int> distribution(min, max);

	return distribution(generator);
}

int Random::binomialDistImpl(int trials, float probability)
{
	// Generates a binomial distribution with the given variables
	std::binomial_distribution<int> distribution(trials, probability);

	return distribution(generator);
}

// This will return a random index of the vector of probabilities given
int Random::customDistImpl(std::vector<float> nums)
{
	// This will return an index using the vector as a list of probabilities
	// This is useful to gain non-uniform distrutions
	std::discrete_distribution<int> distribution(nums.begin(), nums.end());

	return distribution(generator);
}

// This returns a random item
Item *Random::getItemImpl()
{
	Item *item;
	int   r = uniformDist(0, 2);
	// Has a 1/3rd chance or returning a weapon
	if(r == 0)
	{
		int r1 = uniformDist(0, 100);
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
		// Otherwise it will generate a random pointion type
		int          r1 = uniformDist(0, 8);
		Potion::Type type;
		if(r1 < 2)
		{
			int temp = r1 * POTION_SPRITES;
			int r2   = uniformDist(0, 9);
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
			// These are potion for books
			int r2 = uniformDist(0, 2);
			if(r2 == 0)
				type = Potion::Type::MagicBook;
			else
				type = Potion::Type::Book;
		}
		else   // Will return a food type
			type = Potion::Type::Food;

		item = new Potion(type);
	}

	return item;
}

// Function for inversing a number
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