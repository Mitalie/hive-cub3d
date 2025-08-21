/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_internal.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:21:12 by amakinen          #+#    #+#             */
/*   Updated: 2025/08/21 15:21:13 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_INTERNAL_H
# define MAP_INTERNAL_H

# include <stdbool.h>
# include <stdint.h>

# include "map.h"

typedef struct s_map_tex_param
{
	const char		*name;
	t_map_feature	feature;
	t_map_texture	idx;
}	t_map_tex_param;

bool	map_parse(t_map *map, char *file_data);

bool	map_parse_parameter(t_map *map, char **file_data);
bool	map_verify_parameters(t_map *map);

bool	map_parse_grid(t_map *map, char *file_data);
bool	map_verify_grid(t_map *map);

bool	map_check_grid_character(t_map *map, char c, uint32_t col);

#endif
