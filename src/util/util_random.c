#include "util.h"

/*
	Pass zero for seed to not reseed.
	Uses Linear Congruential Generator algorithm.
*/
uint32_t	util_random(uint32_t seed)
{
	static uint32_t	state = 1;

	if (seed)
		state = seed;
	state = (1103515245 * state + 12345) & 0x7FFFFFFF;
	return (state);
}
