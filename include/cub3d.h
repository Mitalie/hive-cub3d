#ifndef CUB3D_H
# define CUB3D_H

# include <stdbool.h>
# include "MLX42/MLX42.h"

# include "enemy.h"
# include "map.h"
# include "minimap.h"
# include "vec2.h"

typedef struct s_cub3d
{
	mlx_t					*mlx;
	mlx_image_t				*render;
	mlx_texture_t			*target_completed_tex;
	float					frame_timestamp;
	t_map					map;
	t_minimap				minimap;
	int						width;
	int						height;
	float					hfov_deg;
	t_vec2					player;
	float					player_facing;
	t_enemy					enemy;
	t_enemy_pathing_state	enemy_pathing;
	bool					cursor_ready;
}	t_cub3d;

#endif
