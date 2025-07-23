#include "map.h"

#include <stdbool.h>
#include <stdint.h>

t_map_tile	map_tile(t_map *map, int x, int y)
{
	char	tile;

	if (x < 0
		|| (uint32_t)x >= map->width
		|| y < 0
		|| (uint32_t)y >= map->height)
		return (true);
	tile = map->grid[y * map->width + x];
	if (tile == '1')
		return (TILE_WALL);
	if (tile == '-')
		return (TILE_DOOR_NS);
	if (tile == '|')
		return (TILE_DOOR_EW);
	if (tile == 'n')
		return (TILE_STATION_N);
	if (tile == 's')
		return (TILE_STATION_S);
	return (TILE_EMPTY);
}
