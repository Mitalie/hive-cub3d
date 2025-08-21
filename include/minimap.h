#ifndef MINIMAP_H
# define MINIMAP_H

# include <stdbool.h>
# include <stdint.h>
# include "MLX42/MLX42.h"

typedef struct s_cub3d	t_cub3d;
typedef struct s_minimap
{
	mlx_image_t	*bg;
	mlx_image_t	*fg;
	uint32_t	size;
}	t_minimap;

bool	minimap_setup(t_cub3d *cub3d);
void	minimap_cleanup(t_cub3d *cub3d);

void	minimap_render_bg(t_cub3d *cub3d);
void	minimap_render_fg(t_cub3d *cub3d);

#endif
