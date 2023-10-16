#include <random>
#include <time.h>

#include "Log.h"

namespace Random
{
	std::default_random_engine generator;

	int reverseNum(int num)   // This returns a number in reverse
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

	void init()
	{   // This sets the seed NOTE: you only have to do this once in the program!
		generator.seed(reverseNum(time(NULL)));
		Log::info("Initialised random number engine");
	}

	int getNum(int min, int max)
	{
		if(min == max || min > max)
		{
			Log::critical("Min and Max are the same or max is less than min!", LOGINFO);   // TODO: Add an ability to exit the program safely
		}
		std::uniform_int_distribution<int> distribution(min, max);
		return distribution(generator);
	}

	int getWeightedNum(std::vector<float> nums)
	{
		std::discrete_distribution<int> distribution(nums.begin(), nums.end());
		return distribution(generator);
	}
}   // namespace Random