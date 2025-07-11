#include "map_internal.h"
#include "map.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "MLX42/MLX42.h"

#include "util.h"

static bool	map_parse_wall(mlx_texture_t **wall_out, char **file_data)
{
	char	*wallfile_start;

	*file_data += 2;
	if (**file_data != ' ' && **file_data != '\t')
		return (false);
	while (**file_data == ' ' || **file_data == '\t')
		(*file_data)++;
	wallfile_start = *file_data;
	while (**file_data != ' ' && **file_data != '\t'
		&& **file_data != '\n' && **file_data != '\0')
		(*file_data)++;
	**file_data = '\0';
	(*file_data)++;
	*wall_out = mlx_load_png(wallfile_start);
	if (*wall_out == NULL)
		return (false);
	return (true);
}

static bool	map_parse_color(uint32_t *color_out, char **file_data)
{
	uint8_t	red;
	uint8_t	green;
	uint8_t	blue;

	(*file_data)++;
	if (**file_data != ' ' && **file_data != '\t')
		return (false);
	while (**file_data == ' ' || **file_data == '\t')
		(*file_data)++;
	if (!util_parse_uint8(file_data, &red))
		return (false);
	if (*(*file_data)++ != ',')
		return (false);
	while (**file_data == ' ' || **file_data == '\t')
		(*file_data)++;
	if (!util_parse_uint8(file_data, &green))
		return (false);
	if (*(*file_data)++ != ',')
		return (false);
	while (**file_data == ' ' || **file_data == '\t')
		(*file_data)++;
	if (!util_parse_uint8(file_data, &blue))
		return (false);
	*color_out = red << 24 | green << 16 | blue << 8 | 0xff;
	return (true);
}

bool	map_parse_parameter(t_map *map, char **file_data)
{
	if (util_memcmp(*file_data, "NO", 2))
		return (map_parse_wall(&map->wall_north, file_data));
	else if (util_memcmp(*file_data, "SO", 2))
		return (map_parse_wall(&map->wall_south, file_data));
	else if (util_memcmp(*file_data, "EA", 2))
		return (map_parse_wall(&map->wall_east, file_data));
	else if (util_memcmp(*file_data, "WE", 2))
		return (map_parse_wall(&map->wall_west, file_data));
	else if (**file_data == 'C')
		return (map_parse_color(&map->color_ceil, file_data));
	else if (**file_data == 'F')
		return (map_parse_color(&map->color_floor, file_data));
	else
		return (false);
}

/*
	TODO: reject duplicate parameters
*/
bool	map_verify_parameters(t_map *map)
{
	if (map->wall_north == NULL
		|| map->wall_south == NULL
		|| map->wall_east == NULL
		|| map->wall_west == NULL
		|| map->color_ceil == 0
		|| map->color_floor == 0)
		return (false);
	return (true);
}
