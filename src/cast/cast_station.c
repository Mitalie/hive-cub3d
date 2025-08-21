/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_station.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:20:22 by amakinen          #+#    #+#             */
/*   Updated: 2025/08/21 15:20:23 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cast_internal.h"
#include "cast.h"

#include <stdbool.h>

#include "cub3d.h"
#include "map.h"
#include "material.h"

static t_material	cast_station_material(t_cast_state *state, t_map_tile tile)
{
	t_target	*target;

	if ((state->dir.y < 0) == (tile == TILE_STATION_N))
		return (MAT_TGT_BACK);
	target = map_target(&state->cub3d->map,
			state->tile_x, state->tile_y);
	if (!target)
		return (MAT_TGT_INACTIVE);
	else if (target->completed)
		return (MAT_TGT_COMPLETED);
	else
		return (MAT_TGT_ACTIVE);
}

bool	cast_station(t_cast_state *state, t_map_tile tile)
{
	float			station_y;
	float			pos_in_tile;
	t_intersection	intersection;
	t_hit			*hit;

	if (state->cr->num_transparent == MAX_TRANSPARENT)
		return (false);
	station_y = state->tile_y + 0.5f;
	intersection = intersect_x(state->pos, state->dir, station_y);
	pos_in_tile = intersection.tgt_pos - state->tile_x;
	if (pos_in_tile >= 0.0f && pos_in_tile < 1.0f)
	{
		hit = &state->cr->transparent[state->cr->num_transparent++];
		hit->distance = intersection.ray_len;
		if (state->dir.y < 0)
			hit->pos_in_tile = pos_in_tile;
		else
			hit->pos_in_tile = 1.0f - pos_in_tile;
		hit->material = cast_station_material(state, tile);
	}
	return (false);
}
