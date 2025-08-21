/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control_turn.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:20:36 by amakinen          #+#    #+#             */
/*   Updated: 2025/08/21 15:20:37 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
