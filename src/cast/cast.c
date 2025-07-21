#include "cast.h"
#include "cast_internal.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#include "cub3d.h"

/*
	If position happens to be *exactly* on grid line, ceil(pos) finds that same
	line instead of the next one over. Special case this to return the correct
	distance.
*/
static float	next_grid_line(float pos, float direction)
{
	if (direction == 0)
		return (INFINITY);
	else if (direction > 0)
		return (ceilf(pos));
	else
		return (floorf(pos));
}

bool	cross_x(t_cub3d *cub3d, t_cast_state *state, t_hit *hit)
{
	if (state->dir.y < 0
		&& map_tile_is_wall(&cub3d->map,
			state->intersection_x.position_on_target, state->grid_line_y - 1))
	{
		hit->distance = state->intersection_x.distance_along_ray;
		hit->side = HIT_SOUTH;
		hit->position_in_tile = state->intersection_x.position_on_target
			- floorf(state->intersection_x.position_on_target);
		return (true);
	}
	else if (map_tile_is_wall(&cub3d->map,
			state->intersection_x.position_on_target, state->grid_line_y))
	{
		hit->distance = state->intersection_x.distance_along_ray;
		hit->side = HIT_NORTH;
		hit->position_in_tile = 1 - (state->intersection_x.position_on_target
				- floorf(state->intersection_x.position_on_target));
		return (true);
	}
	state->grid_line_y += copysignf(1, state->dir.y);
	return (false);
}

bool	cross_y(t_cub3d *cub3d, t_cast_state *state, t_hit *hit)
{
	if (state->dir.x < 0
		&& map_tile_is_wall(&cub3d->map,
			state->grid_line_x - 1, state->intersection_y.position_on_target))
	{
		hit->distance = state->intersection_y.distance_along_ray;
		hit->side = HIT_EAST;
		hit->position_in_tile = state->intersection_y.position_on_target
			- floorf(state->intersection_y.position_on_target);
		return (true);
	}
	else if (map_tile_is_wall(&cub3d->map,
			state->grid_line_x, state->intersection_y.position_on_target))
	{
		hit->distance = state->intersection_y.distance_along_ray;
		hit->side = HIT_WEST;
		hit->position_in_tile = 1 - (state->intersection_y.position_on_target
				- floorf(state->intersection_y.position_on_target));
		return (true);
	}
	state->grid_line_x += copysignf(1, state->dir.x);
	return (false);
}

void	cast(t_cub3d *cub3d, t_vec2 pos, t_vec2 dir, t_hit *hit)
{
	t_cast_state	state;

	state.dir = dir;
	state.grid_line_x = next_grid_line(pos.x, dir.x);
	state.grid_line_y = next_grid_line(pos.y, dir.y);
	state.intersection_x = intersect_x(pos, dir, state.grid_line_y);
	state.intersection_y = intersect_y(pos, dir, state.grid_line_x);
	while (1)
	{
		if (state.intersection_x.distance_along_ray
			< state.intersection_y.distance_along_ray)
		{
			if (cross_x(cub3d, &state, hit))
				break ;
			state.intersection_x = intersect_x(pos, dir, state.grid_line_y);
		}
		else
		{
			if (cross_y(cub3d, &state, hit))
				break ;
			state.intersection_y = intersect_y(pos, dir, state.grid_line_x);
		}
	}
}
