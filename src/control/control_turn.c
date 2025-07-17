#include "control.h"

#include <math.h>

#include "cub3d.h"

void	control_turn(t_cub3d *cub3d, int arg, float time)
{
	float	turn_speed;
	float	angle;

	turn_speed = 90.0;
	angle = turn_speed * time;
	if (arg == CONTROL_TURN_LEFT)
		cub3d->player_facing = fmodf(cub3d->player_facing - angle, 360);
	if (arg == CONTROL_TURN_RIGHT)
		cub3d->player_facing = fmodf(cub3d->player_facing + angle, 360);
}
