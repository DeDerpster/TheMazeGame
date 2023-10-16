#include "RandomGen.h"

#include <time.h>

#include "Log.h"

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