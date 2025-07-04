#include "vec2.h"

#include <math.h>

#define DEG_TO_RAD 0.017453292519943295769f

t_vec2	vec2_rotate(t_vec2 v, float degrees)
{
	t_vec2	rotated;
	float	radians;

	radians = degrees * DEG_TO_RAD;
	rotated.x = v.x * cosf(radians) - v.y * sinf(radians);
	rotated.y = v.x * sinf(radians) + v.y * cosf(radians);
	return (rotated);
}
