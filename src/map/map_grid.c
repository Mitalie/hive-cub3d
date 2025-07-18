#include "map_internal.h"
#include "map.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "util.h"

static bool	map_check_grid_character(t_map *map, char c, uint32_t col)
{
	if (c == ' ' || c == '1' || c == '0')
		return (true);
	if (c == 'n' || c == 's')
		return (true);
	if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
	{
		if (map->player_start)
			return (util_err_false("Map error", "duplicate start"));
		map->player_start = c;
		map->player_x = col;
		map->player_y = map->height;
		return (true);
	}
	return (util_err_false("Map error", "invalid character in grid"));
}

/*
	Validate line in the map file. Return either number of characters consumed
	or zero for invalid line. Empty lines are never valid.
*/
static size_t	map_check_line(t_map *map, char *line_start)
{
	bool	empty;
	size_t	i;

	empty = true;
	i = 0;
	while (line_start[i] && line_start[i] != '\n')
	{
		if (!map_check_grid_character(map, line_start[i], i))
			return (0);
		if (line_start[i] != ' ')
			empty = false;
		i++;
	}
	if (empty)
		return (util_err_false("Map error", "empty line in grid"));
	if (i > map->width)
		map->width = i;
	if (line_start[i] == '\n')
		i++;
	return (i);
}

static bool	map_count_and_alloc(t_map *map, char *file_data)
{
	size_t	line_len;
	size_t	i;

	i = 0;
	map->width = 0;
	map->height = 0;
	while (file_data[i])
	{
		line_len = map_check_line(map, file_data + i);
		if (line_len == 0)
			return (false);
		map->height++;
		i += line_len;
	}
	if (!map->player_start)
		return (util_err_false("Map error", "no starting position"));
	map->grid = malloc(sizeof(map->grid[0]) * map->width * map->height);
	if (!map->grid)
		return (util_err_false("Loading map failed", "malloc failed"));
	return (true);
}

static void	map_fill_grid(t_map *map, char *file_data)
{
	uint32_t	row;
	uint32_t	col;
	size_t		i;

	i = 0;
	row = 0;
	while (row < map->height)
	{
		col = 0;
		while (col < map->width)
		{
			if (file_data[i] && file_data[i] != '\n')
				map->grid[row * map->width + col++] = file_data[i++];
			else
				map->grid[row * map->width + col++] = ' ';
		}
		if (file_data[i] == '\n')
			i++;
		row++;
	}
}

bool	map_parse_grid(t_map *map, char *file_data)
{
	if (!map_verify_parameters(map))
		return (false);
	if (!map_count_and_alloc(map, file_data))
		return (false);
	map_fill_grid(map, file_data);
	return (map_verify_grid(map));
}
