#ifndef CUB3D_H
# define CUB3D_H

# include <stdbool.h>
# include "MLX42/MLX42.h"

# include "map.h"
# include "vec2.h"

typedef struct s_cub3d
{
	mlx_t		*mlx;
	mlx_image_t	*render;
	t_map		map;
	int			width;
	int			height;
	float		hfov_deg;
	t_vec2		player;
	float		player_facing;
	bool		cursor_ready;
}	t_cub3d;

#endif
