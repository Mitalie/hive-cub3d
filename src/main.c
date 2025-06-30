#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include "MLX42/MLX42.h"

#include "cub3d.h"
#include "input.h"
#include "vec2.h"

#define TEST_MAP_HEIGHT 4
#define TEST_MAP_WIDTH 4

static const char	g_test_map[TEST_MAP_HEIGHT * TEST_MAP_WIDTH] = {
	'1', '1', '1', '1',
	'1', '0', '0', '1',
	'1', '0', '1', '1',
	'1', '1', '1', '1',
};

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

bool	check_hit_x(t_cub3d *cub3d, t_vec2 pos, t_vec2 dir, t_vec2 distance_to_grid, t_hit *out)
{
	if (cub3d->map_array[(int) pos.y * cub3d->map_width + (int) pos.x] == '1')
	{
		out->distance = distance_to_grid.x;
		out->side = (dir.x > 0) * HIT_WEST + (dir.x <= 0) * HIT_EAST;
		return (true);
	}
	return (false);
}

bool	check_hit_y(t_cub3d *cub3d, t_vec2 pos, t_vec2 dir, t_vec2 distance_to_grid, t_hit *out)
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
	Distance is not a two-dimensional vector, but rather separate distances to
	next vertical or horizontal grid line.
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

void	render_column(t_cub3d *cub3d, float tan_vfov, int col, t_hit *hit)
{
	int			row;
	float		tan_vert;
	float		tan_wall_height;
	uint32_t	color;

	tan_wall_height = 0.5 / hit->distance;
	row = 0;
	while (row < cub3d->height)
	{
		tan_vert = -1 * tan_vfov * ((row + 0.5) / cub3d->height - 0.5);
		if (tan_vert > -tan_wall_height && tan_vert < tan_wall_height)
			color = wall_color(hit->side, hit->position_in_tile);
		else if (tan_vert > 0)
			color = 0xffffff;
		else
			color = 0x00c060ff;
		mlx_put_pixel(cub3d->render, col, row, color);
		row++;
	}
}

void	render_view(t_cub3d *cub3d)
{
	float	tan_hfov;
	float	tan_vfov;
	t_vec2	direction;
	int		col;
	t_hit	hit;

	tan_hfov = 2 * tan(cub3d->hfov_deg / 2 * 3.1415926535 / 180);
	tan_vfov = tan_hfov * cub3d->height / cub3d->width;
	direction.y = -1;
	col = 0;
	while (col < cub3d->width)
	{
		direction.x = tan_hfov * ((col + 0.5) / cub3d->width - 0.5);
		cast(cub3d, cub3d->player, direction, &hit);
		render_column(cub3d, tan_vfov, col, &hit);
		col++;
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
	render_view(cub3d);
}

int	main(void)
{
	t_cub3d	cub3d;

	cub3d.render = NULL;
	cub3d.map_height = TEST_MAP_HEIGHT;
	cub3d.map_width = TEST_MAP_WIDTH;
	cub3d.map_array = g_test_map;
	cub3d.hfov_deg = 90;
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
