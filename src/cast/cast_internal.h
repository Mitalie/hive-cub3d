#ifndef CAST_INTERNAL_H
# define CAST_INTERNAL_H

# include "cast.h"

# include <stdbool.h>

# include "cub3d.h"
# include "vec2.h"

typedef struct s_intersection
{
	float	distance_along_ray;
	float	position_on_target;
}	t_intersection;

t_intersection	intersect_x(t_vec2 ray_start, t_vec2 ray_dir, float target_y);
t_intersection	intersect_y(t_vec2 ray_start, t_vec2 ray_dir, float target_x);

typedef struct s_cast_state
{
	int				tile_x;
	int				tile_y;
	t_vec2			dir;
	float			grid_line_x;
	float			grid_line_y;
	t_intersection	intersection_x;
	t_intersection	intersection_y;
}	t_cast_state;

bool			cast_wall(t_cub3d *cub3d, t_cast_state *state, t_hit *hit,
					t_intersection intersection);
bool			cast_door(t_cub3d *cub3d, t_cast_state *state, t_hit *hit,
					t_intersection intersection);
bool			cast_station(t_cub3d *cub3d, t_cast_state *state, t_hit *hit,
					t_intersection intersection);

#endif
