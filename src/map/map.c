#include "map.h"

#include <stdbool.h>
#include <stdint.h>

bool	map_tile_is_wall(t_map *map, int x, int y)
{
	char	tile;

	if (x < 0
		|| (uint32_t)x >= map->width
		|| y < 0
		|| (uint32_t)y >= map->height)
		return (true);
	tile = map->grid[y * map->width + x];
	return (tile == '1');
}
