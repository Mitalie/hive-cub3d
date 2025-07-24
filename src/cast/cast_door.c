#include "cast_internal.h"
#include "cast.h"

#include <math.h>
#include <stdbool.h>

#include "cub3d.h"
#include "map.h"

/*
	`map_door` should never return `NULL`, because we only call `cast_door` if
	`map_tile` returned `TILE_DOOR_NS` or `TILE_DOOR_EW`. If this happens, the
	map parsing is broken, but we just assume it is not.

	time since anim start 0.0-0.5 -> state 0.0-1.0
	time since anim start 0.5-3.5 -> state 1.0
	time since anim start 3.5-4.0 -> state 1.0-0.0
	time since anim start 4.0-inf -> state 0.0

	red = [0 * (1 - state), 0.5 * (1 - state)]
	green = [0.5 * (1 + state), 1]
*/

static bool	cast_door_yellow(t_cast_state *state, float door_state)
{
	float			yellow_line_pos;
	t_intersection	intersection;
	t_hit			*hit;

	hit = &state->cr->opaque;
	if (hit->side == HIT_NORTH || hit->side == HIT_SOUTH)
	{
		yellow_line_pos = state->tile_x + 0.5f + copysignf(0.5f * door_state, state->dir.x);
		intersection = intersect_y(state->pos, state->dir, yellow_line_pos);
		if (isinff(intersection.distance_along_ray) || floorf(intersection.position_on_target) != state->tile_y)
			return (false);
		hit->distance = intersection.distance_along_ray;
		hit->position_in_tile = intersection.position_on_target - floorf(intersection.position_on_target);
		hit->material = MAT_DOOR_SIDE;
		return (true);
	}
	else
	{
		yellow_line_pos = state->tile_y + 0.5f + copysignf(0.5f * door_state, state->dir.y);
		intersection = intersect_x(state->pos, state->dir, yellow_line_pos);
		if (isinff(intersection.distance_along_ray) || floorf(intersection.position_on_target) != state->tile_x)
			return (false);
		hit->distance = intersection.distance_along_ray;
		hit->position_in_tile = intersection.position_on_target - floorf(intersection.position_on_target);
		hit->material = MAT_DOOR_SIDE;
		return (true);
	}
}

bool	cast_door(t_cast_state *state, t_intersection intersection)
{
	float	door_state;
	float	front_pos;
	float	red_pos;
	float	green_pos;
	t_hit	*hit;

	hit = &state->cr->opaque;
	door_state = map_door_state(&state->cub3d->map, state->tile_x, state->tile_y);
	front_pos = intersection.position_on_target;
	if (hit->side == HIT_NORTH || hit->side == HIT_EAST)
		front_pos = ceilf(front_pos) - front_pos;
	else
		front_pos = front_pos - floorf(front_pos);
	red_pos = front_pos + 0.5f * door_state;
	green_pos = front_pos - 0.5f * door_state;
	if (red_pos <= 0.5f)
	{
		hit->distance = intersection.distance_along_ray;
		hit->position_in_tile = red_pos;
		hit->material = MAT_DOOR_FACE;
		return (true);
	}
	if (green_pos >= 0.5f)
	{
		hit->distance = intersection.distance_along_ray;
		hit->position_in_tile = green_pos;
		hit->material = MAT_DOOR_FACE;
		return (true);
	}
	return (cast_door_yellow(state, door_state));
}
