#include <stdbool.h>
#include <stddef.h>
#include "MLX42/MLX42.h"

#include "cub3d.h"
#include "input.h"
#include "map.h"
#include "render.h"

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

/*
	If you see a diagonal starting angle, fix map parsing
*/
static float	initial_facing(char player_start)
{
	if (player_start == 'N')
		return (0);
	else if (player_start == 'E')
		return (90);
	else if (player_start == 'S')
		return (180);
	else if (player_start == 'W')
		return (270);
	return (45);
}

int	main(void)
{
	t_cub3d	cub3d;

	if (!map_load(&cub3d.map, "map.cub"))
		return (1);
	cub3d.render = NULL;
	cub3d.hfov_deg = 90;
	cub3d.player.x = cub3d.map.player_x + 0.5;
	cub3d.player.y = cub3d.map.player_y + 0.5;
	cub3d.player_facing = initial_facing(cub3d.map.player_start);
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
