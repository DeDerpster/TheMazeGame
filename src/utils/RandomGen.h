#pragma once

namespace Random
{
	void init();

	int getNum(int min, int max);
	int getWeightedNum(std::vector<float> nums);
}   // namespace Random