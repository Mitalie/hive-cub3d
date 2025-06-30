#ifndef INPUT_H
# define INPUT_H

# include "MLX42/MLX42.h"

# include "cub3d.h"

void	input_timed(t_cub3d *cub3d);
void	input_key(t_cub3d *cub3d, mlx_key_data_t key_data);

#endif
