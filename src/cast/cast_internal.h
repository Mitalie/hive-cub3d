#ifndef CAST_INTERNAL_H
# define CAST_INTERNAL_H

# include "cast.h"

# include <stdbool.h>

# include "cub3d.h"
# include "vec2.h"

typedef struct s_intersection
{
	float	ray_len;
	float	tgt_pos;
}	t_intersection;

t_intersection	intersect_x(t_vec2 ray_start, t_vec2 ray_dir, float target_y);
t_intersection	intersect_y(t_vec2 ray_start, t_vec2 ray_dir, float target_x);
t_intersection	intersect(t_vec2 ray_start, t_vec2 ray_dir,
					t_vec2 target_start, t_vec2 target_dir);

typedef struct s_cast_state
{
	t_cub3d			*cub3d;
	t_cast_result	*cr;
	int				tile_x;
	int				tile_y;
	t_vec2			pos;
	t_vec2			dir;
	float			grid_line_x;
	float			grid_line_y;
	t_intersection	int_x;
	t_intersection	int_y;
}	t_cast_state;

bool			cast_wall(t_cast_state *state, t_intersection intersection);
bool			cast_door(t_cast_state *state, t_intersection intersection);
bool			cast_station(t_cast_state *state, t_map_tile tile);
void			cast_enemy(t_cast_state *state);

#endif
