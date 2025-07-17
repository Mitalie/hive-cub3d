#ifndef MAP_H
# define MAP_H

# include <stdbool.h>
# include <stdint.h>
# include "MLX42/MLX42.h"

typedef struct s_map
{
	uint32_t		width;
	uint32_t		height;
	char			*grid;
	mlx_texture_t	*wall_north;
	mlx_texture_t	*wall_south;
	mlx_texture_t	*wall_east;
	mlx_texture_t	*wall_west;
	uint32_t		color_floor;
	uint32_t		color_ceil;
	uint32_t		player_x;
	uint32_t		player_y;
	char			player_start;
}	t_map;

bool	map_load(t_map *map, const char *path);
void	map_unload(t_map *map);

bool	map_tile_is_wall(t_map *map, int x, int y);

#endif
