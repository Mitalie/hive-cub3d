#ifndef CAST_H
# define CAST_H

# include "cub3d.h"
# include "vec2.h"

typedef enum e_side
{
	HIT_NORTH,
	HIT_SOUTH,
	HIT_WEST,
	HIT_EAST,
}	t_side;

typedef enum e_material
{
	MAT_WALL_NORTH,
	MAT_WALL_SOUTH,
	MAT_WALL_WEST,
	MAT_WALL_EAST,
	MAT_DOOR_FACE,
	MAT_DOOR_SIDE,
	MAT_STATION_INACTIVE,
	MAT_STATION_ACTIVE,
	MAT_STATION_COMPLETED,
	MAT_STATION_BACK,
}	t_material;

typedef struct s_hit
{
	float		distance;
	float		position_in_tile;
	t_side		side;
	t_material	material;
}	t_hit;

void	cast(t_cub3d *cub3d, t_vec2 pos, t_vec2 dir, t_hit *out);

#endif
