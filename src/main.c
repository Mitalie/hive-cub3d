#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
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

/*
	We call to mlx_get_window_pos even though we don't need the result to work
	around a bug in MLX42 and/or GLFW that causes delays with input handling.

	After loop hook, MLX42 calls glfwPollEvents, which processes any pending
	events that have already been read from the X11 socket into the event queue.
	This includes the mouse movement events we want to handle to turn the view.
	But any new events that arrived during render are still in the socket buffer
	as nothing has triggered moving them into the queue. By calling a function
	that requires a reply from the X server, we force the connection buffer to
	be drained and get the events placed into queue so that the're available for
	processing. Otherwise we'd receive these events only after rendering the
	next frame.
*/
void	loop_hook(void *param)
{
	t_cub3d	*cub3d;
	int32_t	unused;

	cub3d = param;
	if (!image_setup(cub3d))
	{
		mlx_close_window(cub3d->mlx);
		return ;
	}
	input_timed(cub3d);
	render_view(cub3d);
	mlx_get_window_pos(cub3d->mlx, &unused, &unused);
}
}

void	cursor_hook(double x, double y, void *param)
{
	t_cub3d	*cub3d;

	cub3d = param;
	if (!cub3d->cursor_ready)
	{
		cub3d->cursor_ready = true;
		mlx_set_mouse_pos(cub3d->mlx, 0, 0);
		return ;
	}
	input_cursor(cub3d, x, y);
	mlx_set_mouse_pos(cub3d->mlx, 0, 0);
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

int	main(int argc, char **argv)
{
	t_cub3d	cub3d;

	if (argc != 2)
	{
		printf("Error\nInvalid arguments, usage: cub3D <path/to/map.cub>\n");
		return (1);
	}
	if (!map_load(&cub3d.map, argv[1]))
		return (1);
	cub3d.render = NULL;
	cub3d.hfov_deg = 90;
	cub3d.player.x = cub3d.map.player_x + 0.5;
	cub3d.player.y = cub3d.map.player_y + 0.5;
	cub3d.player_facing = initial_facing(cub3d.map.player_start);
	cub3d.mlx = mlx_init(1920, 1080, "SIM-ulator", true);
	if (!cub3d.mlx)
	{
		printf("Error\nFailed to initialize MLX42\n");
		return (1);
	}
	mlx_loop_hook(cub3d.mlx, loop_hook, &cub3d);
	mlx_key_hook(cub3d.mlx, key_hook, &cub3d);
	mlx_cursor_hook(cub3d.mlx, cursor_hook, &cub3d);
	mlx_set_cursor_mode(cub3d.mlx, MLX_MOUSE_DISABLED);
	cub3d.cursor_ready = false;
	mlx_loop(cub3d.mlx);
	if (cub3d.render)
		mlx_delete_image(cub3d.mlx, cub3d.render);
	mlx_terminate(cub3d.mlx);
}
