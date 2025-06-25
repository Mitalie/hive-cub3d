/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:00:04 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/25 19:08:31 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdbool.h>
#include "MLX42/MLX42.h"

typedef struct s_cub3d {
	mlx_t		*mlx;
	mlx_image_t *render;
	float		player_x;
	float		player_y;
	float		player_facing;
} t_cub3d;

static const char	g_grid[] = {
	'1', '1', '1', '1',
	'1', '0', '0', '1',
	'1', '0', '1', '1',
	'1', '1', '1', '1',
};

static const int	g_width = 4;

float	cast(float sx, float sy, float dx, float dy)
{
	float	tx;
	float	ty;
	int		x;
	int		y;

	x = sx;
	y = sy;
	if (dx == 0)
		tx = INFINITY;
	else if (dx > 0)
		tx = (ceilf(sx) - sx) / dx;
	else
		tx = (floor(sx) - sx) / dx;
	if (dy == 0)
		ty = INFINITY;
	else if (dy > 0)
		ty = (ceilf(sy) - sy) / dy;
	else
		ty = (floor(sy) - sy) / dy;
	while (1)
	{
		if (tx < ty) // advance in X direction
		{
			if (dx > 0)
				x++;
			else
				x--;
			if (g_grid[y * g_width + x] == '1')
				return (tx);
			tx += 1 / fabsf(dx);
		}
		else
		{
			if (dy > 0)
				y++;
			else
				y--;
			if (g_grid[y * g_width + x] == '1')
				return (ty);
			ty += 1 / fabsf(dy);
		}
	}
}

void	render(t_cub3d *cub3d)
{
	int	x;
	int	y;

	x = 0;
	while (x < (int)cub3d->render->width)
	{
		float dx = -(cub3d->render->width * 0.5) + x + 0.5;
		float dy = -(cub3d->render->width * 0.5);
		float t = cast(cub3d->player_x, cub3d->player_y, dx, dy);
		float d = t * cub3d->render->width;
		int height = 0.5 * cub3d->render->width / d;
		y = 0;
		while (y < (int)cub3d->render->height)
		{
			int horizon = cub3d->render->height / 2;
			if (y > horizon - height && y < horizon + height)
				mlx_put_pixel(cub3d->render, x, y, 0xff0000ff);
			else if (y < horizon)
				mlx_put_pixel(cub3d->render, x, y, 0xffffff);
			else
				mlx_put_pixel(cub3d->render, x, y, 0xffff00ff);
			y++;
		}
		x++;
	}
}

bool image_setup(t_cub3d *cub3d)
{
	cub3d->render = mlx_new_image(cub3d->mlx, cub3d->mlx->width, cub3d->mlx->height);
	if (!cub3d->render)
		return (false);
	if (mlx_image_to_window(cub3d->mlx, cub3d->render, 0, 0) < 0)
		return (false);
	render(cub3d);
	return (true);
}

int	main(void)
{
	t_cub3d cub3d;

	cub3d.player_x = 1.5;
	cub3d.player_y = 2.8;
	cub3d.mlx = mlx_init(800, 1200, "fdf", true);
	if (!cub3d.mlx)
		return (1);
	if (image_setup(&cub3d))
		mlx_loop(cub3d.mlx);
	if (cub3d.render)
		mlx_delete_image(cub3d.mlx, cub3d.render);
	mlx_terminate(cub3d.mlx);
}
