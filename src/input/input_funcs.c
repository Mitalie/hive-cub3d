#include "input_internal.h"

#include "MLX42/MLX42.h"

#include "cub3d.h"

void	input_simple_action(t_cub3d *cub3d, int arg)
{
	if (arg == INPUT_CLOSE)
		mlx_close_window(cub3d->mlx);
}

/*
	FIXME: account for facing direction
*/
void	input_move(t_cub3d *cub3d, int arg, float time)
{
	float	speed;
	float	distance;

	speed = 1.0;
	distance = speed * time;
	if (arg == INPUT_FORWARD)
		cub3d->player.y -= distance;
	if (arg == INPUT_BACKWARD)
		cub3d->player.y += distance;
	if (arg == INPUT_LEFT)
		cub3d->player.x -= distance;
	if (arg == INPUT_RIGHT)
		cub3d->player.x += distance;
}
