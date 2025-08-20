#include "map_internal.h"
#include "map.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "MLX42/MLX42.h"

#include "color.h"
#include "util.h"

static bool	map_parse_wall(mlx_texture_t **wall_out, char **file_data)
{
	char	*wallfile_start;
	char	wallfile_end_tmp;

	*file_data += 2;
	if (**file_data != ' ' && **file_data != '\t')
		return (util_err_false("Map error", "unrecognized parameter"));
	while (**file_data == ' ' || **file_data == '\t')
		(*file_data)++;
	wallfile_start = *file_data;
	while (**file_data != ' ' && **file_data != '\t'
		&& **file_data != '\n' && **file_data != '\0')
		(*file_data)++;
	wallfile_end_tmp = **file_data;
	**file_data = '\0';
	*wall_out = mlx_load_png(wallfile_start);
	**file_data = wallfile_end_tmp;
	if (*wall_out == NULL)
		return (util_err_false("Map error", "failed to load texture"));
	return (true);
}

static bool	map_parse_color(uint32_t *color_out, char **file_data)
{
	uint8_t	red;
	uint8_t	green;
	uint8_t	blue;

	(*file_data)++;
	if (**file_data != ' ' && **file_data != '\t')
		return (util_err_false("Map error", "unrecognized parameter"));
	while (**file_data == ' ' || **file_data == '\t')
		(*file_data)++;
	if (!util_parse_uint8(file_data, &red))
		return (util_err_false("Map error", "invalid color value"));
	if (*(*file_data)++ != ',')
		return (util_err_false("Map error", "invalid color value"));
	while (**file_data == ' ' || **file_data == '\t')
		(*file_data)++;
	if (!util_parse_uint8(file_data, &green))
		return (util_err_false("Map error", "invalid color value"));
	if (*(*file_data)++ != ',')
		return (util_err_false("Map error", "invalid color value"));
	while (**file_data == ' ' || **file_data == '\t')
		(*file_data)++;
	if (!util_parse_uint8(file_data, &blue))
		return (util_err_false("Map error", "invalid color value"));
	*color_out = color_create(red, green, blue, 0xff);
	return (true);
}

static const t_map_tex_param	g_map_tex_params[NUM_MAP_TEXTURES] = {
{"NO", TEX_WALL_NORTH},
{"SO", TEX_WALL_SOUTH},
{"EA", TEX_WALL_EAST},
{"WE", TEX_WALL_WEST},
{"DF", TEX_DOOR_FACE},
{"DS", TEX_DOOR_SIDE},
{"SI", TEX_TGT_INACTIVE},
{"SA", TEX_TGT_ACTIVE},
{"SB", TEX_TGT_BACK},
{"S1", TEX_TGT_COMPLETED1},
{"S2", TEX_TGT_COMPLETED2},
{"S3", TEX_TGT_COMPLETED3},
{"S4", TEX_TGT_COMPLETED4},
{"EN", TEX_ENEMY},
};

bool	map_parse_parameter(t_map *map, char **file_data)
{
	size_t	i;

	i = -1;
	while (++i < NUM_MAP_TEXTURES)
		if (util_memcmp(*file_data, g_map_tex_params[i].name, 2) == 0)
			return (map_parse_wall(
					&map->textures[g_map_tex_params[i].idx], file_data));
	if (**file_data == 'C')
		return (map_parse_color(&map->color_ceil, file_data));
	else if (**file_data == 'F')
		return (map_parse_color(&map->color_floor, file_data));
	else
		return (util_err_false("Map error", "unrecognized parameter"));
}

bool	map_verify_parameters(t_map *map)
{
	size_t	i;

	i = -1;
	while (++i < NUM_MAP_TEXTURES)
		if (map->textures[i] == NULL)
			return (util_err_false("Map error",
					"missing one or more parameters"));
	if (map->color_ceil == 0
		|| map->color_floor == 0)
		return (util_err_false("Map error", "missing one or more parameters"));
	return (true);
}
