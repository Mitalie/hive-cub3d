/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_door.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:20:11 by amakinen          #+#    #+#             */
/*   Updated: 2025/08/21 15:20:12 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cast_internal.h"
#include "cast.h"

#include <math.h>
#include <stdbool.h>

#include "cub3d.h"
#include "map.h"
#include "material.h"

static bool	cast_door_edge_ns(t_cast_state *state, t_hit *hit, float door_state)
{
	float			edge_x;
	t_intersection	intersection;

	edge_x = state->tile_x + 0.5f + copysignf(0.5f * door_state, state->dir.x);
	intersection = intersect_y(state->pos, state->dir, edge_x);
	if (isinff(intersection.ray_len)
		|| floorf(intersection.tgt_pos) != state->tile_y)
		return (false);
	hit->distance = intersection.ray_len;
	hit->pos_in_tile = intersection.tgt_pos - floorf(intersection.tgt_pos);
	hit->material = MAT_DOOR_SIDE;
	return (true);
}

static bool	cast_door_edge_ew(t_cast_state *state, t_hit *hit, float door_state)
{
	float			edge_y;
	t_intersection	intersection;

	edge_y = state->tile_y + 0.5f + copysignf(0.5f * door_state, state->dir.y);
	intersection = intersect_x(state->pos, state->dir, edge_y);
	if (isinff(intersection.ray_len)
		|| floorf(intersection.tgt_pos) != state->tile_x)
		return (false);
	hit->distance = intersection.ray_len;
	hit->pos_in_tile = intersection.tgt_pos - floorf(intersection.tgt_pos);
	hit->material = MAT_DOOR_SIDE;
	return (true);
}

static bool	cast_door_face(t_hit *hit, float door_state,
	t_intersection intersection)
{
	float	pos_on_face;
	float	pos_on_left_half;
	float	pos_on_right_half;

	pos_on_face = intersection.tgt_pos;
	if (hit->side == HIT_NORTH || hit->side == HIT_EAST)
		pos_on_face = ceilf(pos_on_face) - pos_on_face;
	else
		pos_on_face = pos_on_face - floorf(pos_on_face);
	pos_on_left_half = pos_on_face + 0.5f * door_state;
	pos_on_right_half = pos_on_face - 0.5f * door_state;
	if (pos_on_left_half < 0.5f)
		hit->pos_in_tile = pos_on_left_half;
	else if (pos_on_right_half >= 0.5f)
		hit->pos_in_tile = pos_on_right_half;
	else
		return (false);
	hit->distance = intersection.ray_len;
	hit->material = MAT_DOOR_FACE;
	return (true);
}

bool	cast_door(t_cast_state *state, t_intersection intersection)
{
	t_hit	*hit;
	float	door_state;

	hit = &state->cr->opaque;
	door_state = map_door_state(&state->cub3d->map, state->tile_x,
			state->tile_y, state->cub3d->frame_timestamp);
	if (cast_door_face(hit, door_state, intersection))
		return (true);
	if (hit->side == HIT_NORTH || hit->side == HIT_SOUTH)
		return (cast_door_edge_ns(state, hit, door_state));
	else
		return (cast_door_edge_ew(state, hit, door_state));
}
