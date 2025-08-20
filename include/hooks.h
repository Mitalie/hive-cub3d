#ifndef HOOKS_H
# define HOOKS_H

# include "MLX42/MLX42.h"

void	loop_hook(void *param);
void	key_hook(mlx_key_data_t data, void *param);
void	cursor_hook(double x, double y, void *param);

#endif
