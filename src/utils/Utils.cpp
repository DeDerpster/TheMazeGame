#include "Utils.h"

uint32_t factorial(int num)
{
	uint32_t output = 1;
	for(uint16_t i = 1; i <= num; i++)
		output *= i;
	return output;
}