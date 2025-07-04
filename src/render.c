#include "render.h"

#include <math.h>
#include <stdint.h>
#include "MLX42/MLX42.h"

#include "cast.h"
#include "color.h"
#include "vec2.h"

static uint32_t	wall_color(t_side side, float pos, float ypos)
{
	uint32_t	base;

	pos = pos - floorf(pos);
	if (side == HIT_NORTH)
		base = 0xffff00ff;
	else if (side == HIT_SOUTH)
		base = 0xc03f3fff;
	else if (side == HIT_WEST)
		base = 0xc06000ff;
	else if (side == HIT_EAST)
		base = 0xc000ffff;
	else
		return (0xff);
	return (color_interp(color_interp(0xff, 0xffffffff, ypos), base, pos));
}

static void	render_column(t_cub3d *cub3d, float tan_vfov, int col, t_hit *hit)
{
	int			row;
	float		tan_vert;
	float		tan_wall_height;
	uint32_t	color;

	tan_wall_height = 0.5 / hit->distance;
	row = 0;
	while (row < cub3d->height)
	{
		tan_vert = -1 * tan_vfov * ((row + 0.5) / cub3d->height - 0.5);
		if (tan_vert > -tan_wall_height && tan_vert < tan_wall_height)
			color = wall_color(hit->side, hit->position_in_tile,
					tan_vert / tan_wall_height * 0.5 + 0.5);
		else if (tan_vert > 0)
			color = 0xffffff;
		else
			color = 0x00c060ff;
		mlx_put_pixel(cub3d->render, col, row, color);
		row++;
	}
}

void	render_view(t_cub3d *cub3d)
{
	float	tan_hfov;
	float	tan_vfov;
	t_vec2	direction;
	int		col;
	t_hit	hit;

	tan_hfov = 2 * tan(cub3d->hfov_deg / 2 * 3.1415926535 / 180);
	tan_vfov = tan_hfov * cub3d->height / cub3d->width;
	col = 0;
	while (col < cub3d->width)
	{
		direction.y = -1;
		direction.x = tan_hfov * ((col + 0.5) / cub3d->width - 0.5);
		direction = vec2_rotate(direction, cub3d->player_facing);
		cast(cub3d, cub3d->player, direction, &hit);
		render_column(cub3d, tan_vfov, col, &hit);
		col++;
	}
}
