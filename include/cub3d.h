#ifndef CUB3D_H
# define CUB3D_H

# include "MLX42/MLX42.h"

# include "vec2.h"

typedef struct s_cub3d
{
	mlx_t		*mlx;
	mlx_image_t	*render;
	int			width;
	int			height;
	t_vec2		player;
	float		player_facing;
}	t_cub3d;

#endif
