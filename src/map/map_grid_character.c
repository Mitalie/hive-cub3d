#include "map_internal.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#include "util.h"

static bool	map_grid_door(t_map *map, uint32_t x, uint32_t y)
{
	if (map->num_doors == MAX_DOORS)
		return (util_err_false("Map error", "too many doors"));
	map->doors[map->num_doors].x = x;
	map->doors[map->num_doors].y = y;
	map->doors[map->num_doors].anim_start_time = -INFINITY;
	map->num_doors++;
	return (true);
}

/*
	Insert the new station into a random index in a list of stations, shifting
	other stations out of the way if necessary. But wait, we're storing only
	MAX_TARGETS stations, so do nothing if the new station would end up outside
	the array, and don't try to shift the last entry if it would end up outside
	the array.

	`num_targets_stored` means the number of existing targets in the array to
	preserve, in addition to the new one we're adding.
*/
static bool	map_grid_station(t_map *map, uint32_t x, uint32_t y)
{
	uint32_t	index;
	uint32_t	num_targets_stored;

	if (map->num_targets == MAX_STATIONS)
		return (util_err_false("Map error", "too many stations"));
	index = util_random(0) % (map->num_targets + 1);
	if (index >= MAX_TARGETS)
	{
		map->num_targets++;
		return (true);
	}
	num_targets_stored = map->num_targets;
	if (num_targets_stored >= MAX_TARGETS)
		num_targets_stored = MAX_TARGETS - 1;
	if (index < num_targets_stored)
	{
		util_memmove(&map->targets[index + 1], &map->targets[index],
			(num_targets_stored - index) * sizeof(*map->targets));
	}
	map->targets[index].x = x;
	map->targets[index].y = y;
	map->targets[index].completed = false;
	map->num_targets++;
	return (true);
}

static bool	map_grid_start(t_map *map, uint32_t x, uint32_t y, char c)
{
	if (map->player_start)
		return (util_err_false("Map error", "duplicate start"));
	map->player_start = c;
	map->player_x = x;
	map->player_y = y;
	return (true);
}

bool	map_check_grid_character(t_map *map, char c, uint32_t col)
{
	if (c == ' ' || c == '1' || c == '0')
		return (true);
	if (c == '-' || c == '|')
		return (map_grid_door(map, col, map->height));
	if (c == 'n' || c == 's')
		return (map_grid_station(map, col, map->height));
	if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
		return (map_grid_start(map, col, map->height, c));
	return (util_err_false("Map error", "invalid character in grid"));
}
