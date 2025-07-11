#include "map_internal.h"
#include "map.h"

#include <stdbool.h>

/*
	TODO: determine width and height, alloc grid
	TODO: validate each tile on map
	TODO: validate closed outside wall
	TODO: store player starting position
*/
static bool	map_parse_grid(t_map *map, char *file_data)
{
	if (!map_verify_parameters(map))
		return (false);
	(void)file_data;
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
