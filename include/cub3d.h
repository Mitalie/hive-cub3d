#ifndef CUB3D_H
# define CUB3D_H

# include "MLX42/MLX42.h"

typedef struct s_cub3d
{
	mlx_t		*mlx;
	mlx_image_t	*render;
	int			width;
	int			height;
	float		player_x;
	float		player_y;
	float		player_facing;
}	t_cub3d;

#endif
