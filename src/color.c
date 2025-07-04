#include "color.h"

#include <math.h>
#include <stdint.h>

uint32_t	color_interp(uint32_t a, uint32_t b, float t)
{
	uint32_t	i;
	uint32_t	out;
	float		at;
	float		bt;

	t = fmaxf(0, fminf(1, t));
	i = 0;
	out = 0;
	while (i < 32)
	{
		at = (a >> i & 0xff) * (1 - t);
		bt = (b >> i & 0xff) * t;
		out |= (uint32_t)(at + bt) << i;
		i += 8;
	}
	return (out);
}
