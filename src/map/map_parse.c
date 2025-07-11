#include "map_internal.h"
#include "map.h"

#include <stdbool.h>
#include <stdint.h>

static bool	can_reach_out_of_bounds(t_map *map, uint32_t x, uint32_t y)
{
	if (map->grid[y * map->width + x] == '1'
		|| map->grid[y * map->width + x] == 'F')
		return (false);
	if (x == 0 || y == 0 || x == map->width - 1 || y == map->height - 1)
		return (true);
	map->grid[y * map->width + x] = 'F';
	if (can_reach_out_of_bounds(map, x + 1, y)
		|| can_reach_out_of_bounds(map, x - 1, y)
		|| can_reach_out_of_bounds(map, x, y + 1)
		|| can_reach_out_of_bounds(map, x, y - 1))
		return (true);
	return (false);
}

bool	map_verify_grid(t_map *map)
{
	if (can_reach_out_of_bounds(map, map->player_x, map->player_y))
		return (false);
	return (true);
}

bool	map_parse(t_map *map, char *file_data)
{
	char	*line_start;

	while (true)
	{
		if (*file_data == ' ' || *file_data == '\t' || *file_data == '\n')
		{
			line_start = file_data;
			while (*file_data == ' ' || *file_data == '\t')
				file_data++;
			if (*file_data != '\n')
				return (map_parse_grid(map, line_start));
			file_data++;
		}
		else if (*file_data == '1')
			return (map_parse_grid(map, file_data));
		else
		{
			if (!map_parse_parameter(map, &file_data))
				return (false);
			while (*file_data == ' ' || *file_data == '\t')
				file_data++;
			if (*file_data != '\n')
				return (false);
		}
	}
}
