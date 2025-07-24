#include "cast_internal.h"
#include "cast.h"

#include <stdbool.h>

#include "cub3d.h"

bool	cast_station(t_cast_state *state, t_map_tile tile)
{
	t_target		*target;
	float			station_y;
	float			pos_in_tile;
	t_intersection	intersection;
	t_hit			*hit;

	if (state->cr->num_transparent == MAX_TRANSPARENT)
		return (false);
	station_y = state->tile_y + 0.5f;
	intersection = intersect_x(state->pos, state->dir, station_y);
	pos_in_tile = intersection.position_on_target - state->tile_x;
	if (pos_in_tile >= 0.0f && pos_in_tile < 1.0f)
	{
		hit = &state->cr->transparent[state->cr->num_transparent++];
		hit->distance = intersection.distance_along_ray;
		if (state->dir.y < 0)
			hit->position_in_tile = pos_in_tile;
		else
			hit->position_in_tile = 1.0f - pos_in_tile;
		if ((state->dir.y < 0) == (tile == TILE_STATION_N))
		{
			hit->material = MAT_STATION_BACK;
			return (false);
		}
		target = map_target(&state->cub3d->map, state->tile_x, state->tile_y);
		if (!target)
			hit->material = MAT_STATION_INACTIVE;
		else if (target->completed)
			hit->material = MAT_STATION_COMPLETED;
		else
			hit->material = MAT_STATION_ACTIVE;
	}
	return (false);
}
