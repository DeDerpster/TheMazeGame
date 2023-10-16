#pragma once

#include <random>
#include <vector>

#include "item/Item.h"

class Random
{
  private:
	std::default_random_engine generator;

	int getNumImpl(int min, int max);
	int getWeightedNumImpl(std::vector<float> nums);
	Item *getItemImpl();
	int   reverseNum(int num);

	Random();

  public:
	Random(const Random &) = delete;

	static int getNum(int min, int max) { return get().getNumImpl(min, max); }
	static int getWeightedNum(std::vector<float> nums) { return get().getWeightedNumImpl(nums); }

	static Item *getItem() { return get().getItemImpl(); }

	static Random &get()
	{
		static Random instance;
		return instance;
	}
};