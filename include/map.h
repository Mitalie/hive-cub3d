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

typedef enum e_map_texture
{
	TEX_WALL_NORTH,
	TEX_WALL_SOUTH,
	TEX_WALL_EAST,
	TEX_WALL_WEST,
	TEX_DOOR_FACE,
	TEX_DOOR_SIDE,
	TEX_TGT_INACTIVE,
	TEX_TGT_ACTIVE,
	TEX_TGT_BACK,
	TEX_TGT_COMPLETED1,
	TEX_TGT_COMPLETED2,
	TEX_TGT_COMPLETED3,
	TEX_TGT_COMPLETED4,
	TEX_ENEMY,
	NUM_MAP_TEXTURES,
}	t_map_texture;

typedef struct s_map
{
	uint32_t		width;
	uint32_t		height;
	char			*grid;
	mlx_texture_t	*textures[NUM_MAP_TEXTURES];
	uint32_t		color_floor;
	uint32_t		color_ceil;
	uint32_t		player_x;
	uint32_t		player_y;
	char			player_start;
	bool			has_enemy;
	uint32_t		enemy_start_x;
	uint32_t		enemy_start_y;
	uint32_t		num_doors;
	t_door			doors[MAX_DOORS];
	uint32_t		num_targets;
	t_target		targets[MAX_TARGETS];
}	t_map;

typedef enum e_map_tile
{
	TILE_EMPTY,
	TILE_WALL,
	TILE_DOOR_NS,
	TILE_DOOR_EW,
	TILE_STATION_N,
	TILE_STATION_S,
}	t_map_tile;

bool			map_load(t_map *map, const char *path);
void			map_unload(t_map *map);

t_map_tile		map_tile(t_map *map, int x, int y);
float			map_door_state(t_map *map, int x, int y, float frame_timestamp);
t_target		*map_target(t_map *map, int x, int y);
mlx_texture_t	*map_target_completed_tex(t_map *map, float frame_timestamp);

#endif
