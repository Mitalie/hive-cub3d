#ifndef CAST_H
# define CAST_H

# include "cub3d.h"
# include "material.h"
# include "vec2.h"

typedef enum e_side
{
	HIT_NORTH,
	HIT_SOUTH,
	HIT_WEST,
	HIT_EAST,
}	t_side;

typedef struct s_hit
{
	float		distance;
	float		pos_in_tile;
	t_side		side;
	t_material	material;
}	t_hit;

# define MAX_TRANSPARENT 8

typedef struct s_cast_result
{
	t_hit	opaque;
	size_t	num_transparent;
	t_hit	transparent[MAX_TRANSPARENT];
}	t_cast_result;

void	cast(t_cub3d *cub3d, t_vec2 pos, t_vec2 dir, t_cast_result *out);

#endif
