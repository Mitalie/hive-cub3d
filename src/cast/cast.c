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
static float	cast_initial_distance_to_grid(float pos, float direction)
{
	if (direction == 0)
		return (INFINITY);
	else if (direction > 0)
	{
		if ((ceilf(pos) == pos))
			return (1 / direction);
		return ((ceilf(pos) - pos) / direction);
	}
	else
		return ((pos - floorf(pos)) / -direction);
}

static bool	cast_check_wall(t_cub3d *cub3d, t_cast_state *state)
{
	char	tile;

	if (state->grid_x < 0
		|| (uint32_t)state->grid_x >= cub3d->map.width
		|| state->grid_y < 0
		|| (uint32_t)state->grid_y >= cub3d->map.height)
		return (true);
	tile = cub3d->map.grid[state->grid_y * cub3d->map.width + state->grid_x];
	return (tile == '1');
}

static bool	cast_advance_x(t_cub3d *cub3d, t_cast_state *state, t_hit *out)
{
	if (state->dir.x > 0)
		state->grid_x++;
	else
		state->grid_x--;
	if (cast_check_wall(cub3d, state))
	{
		out->distance = state->distance_to_grid_x;
		if (state->dir.x > 0)
			out->side = HIT_WEST;
		else
			out->side = HIT_EAST;
		return (true);
	}
	state->distance_to_grid_x += 1 / fabsf(state->dir.x);
	return (false);
}

static bool	cast_advance_y(t_cub3d *cub3d, t_cast_state *state, t_hit *out)
{
	if (state->dir.y > 0)
		state->grid_y++;
	else
		state->grid_y--;
	if (cast_check_wall(cub3d, state))
	{
		out->distance = state->distance_to_grid_y;
		if (state->dir.y > 0)
			out->side = HIT_NORTH;
		else
			out->side = HIT_SOUTH;
		return (true);
	}
	state->distance_to_grid_y += 1 / fabsf(state->dir.y);
	return (false);
}

void	cast(t_cub3d *cub3d, t_vec2 pos, t_vec2 dir, t_hit *out)
{
	t_cast_state	state;

	state.grid_x = floorf(pos.x);
	state.grid_y = floorf(pos.y);
	state.dir = dir;
	state.distance_to_grid_x = cast_initial_distance_to_grid(pos.x, dir.x);
	state.distance_to_grid_y = cast_initial_distance_to_grid(pos.y, dir.y);
	while (1)
	{
		if (state.distance_to_grid_x < state.distance_to_grid_y)
		{
			if (cast_advance_x(cub3d, &state, out))
				break ;
		}
		else
			if (cast_advance_y(cub3d, &state, out))
				break ;
	}
	if (state.distance_to_grid_x < state.distance_to_grid_y)
		out->position_in_tile = pos.y + dir.y * out->distance;
	else
		out->position_in_tile = pos.x + dir.x * out->distance;
}
