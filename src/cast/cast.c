#include "cast.h"
#include "cast_internal.h"

#include <math.h>
#include <stdbool.h>

#include "cub3d.h"
#include "map.h"

static bool	cross_x(t_cast_state *state)
{
	t_map_tile	tile;
	bool		terminate;

	state->tile_y += copysignf(1, state->dir.y);
	state->grid_line_y += copysignf(1, state->dir.y);
	tile = map_tile(&state->cub3d->map, state->tile_x, state->tile_y);
	if (state->dir.y < 0)
		state->cr->opaque.side = HIT_SOUTH;
	else
		state->cr->opaque.side = HIT_NORTH;
	terminate = false;
	if (tile == TILE_WALL)
		terminate = cast_wall(state, state->int_x);
	else if (tile == TILE_DOOR_NS || tile == TILE_DOOR_EW)
		terminate = cast_door(state, state->int_x);
	else if (tile == TILE_STATION_N || tile == TILE_STATION_S)
		terminate = cast_station(state, tile);
	if (terminate)
		return (true);
	state->int_x = intersect_x(state->pos, state->dir, state->grid_line_y);
	return (false);
}

static bool	cross_y(t_cast_state *state)
{
	t_map_tile	tile;
	bool		terminate;

	state->tile_x += copysignf(1, state->dir.x);
	state->grid_line_x += copysignf(1, state->dir.x);
	tile = map_tile(&state->cub3d->map, state->tile_x, state->tile_y);
	if (state->dir.x < 0)
		state->cr->opaque.side = HIT_EAST;
	else
		state->cr->opaque.side = HIT_WEST;
	terminate = false;
	if (tile == TILE_WALL)
		terminate = cast_wall(state, state->int_y);
	else if (tile == TILE_DOOR_NS || tile == TILE_DOOR_EW)
		terminate = cast_door(state, state->int_y);
	else if (tile == TILE_STATION_N || tile == TILE_STATION_S)
		terminate = cast_station(state, tile);
	if (terminate)
		return (true);
	state->int_y = intersect_y(state->pos, state->dir, state->grid_line_x);
	return (false);
}

static bool	cast_inside_door(t_cast_state *state)
{
	t_map_tile	tile;
	float		int_x_dist;
	float		int_y_dist;
	t_hit		*hit;

	hit = &state->cr->opaque;
	tile = map_tile(&state->cub3d->map, state->tile_x, state->tile_y);
	int_x_dist = state->int_x.ray_len;
	int_y_dist = state->int_y.ray_len;
	if (tile == TILE_DOOR_NS && int_y_dist < int_x_dist)
	{
		hit->distance = state->int_y.ray_len;
		hit->pos_in_tile = state->int_y.tgt_pos - floorf(state->int_y.tgt_pos);
		hit->material = MAT_DOOR_SIDE;
		return (true);
	}
	if (tile == TILE_DOOR_EW && int_x_dist < int_y_dist)
	{
		hit->distance = state->int_x.ray_len;
		hit->pos_in_tile = state->int_x.tgt_pos - floorf(state->int_x.tgt_pos);
		hit->material = MAT_DOOR_SIDE;
		return (true);
	}
	return (false);
}

static void	cast_init(t_cast_state *state,
	t_vec2 pos, t_vec2 dir, t_cast_result *cr)
{
	state->cr = cr;
	state->pos = pos;
	state->dir = dir;
	state->tile_x = floorf(pos.x);
	state->tile_y = floorf(pos.y);
	state->grid_line_x = INFINITY;
	if (dir.x < 0.0f)
		state->grid_line_x = state->tile_x;
	if (dir.x > 0.0f)
		state->grid_line_x = state->tile_x + 1;
	state->grid_line_y = INFINITY;
	if (dir.y < 0.0f)
		state->grid_line_y = state->tile_y;
	if (dir.y > 0.0f)
		state->grid_line_y = state->tile_y + 1;
	state->int_x = intersect_x(pos, dir, state->grid_line_y);
	state->int_y = intersect_y(pos, dir, state->grid_line_x);
}

void	cast(t_cub3d *cub3d, t_vec2 pos, t_vec2 dir, t_cast_result *cr)
{
	t_cast_state	state;
	bool			terminate;

	state.cub3d = cub3d;
	cast_init(&state, pos, dir, cr);
	if (cast_inside_door(&state))
		return ;
	terminate = false;
	while (!terminate)
	{
		if (state.int_x.ray_len < state.int_y.ray_len)
			terminate = cross_x(&state);
		else
			terminate = cross_y(&state);
	}
	cast_enemy(&state);
}
