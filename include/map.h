#ifndef MAP_H
# define MAP_H

# include <stdbool.h>
# include <stdint.h>
# include "MLX42/MLX42.h"

# define MAX_DOORS 16
# define MAX_TARGETS 10
/*
	This is not absolutely necessary, but we do a sanity check and reject maps
	with ridiculous number of stations.
*/
# define MAX_STATIONS 512

typedef struct s_door
{
	uint32_t	x;
	uint32_t	y;
	float		anim_start_time;
}	t_door;

typedef struct s_target
{
	uint32_t	x;
	uint32_t	y;
	bool		completed;
}	t_target;

typedef struct s_map
{
	uint32_t		width;
	uint32_t		height;
	char			*grid;
	mlx_texture_t	*wall_north;
	mlx_texture_t	*wall_south;
	mlx_texture_t	*wall_east;
	mlx_texture_t	*wall_west;
	mlx_texture_t	*door_face;
	mlx_texture_t	*door_side;
	uint32_t		color_floor;
	uint32_t		color_ceil;
	uint32_t		player_x;
	uint32_t		player_y;
	char			player_start;
	uint32_t		num_doors;
	t_door			doors[MAX_DOORS];
	uint32_t		num_targets;
	t_target		targets[MAX_TARGETS];
}	t_map;

bool		map_load(t_map *map, const char *path);
void		map_unload(t_map *map);

typedef enum e_map_tile
{
	TILE_EMPTY,
	TILE_WALL,
	TILE_DOOR_NS,
	TILE_DOOR_EW,
	TILE_STATION_N,
	TILE_STATION_S,
}	t_map_tile;

t_map_tile	map_tile(t_map *map, int x, int y);
t_door		*map_door(t_map *map, int x, int y);
t_target	*map_target(t_map *map, int x, int y);

#endif
