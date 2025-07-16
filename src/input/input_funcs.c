#include "input_internal.h"

#include <math.h>
#include "MLX42/MLX42.h"

#include "cub3d.h"
#include "vec2.h"

void	input_simple_action(t_cub3d *cub3d, int arg)
{
	if (arg == INPUT_CLOSE)
		mlx_close_window(cub3d->mlx);
}

/*
	First set up the move vector as if facing north, then rotate it based on
	actual facing direction and apply the rotated movement to player position.
*/
void	input_move(t_cub3d *cub3d, int arg, float time)
{
	float	speed;
	float	distance;
	t_vec2	move;

	speed = 1.0;
	if (mlx_is_key_down(cub3d->mlx, MLX_KEY_LEFT_SHIFT))
		speed = 3.0;
	distance = speed * time;
	move.x = 0;
	move.y = 0;
	if (arg == INPUT_FORWARD)
		move.y = -distance;
	if (arg == INPUT_BACKWARD)
		move.y = distance;
	if (arg == INPUT_LEFT)
		move.x = -distance;
	if (arg == INPUT_RIGHT)
		move.x = distance;
	move = vec2_rotate(move, cub3d->player_facing);
	cub3d->player.x += move.x;
	cub3d->player.y += move.y;
}

void	input_turn(t_cub3d *cub3d, int arg, float time)
{
	float	turn_speed;
	float	angle;

	turn_speed = 90.0;
	angle = turn_speed * time;
	if (arg == INPUT_TURN_LEFT)
		cub3d->player_facing = fmodf(cub3d->player_facing - angle, 360);
	if (arg == INPUT_TURN_RIGHT)
		cub3d->player_facing = fmodf(cub3d->player_facing + angle, 360);
}
