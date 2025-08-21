#include "render.h"

#include <stdbool.h>
#include "MLX42/MLX42.h"

#include "cub3d.h"

bool	render_setup(t_cub3d *cub3d)
{
	if (cub3d->render
		&& cub3d->width == cub3d->mlx->width
		&& cub3d->height == cub3d->mlx->height)
		return (true);
	if (cub3d->render)
		mlx_delete_image(cub3d->mlx, cub3d->render);
	cub3d->width = cub3d->mlx->width;
	cub3d->height = cub3d->mlx->height;
	cub3d->render = mlx_new_image(cub3d->mlx, cub3d->width, cub3d->height);
	if (!cub3d->render)
		return (false);
	if (mlx_image_to_window(cub3d->mlx, cub3d->render, 0, 0) < 0)
		return (false);
	return (true);
}

void	render_cleanup(t_cub3d *cub3d)
{
	if (cub3d->render)
		mlx_delete_image(cub3d->mlx, cub3d->render);
}
