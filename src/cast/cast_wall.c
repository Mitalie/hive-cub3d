#include "cast_internal.h"
#include "cast.h"

#include <math.h>
#include <stdbool.h>

#include "cub3d.h"

bool	cast_wall(t_cast_state *state, t_intersection intersection)
{
	float	position;
	t_hit	*hit;

	hit = &state->cr->opaque;
	hit->distance = intersection.ray_len;
	position = intersection.tgt_pos;
	if (hit->side == HIT_NORTH)
		hit->material = MAT_WALL_NORTH;
	if (hit->side == HIT_SOUTH)
		hit->material = MAT_WALL_SOUTH;
	if (hit->side == HIT_EAST)
		hit->material = MAT_WALL_EAST;
	if (hit->side == HIT_WEST)
		hit->material = MAT_WALL_WEST;
	if (hit->side == HIT_NORTH || hit->side == HIT_EAST)
		hit->pos_in_tile = ceilf(position) - position;
	else
		hit->pos_in_tile = position - floorf(position);
	return (true);
}
