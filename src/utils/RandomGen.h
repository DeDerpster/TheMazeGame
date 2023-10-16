#pragma once

#include <random>
#include <vector>

/*namespace Random
{
	void init();

	int getNum(int min, int max);
	int getWeightedNum(std::vector<float> nums);
}   // namespace Random*/

class Random
{
  private:
	static Random s_Instance;

	std::default_random_engine generator;

	int getNumImpl(int min, int max);
	int getWeightedNumImpl(std::vector<float> nums);
	int reverseNum(int num);

	Random();

  public:
	Random(const Random &) = delete;

	static int getNum(int min, int max) { return get().getNumImpl(min, max); }
	static int getWeightedNum(std::vector<float> nums) { return get().getWeightedNumImpl(nums); }

	static Random &get()
	{
		static Random instance;
		return instance;
	}
};