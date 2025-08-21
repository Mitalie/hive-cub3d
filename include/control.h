/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:19:37 by amakinen          #+#    #+#             */
/*   Updated: 2025/08/21 15:19:37 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTROL_H
# define CONTROL_H

# include "cub3d.h"

enum e_control_move_dir
{
	CONTROL_FORWARD,
	CONTROL_BACKWARD,
	CONTROL_LEFT,
	CONTROL_RIGHT,
	CONTROL_TURN_LEFT,
	CONTROL_TURN_RIGHT,
};

void	control_move(t_cub3d *cub3d, int arg, float time);
void	control_turn(t_cub3d *cub3d, int arg, float time);

void	control_interact_door(t_cub3d *cub3d);
void	control_interact_target(t_cub3d *cub3d);

#endif
