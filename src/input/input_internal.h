#ifndef INPUT_INTERNAL_H
# define INPUT_INTERNAL_H

# include "MLX42/MLX42.h"

# include "cub3d.h"

/**** Types for input maps ****************************************************/

typedef void (t_input_timed_func)(t_cub3d *cub3d, int arg, float time);

typedef struct s_input_timed
{
	keys_t				key;
	t_input_timed_func	*func;
	int					arg;
}	t_input_timed;

typedef void (t_input_event_func)(t_cub3d *cub3d, int arg);

typedef struct s_input_event
{
	keys_t				key;
	action_t			action;
	t_input_event_func	*func;
	int					arg;
}	t_input_event;

/**** Handler functions *******************************************************/

enum e_input_simple_action
{
	INPUT_CLOSE,
};

void	input_simple_action(t_cub3d *cub3d, int arg);

enum e_input_move_dir
{
	INPUT_FORWARD,
	INPUT_BACKWARD,
	INPUT_LEFT,
	INPUT_RIGHT,
	INPUT_TURN_LEFT,
	INPUT_TURN_RIGHT,
};

void	input_move(t_cub3d *cub3d, int arg, float time);
void	input_turn(t_cub3d *cub3d, int arg, float time);

#endif
