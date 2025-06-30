#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include "MLX42/MLX42.h"

typedef struct s_cub3d
{
	mlx_t		*mlx;
	mlx_image_t	*render;
	int			width;
	int			height;
	float		player_x;
	float		player_y;
	float		player_facing;
}	t_cub3d;

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
	while (x < (int)cub3d->width)
	{
		float dx = -(cub3d->width * 0.5) + x + 0.5;
		float dy = -(cub3d->width * 0.5);
		float t = cast(cub3d->player_x, cub3d->player_y, dx, dy);
		float d = t * cub3d->width;
		int height = 0.5 * cub3d->width / d;
		y = 0;
		while (y < (int)cub3d->height)
		{
			int horizon = cub3d->height / 2;
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

void	key_hooks(mlx_key_data_t data, void *param)
{
	t_cub3d	*cub3d;
	float	x;
	float	y;

	cub3d = param;
	x = cub3d->player_x;
	y = cub3d->player_y;
	if (data.key == MLX_KEY_ESCAPE && data.action == MLX_RELEASE)
		mlx_close_window(cub3d->mlx);
	else if ((data.key == MLX_KEY_UP && data.action == MLX_PRESS)
		|| (data.key == MLX_KEY_W && data.action == MLX_PRESS))
		cub3d->player_y += 0.1;
	else if ((data.key == MLX_KEY_DOWN && data.action == MLX_PRESS)
		|| (data.key == MLX_KEY_S && data.action == MLX_PRESS))
		cub3d->player_y -= 0.1;
	else if ((data.key == MLX_KEY_LEFT && data.action == MLX_PRESS)
		|| (data.key == MLX_KEY_A && data.action == MLX_PRESS))
		cub3d->player_x -= 0.1;
	else if ((data.key == MLX_KEY_RIGHT && data.action == MLX_PRESS)
		|| (data.key == MLX_KEY_D && data.action == MLX_PRESS))
		cub3d->player_x += 0.1;
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
	render(cub3d);
}

int	main(void)
{
	t_cub3d	cub3d;

	cub3d.render = NULL;
	cub3d.player_x = 1.5;
	cub3d.player_y = 2.8;
	cub3d.mlx = mlx_init(800, 1200, "fdf", true);
	if (!cub3d.mlx)
		return (1);
	mlx_loop_hook(cub3d.mlx, loop_hook, &cub3d);
	mlx_key_hook(cub3d.mlx, key_hooks, &cub3d);
	mlx_loop(cub3d.mlx);
	if (cub3d.render)
		mlx_delete_image(cub3d.mlx, cub3d.render);
	mlx_terminate(cub3d.mlx);
}
