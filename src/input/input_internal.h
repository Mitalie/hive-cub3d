/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:21:02 by amakinen          #+#    #+#             */
/*   Updated: 2025/08/21 15:21:03 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	INPUT_INTERACT,
};

void	input_simple_action(t_cub3d *cub3d, int arg);

#endif
