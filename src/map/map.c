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

t_door	*map_door(t_map *map, int x, int y)
{
	uint32_t	i;
	t_door		*door;

	if (x < 0
		|| (uint32_t)x >= map->width
		|| y < 0
		|| (uint32_t)y >= map->height)
		return (NULL);
	i = 0;
	while (i < map->num_doors)
	{
		door = &map->doors[i++];
		if (door->x == (uint32_t)x && door->y == (uint32_t)y)
			return (door);
	}
	return (NULL);
}

t_target	*map_target(t_map *map, int x, int y)
{
	uint32_t	i;
	t_target	*target;

	if (x < 0
		|| (uint32_t)x >= map->width
		|| y < 0
		|| (uint32_t)y >= map->height)
		return (NULL);
	i = 0;
	while (i < map->num_targets)
	{
		target = &map->targets[i++];
		if (target->x == (uint32_t)x && target->y == (uint32_t)y)
			return (target);
	}
	return (NULL);
}
