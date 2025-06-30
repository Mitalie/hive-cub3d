#include <stdbool.h>
#include <stddef.h>
#include "MLX42/MLX42.h"

#include "cub3d.h"
#include "input.h"
#include "render.h"

#define TEST_MAP_HEIGHT 4
#define TEST_MAP_WIDTH 4

static const char	g_test_map[TEST_MAP_HEIGHT * TEST_MAP_WIDTH] = {
	'1', '1', '1', '1',
	'1', '0', '0', '1',
	'1', '0', '1', '1',
	'1', '1', '1', '1',
};

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
