#ifndef CAST_INTERNAL_H
# define CAST_INTERNAL_H

# include "vec2.h"

typedef struct s_cast_state
{
	int		grid_x;
	int		grid_y;
	t_vec2	dir;
	float	distance_to_grid_x;
	float	distance_to_grid_y;
}	t_cast_state;

#endif
