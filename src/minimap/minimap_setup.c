#include "minimap.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "MLX42/MLX42.h"

#include "cub3d.h"

bool	minimap_setup(t_cub3d *cub3d)
{
	uint32_t	size;
	uint32_t	x;
	uint32_t	y;

	size = 0.3f * fminf(cub3d->width, cub3d->height);
	if (cub3d->minimap.bg && cub3d->minimap.fg
		&& size == cub3d->minimap.size)
		return (true);
	minimap_cleanup(cub3d);
	x = cub3d->width - size;
	y = 0;
	cub3d->minimap.size = size;
	cub3d->minimap.bg = mlx_new_image(cub3d->mlx, size, size);
	cub3d->minimap.fg = mlx_new_image(cub3d->mlx, size, size);
	if (!cub3d->minimap.bg || !cub3d->minimap.fg)
		return (false);
	if (mlx_image_to_window(cub3d->mlx, cub3d->minimap.bg, x, y) < 0
		|| mlx_image_to_window(cub3d->mlx, cub3d->minimap.fg, x, y) < 0)
		return (false);
	minimap_render_bg(cub3d);
	return (true);
}

void	minimap_cleanup(t_cub3d *cub3d)
{
	if (cub3d->minimap.bg)
		mlx_delete_image(cub3d->mlx, cub3d->minimap.bg);
	if (cub3d->minimap.fg)
		mlx_delete_image(cub3d->mlx, cub3d->minimap.fg);
}
