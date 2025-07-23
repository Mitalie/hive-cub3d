#include "util.h"

#include <stdlib.h>

/*
	Pass zero for seed to not reseed.

	TODO: implement without stdlib random
*/
uint32_t	util_random(uint32_t seed)
{
	if (seed)
		srandom(seed);
	return (random());
}
