#include "input.h"
#include "input_internal.h"

#include <math.h>
#include <stddef.h>
#include "MLX42/MLX42.h"

#include "control.h"
#include "cub3d.h"

/**** Input maps **************************************************************/

static const t_input_timed	g_input_timed[] = {
{MLX_KEY_W, control_move, CONTROL_FORWARD},
{MLX_KEY_S, control_move, CONTROL_BACKWARD},
{MLX_KEY_A, control_move, CONTROL_LEFT},
{MLX_KEY_D, control_move, CONTROL_RIGHT},
{MLX_KEY_LEFT, control_turn, CONTROL_TURN_LEFT},
{MLX_KEY_Q, control_turn, CONTROL_TURN_LEFT},
{MLX_KEY_RIGHT, control_turn, CONTROL_TURN_RIGHT},
{MLX_KEY_E, control_turn, CONTROL_TURN_RIGHT},
};

static const t_input_event	g_input_event[] = {
{MLX_KEY_ESCAPE, MLX_PRESS, input_simple_action, INPUT_CLOSE},
};

/**** Dispatchers *************************************************************/

void	input_timed(t_cub3d *cub3d)
{
	size_t			i;
	t_input_timed	input;

	i = 0;
	while (i < sizeof g_input_timed / sizeof(*g_input_timed))
	{
		input = g_input_timed[i++];
		if (mlx_is_key_down(cub3d->mlx, input.key))
			input.func(cub3d, input.arg, cub3d->mlx->delta_time);
	}
}

void	input_key(t_cub3d *cub3d, mlx_key_data_t key_data)
{
	size_t			i;
	t_input_event	input;

	i = 0;
	while (i < sizeof g_input_event / sizeof(*g_input_event))
	{
		input = g_input_event[i++];
		if (key_data.action == input.action && key_data.key == input.key)
			input.func(cub3d, input.arg);
	}
}

void	input_cursor(t_cub3d *cub3d, float dx, float dy)
{
	float	degrees_per_cursor_unit;
	float	angle;

	(void)dy;
	degrees_per_cursor_unit = 0.07f;
	angle = degrees_per_cursor_unit * dx;
	cub3d->player_facing = fmodf(cub3d->player_facing + angle, 360);
}

/**** Handler functions *******************************************************/

void	input_simple_action(t_cub3d *cub3d, int arg)
{
	if (arg == INPUT_CLOSE)
		mlx_close_window(cub3d->mlx);
}
