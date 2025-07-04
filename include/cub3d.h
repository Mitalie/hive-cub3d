#ifndef CUB3D_H
# define CUB3D_H

# include "MLX42/MLX42.h"

# include "vec2.h"

typedef struct s_cub3d
{
	mlx_t		*mlx;
	mlx_image_t	*render;
	const char	*map_array;
	int			map_height;
	int			map_width;
	int			width;
	int			height;
	float		hfov_deg;
	t_vec2		player;
	float		player_facing;
}	t_cub3d;

#endif
