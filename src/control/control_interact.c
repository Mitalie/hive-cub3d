/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control_interact.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:20:31 by amakinen          #+#    #+#             */
/*   Updated: 2025/08/21 15:20:32 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "control.h"

#include <stdbool.h>
#include <stddef.h>

#include "cub3d.h"
#include "map.h"

/*
	If door is opening (t < 0.5), no change.
	If door is open (0.5 <= t < 3.5), reset it to start of fully open (0.5).
	If door is closing (3.5 <= t < 4.0), reset it to equivalent opening position
	(same time after 0.0 as it was before 4.0).
	If door is closed (4.0 <= t), start the opening animation.
*/
static void	control_start_door_anim(t_cub3d *cub3d, t_door *door)
{
	float	anim_time;

	anim_time = cub3d->frame_timestamp - door->anim_start_time;
	if (anim_time < 0.5f)
		;
	else if (anim_time < 3.5f)
		door->anim_start_time = cub3d->frame_timestamp - 0.5f;
	else if (anim_time < 4.0f)
		door->anim_start_time = cub3d->frame_timestamp - (4.0f - anim_time);
	else
		door->anim_start_time = cub3d->frame_timestamp;
}

void	control_interact_door(t_cub3d *cub3d)
{
	size_t	i;
	float	distx;
	float	disty;
	float	distsq;

	i = -1;
	while (++i < cub3d->map.num_doors)
	{
		distx = cub3d->player.x - (cub3d->map.doors[i].x + 0.5f);
		disty = cub3d->player.y - (cub3d->map.doors[i].y + 0.5f);
		distsq = distx * distx + disty * disty;
		if (distsq < 1.44f)
			control_start_door_anim(cub3d, &cub3d->map.doors[i]);
	}
}

void	control_interact_target(t_cub3d *cub3d)
{
	size_t	i;
	float	distx;
	float	disty;
	float	distsq;

	i = -1;
	while (++i < cub3d->map.num_targets)
	{
		distx = cub3d->player.x - (cub3d->map.targets[i].x + 0.5f);
		disty = cub3d->player.y - (cub3d->map.targets[i].y + 0.5f);
		distsq = distx * distx + disty * disty;
		if (distsq < 1.44f)
			cub3d->map.targets[i].completed = true;
	}
}
