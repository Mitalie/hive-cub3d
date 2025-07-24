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
	t_map_tile	tile;

	state->tile_y += copysignf(1, state->dir.y);
	state->grid_line_y += copysignf(1, state->dir.y);
	tile = map_tile(&cub3d->map, state->tile_x, state->tile_y);
	if (state->dir.y < 0)
		hit->side = HIT_SOUTH;
	else
		hit->side = HIT_NORTH;
	if (tile == TILE_WALL)
		return (cast_wall(cub3d, state, hit, state->intersection_x));
	else if (tile == TILE_DOOR_NS || tile == TILE_DOOR_EW)
		return (cast_door(cub3d, state, hit, state->intersection_x));
	else if (tile == TILE_STATION_N || tile == TILE_STATION_S)
		return (cast_station(cub3d, state, hit, state->intersection_x));
	return (false);
}

bool	cross_y(t_cub3d *cub3d, t_cast_state *state, t_hit *hit)
{
	t_map_tile	tile;

	state->tile_x += copysignf(1, state->dir.x);
	state->grid_line_x += copysignf(1, state->dir.x);
	tile = map_tile(&cub3d->map, state->tile_x, state->tile_y);
	if (state->dir.x < 0)
		hit->side = HIT_EAST;
	else
		hit->side = HIT_WEST;
	if (tile == TILE_WALL)
		return (cast_wall(cub3d, state, hit, state->intersection_y));
	else if (tile == TILE_DOOR_NS || tile == TILE_DOOR_EW)
		return (cast_door(cub3d, state, hit, state->intersection_y));
	else if (tile == TILE_STATION_N || tile == TILE_STATION_S)
		return (cast_station(cub3d, state, hit, state->intersection_y));
	return (false);
}

static bool	cast_inside_door(t_cub3d *cub3d, t_cast_state *state, t_hit *hit)
{
	t_map_tile	tile;
	float		int_x_dist;
	float		int_y_dist;

	tile = map_tile(&cub3d->map, state->tile_x, state->tile_y);
	int_x_dist = state->intersection_x.distance_along_ray;
	int_y_dist = state->intersection_y.distance_along_ray;
	if (tile == TILE_DOOR_NS && int_y_dist < int_x_dist)
	{
		hit->distance = state->intersection_y.distance_along_ray;
		hit->position_in_tile = state->intersection_y.position_on_target - floorf(state->intersection_y.position_on_target);
		hit->material = MAT_DOOR_SIDE;
		return (true);
	}
	if (tile == TILE_DOOR_EW && int_x_dist < int_y_dist)
	{
		hit->distance = state->intersection_x.distance_along_ray;
		hit->position_in_tile = state->intersection_x.position_on_target - floorf(state->intersection_x.position_on_target);
		hit->material = MAT_DOOR_SIDE;
		return (true);
	}
	return (false);
}

void	cast(t_cub3d *cub3d, t_vec2 pos, t_vec2 dir, t_hit *hit)
{
	t_cast_state	state;

	state.pos = pos;
	state.dir = dir;
	state.tile_x = floorf(pos.x);
	state.tile_y = floorf(pos.y);
	state.grid_line_x = next_grid_line(pos.x, dir.x);
	state.grid_line_y = next_grid_line(pos.y, dir.y);
	state.intersection_x = intersect_x(pos, dir, state.grid_line_y);
	state.intersection_y = intersect_y(pos, dir, state.grid_line_x);
	if (cast_inside_door(cub3d, &state, hit))
		return ;
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
