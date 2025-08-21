/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_anim_move_y.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:20:43 by amakinen          #+#    #+#             */
/*   Updated: 2025/08/21 15:20:44 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "enemy_anim_internal.h"

static float	enemy_anim_crouch_top(float t)
{
	return (-A_CROUCH * 0.5f * t * t);
}

static float	enemy_anim_crouch_bottom(float t)
{
	t -= CROUCH_BOTTOM_TIME;
	return (-A_CROUCH * 0.5f * t * t - CROUCH_DEPTH);
}

static float	enemy_anim_jump(float t)
{
	t -= CROUCH_BOTTOM_TIME;
	return ((A_JUMP_Y) * 0.5f * t * t - CROUCH_DEPTH);
}

static float	enemy_anim_flight(float t)
{
	t -= FLIGHT_PEAK_TIME;
	return (-G * 0.5f * t * t + FLIGHT_PEAK);
}

float	enemy_anim_move_y(float t)
{
	if (t <= 0)
		return (0);
	if (t <= CROUCH_HALF_TIME)
		return (enemy_anim_crouch_top(t));
	if (t <= CROUCH_BOTTOM_TIME)
		return (enemy_anim_crouch_bottom(t));
	if (t <= JUMP_TIME)
		return (enemy_anim_jump(t));
	if (t <= FLIGHT_PEAK_TIME)
		return (enemy_anim_flight(t));
	return (enemy_anim_move_y(2 * FLIGHT_PEAK_TIME - t));
}
