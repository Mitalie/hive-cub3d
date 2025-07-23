#include "cast_internal.h"
#include "cast.h"

#include <math.h>
#include <stdbool.h>

#include "cub3d.h"

bool	cast_wall(t_cub3d *cub3d, t_cast_state *state, t_hit *hit,
	t_intersection intersection)
{
	float	position;

	(void)cub3d;
	(void)state;
	hit->distance = intersection.distance_along_ray;
	position = intersection.position_on_target;
	if (hit->side == HIT_NORTH || hit->side == HIT_WEST)
		hit->position_in_tile = ceilf(position) - position;
	else
		hit->position_in_tile = position - floorf(position);
	return (true);
}
