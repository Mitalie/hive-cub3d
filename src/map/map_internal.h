#ifndef MAP_INTERNAL_H
# define MAP_INTERNAL_H

# include <stdbool.h>

# include "map.h"

bool	map_parse(t_map *map, char *file_data);

bool	map_parse_parameter(t_map *map, char **file_data);
bool	map_verify_parameters(t_map *map);

bool	map_parse_grid(t_map *map, char *file_data);
bool	map_verify_grid(t_map *map);

#endif
