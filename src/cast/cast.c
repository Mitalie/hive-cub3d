#include "cast.h"

#include <math.h>

#include "cub3d.h"

static float	initial_distance_to_grid(float pos, float direction)
{
	if (direction == 0)
		return (INFINITY);
	else if (direction > 0)
		return ((ceilf(pos) - pos) / direction);
	else
		return ((floor(pos) - pos) / direction);
}

static bool	check_hit_x(t_cub3d *cub3d, t_vec2 pos, t_vec2 dir, t_vec2 distance_to_grid, t_hit *out)
{
	if (cub3d->map_array[(int) pos.y * cub3d->map_width + (int) pos.x] == '1')
	{
		out->distance = distance_to_grid.x;
		out->side = (dir.x > 0) * HIT_WEST + (dir.x <= 0) * HIT_EAST;
		return (true);
	}
	return (false);
}

static bool	check_hit_y(t_cub3d *cub3d, t_vec2 pos, t_vec2 dir, t_vec2 distance_to_grid, t_hit *out)
{
	if (cub3d->map_array[(int) pos.y * cub3d->map_width + (int) pos.x] == '1')
	{
		out->distance = distance_to_grid.y;
		out->side = (dir.y > 0) * HIT_NORTH + (dir.y <= 0) * HIT_SOUTH;
		return (true);
	}
	return (false);
}

/*
	distance_to_grid is not a two-dimensional vector, but rather separate
	distances to next vertical or horizontal grid line.
*/
void	cast(t_cub3d *cub3d, t_vec2 pos, t_vec2 dir, t_hit *out)
{
	const t_vec2	start = pos;
	t_vec2			distance_to_grid;

	distance_to_grid.x = initial_distance_to_grid(pos.x, dir.x);
	distance_to_grid.y = initial_distance_to_grid(pos.y, dir.y);
	while (1)
	{
		if (distance_to_grid.x < distance_to_grid.y)
		{
			pos.x += dir.x / fabsf(dir.x);
			if (check_hit_x(cub3d, pos, dir, distance_to_grid, out))
				break ;
			distance_to_grid.x += 1 / fabsf(dir.x);
		}
		else
		{
			pos.y += dir.y / fabsf(dir.y);
			if (check_hit_y(cub3d, pos, dir, distance_to_grid, out))
				break ;
			distance_to_grid.y += 1 / fabsf(dir.y);
		}
	}
	out->position_in_tile = start.x + dir.x * out->distance;
	if (distance_to_grid.x < distance_to_grid.y)
		out->position_in_tile = start.y + dir.y * out->distance;
}
