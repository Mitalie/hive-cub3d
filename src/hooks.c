#include "hooks.h"

#include <stdbool.h>
#include <stdint.h>
#include "MLX42/MLX42.h"

#include "cub3d.h"
#include "enemy.h"
#include "input.h"
#include "map.h"
#include "minimap.h"
#include "render.h"

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
	if (!render_setup(cub3d) || !minimap_setup(cub3d))
	{
		mlx_close_window(cub3d->mlx);
		return ;
	}
	cub3d->frame_timestamp = mlx_get_time();
	cub3d->target_completed_tex = map_target_completed_tex(
			&cub3d->map, cub3d->frame_timestamp);
	input_timed(cub3d);
	enemy_update(cub3d);
	render_view(cub3d);
	minimap_render_fg(cub3d);
	mlx_get_window_pos(cub3d->mlx, &unused, &unused);
	//printf("%f\n", cub3d->mlx->delta_time);
}

void	key_hook(mlx_key_data_t data, void *param)
{
	t_cub3d	*cub3d;

	cub3d = param;
	input_key(cub3d, data);
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
