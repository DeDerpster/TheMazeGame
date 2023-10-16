#pragma once

#include <random>
#include <vector>

#include "item/Item.h"

// This is a static class for being the center of randomness storing the generator needed
class Random
{
  private:
	std::default_random_engine generator;

	// private implementations of each functions
	int   uniformDistImpl(int min, int max);
	int   binomialDistImpl(int trials, float probability);
	int   customDistImpl(std::vector<float> nums);
	Item *getItemImpl();

	// This function has just been added in so that it can reverse the time
	int   reverseNum(int num);

	Random();

  public:
	Random(const Random &) = delete;

	// Generates a random number using a uniform distribution
	static int uniformDist(int min, int max) { return get().uniformDistImpl(min, max); }

	// Generates a random number using a binomial distribution
	static int binomialDist(int trials, float probability) { return get().binomialDistImpl(trials, probability); }

	// Generates a random index using a given list of probabilities
	static int customDist(std::vector<float> nums) { return get().customDistImpl(nums); }

	// Generates a random item
	static Item *getItem() { return get().getItemImpl(); }

	static Random &get()
	{
		static Random instance;
		return instance;
	}
};