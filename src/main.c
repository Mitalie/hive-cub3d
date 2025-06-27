#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include "MLX42/MLX42.h"

#include "cub3d.h"
#include "input.h"
#include "vec2.h"

static const char	g_grid[] = {
	'1', '1', '1', '1',
	'1', '0', '0', '1',
	'1', '0', '1', '1',
	'1', '1', '1', '1',
};

static const int	g_width = 4;

float	initial_distance_to_grid(float pos, float direction)
{
	if (direction == 0)
		return (INFINITY);
	else if (direction > 0)
		return ((ceilf(pos) - pos) / direction);
	else
		return ((floor(pos) - pos) / direction);
}

typedef enum e_side
{
	HIT_NORTH,
	HIT_SOUTH,
	HIT_WEST,
	HIT_EAST,
}	t_side;

typedef struct s_hit
{
	float	distance;
	float	position_in_tile;
	t_side	side;
}	t_hit;

bool	check_hit_x(t_vec2 pos, t_vec2 dir, t_vec2 distance_to_grid, t_hit *out)
{
	if (g_grid[(int) pos.y * g_width + (int) pos.x] == '1')
	{
		out->distance = distance_to_grid.x;
		out->side = (dir.x > 0) * HIT_WEST + (dir.x <= 0) * HIT_EAST;
		return (true);
	}
	return (false);
}

bool	check_hit_y(t_vec2 pos, t_vec2 dir, t_vec2 distance_to_grid, t_hit *out)
{
	if (g_grid[(int) pos.y * g_width + (int) pos.x] == '1')
	{
		out->distance = distance_to_grid.y;
		out->side = (dir.y > 0) * HIT_NORTH + (dir.y <= 0) * HIT_SOUTH;
		return (true);
	}
	return (false);
}

/*
	Distance is not a two-dimensional vector, but rather separate distances to
	next vertical or horizontal grid line.
*/
void	cast(t_vec2 pos, t_vec2 dir, t_hit *out)
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
			if (check_hit_x(pos, dir, distance_to_grid, out))
				break ;
			distance_to_grid.x += 1 / fabsf(dir.x);
		}
		else
		{
			pos.y += dir.y / fabsf(dir.y);
			if (check_hit_y(pos, dir, distance_to_grid, out))
				break ;
			distance_to_grid.y += 1 / fabsf(dir.y);
		}
	}
	out->position_in_tile = start.x + dir.x * out->distance;
	if (distance_to_grid.x < distance_to_grid.y)
		out->position_in_tile = start.y + dir.y * out->distance;
}

uint32_t	wall_color(t_side side, float pos)
{
	uint32_t	base;
	uint32_t	out;

	pos = pos - floorf(pos);
	if (side == HIT_NORTH)
		base = 0xffff00ff;
	else if (side == HIT_SOUTH)
		base = 0xc03f3fff;
	else if (side == HIT_WEST)
		base = 0xc06000ff;
	else if (side == HIT_EAST)
		base = 0xc000ffff;
	else
		return (0xff);
	out = 0xff;
	out |= (uint32_t)((base >> 24 & 0xff) * pos) << 24;
	out |= (uint32_t)((base >> 16 & 0xff) * pos) << 16;
	out |= (uint32_t)((base >> 8 & 0xff) * pos) << 8;
	return (out);
}

void	render(t_cub3d *cub3d)
{
	int		x;
	int		y;
	t_vec2	direction;
	t_hit	hit;
	int		height;

	x = 0;
	direction.y = -(cub3d->width * 0.5);
	while (x < (int)cub3d->width)
	{
		direction.x = -(cub3d->width * 0.5) + x + 0.5;
		cast(cub3d->player, direction, &hit);
		height = 0.5 / hit.distance;
		y = 0;
		while (y < (int)cub3d->height)
		{
			int horizon = cub3d->height / 2;
			if (y > horizon - height && y < horizon + height)
				mlx_put_pixel(cub3d->render, x, y, wall_color(hit.side, hit.position_in_tile));
			else if (y < horizon)
				mlx_put_pixel(cub3d->render, x, y, 0xffffff);
			else
				mlx_put_pixel(cub3d->render, x, y, 0x00c060ff);
			y++;
		}
		x++;
	}
}

bool	image_setup(t_cub3d *cub3d)
{
	if (cub3d->render
		&& cub3d->width == cub3d->mlx->width
		&& cub3d->height == cub3d->mlx->height)
		return (true);
	if (cub3d->render)
		mlx_delete_image(cub3d->mlx, cub3d->render);
	cub3d->width = cub3d->mlx->width;
	cub3d->height = cub3d->mlx->height;
	cub3d->render = mlx_new_image(cub3d->mlx, cub3d->width, cub3d->height);
	if (!cub3d->render)
		return (false);
	if (mlx_image_to_window(cub3d->mlx, cub3d->render, 0, 0) < 0)
		return (false);
	return (true);
}

void	key_hook(mlx_key_data_t data, void *param)
{
	t_cub3d	*cub3d;

	cub3d = param;
	input_key(cub3d, data);
}

void	loop_hook(void *param)
{
	t_cub3d	*cub3d;

	cub3d = param;
	if (!image_setup(cub3d))
	{
		mlx_close_window(cub3d->mlx);
		return ;
	}
	input_timed(cub3d);
	render(cub3d);
}

int	main(void)
{
	t_cub3d	cub3d;

	cub3d.render = NULL;
	cub3d.player.x = 1.5;
	cub3d.player.y = 2.8;
	cub3d.mlx = mlx_init(800, 1200, "fdf", true);
	if (!cub3d.mlx)
		return (1);
	mlx_loop_hook(cub3d.mlx, loop_hook, &cub3d);
	mlx_key_hook(cub3d.mlx, key_hook, &cub3d);
	mlx_loop(cub3d.mlx);
	if (cub3d.render)
		mlx_delete_image(cub3d.mlx, cub3d.render);
	mlx_terminate(cub3d.mlx);
}
