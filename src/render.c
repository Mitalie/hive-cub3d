#include "render.h"

#include <math.h>
#include <stdint.h>
#include "MLX42/MLX42.h"

#include "cast.h"
#include "vec2.h"

static uint32_t	wall_color(t_cub3d *cub3d, t_side side, float xpos, float ypos)
{
	mlx_texture_t	*tex;
	uint32_t		x;
	uint32_t		y;
	uint8_t			*texel;

	xpos = xpos - floorf(xpos);
	if (side == HIT_NORTH)
		tex = cub3d->map.wall_north;
	else if (side == HIT_SOUTH)
		tex = cub3d->map.wall_south;
	else if (side == HIT_WEST)
		tex = cub3d->map.wall_west;
	else if (side == HIT_EAST)
		tex = cub3d->map.wall_east;
	else
		return (0xff);
	x = tex->width * xpos;
	y = tex->height - tex->height * ypos;
	texel = &tex->pixels[tex->bytes_per_pixel * (y * tex->width + x)];
	return (texel[0] << 24 | texel[1] << 16 | texel[2] << 8 | 0xff);
}

static void	render_column(t_cub3d *cub3d, float tan_vfov, int col, t_hit *hit)
{
	int			row;
	float		tan_vert;
	float		tan_wall_height;
	uint32_t	color;

	tan_wall_height = 0.5f / hit->distance;
	row = 0;
	while (row < cub3d->height)
	{
		tan_vert = -1 * tan_vfov * ((row + 0.5f) / cub3d->height - 0.5f);
		if (tan_vert > -tan_wall_height && tan_vert < tan_wall_height)
			color = wall_color(cub3d, hit->side, hit->position_in_tile,
					tan_vert / tan_wall_height * 0.5f + 0.5f);
		else if (tan_vert > 0)
			color = cub3d->map.color_ceil;
		else
			color = cub3d->map.color_floor;
		mlx_put_pixel(cub3d->render, col, row, color);
		row++;
	}
}

#define DEG_TO_RAD 0.017453292519943295769f

void	render_view(t_cub3d *cub3d)
{
	float	tan_hfov;
	float	tan_vfov;
	t_vec2	direction;
	int		col;
	t_hit	hit;

	tan_hfov = 2 * tanf(cub3d->hfov_deg * 0.5f * DEG_TO_RAD);
	tan_vfov = tan_hfov * cub3d->height / cub3d->width;
	col = 0;
	while (col < cub3d->width)
	{
		direction.y = -1;
		direction.x = tan_hfov * ((col + 0.5f) / cub3d->width - 0.5f);
		direction = vec2_rotate(direction, cub3d->player_facing);
		cast(cub3d, cub3d->player, direction, &hit);
		render_column(cub3d, tan_vfov, col, &hit);
		col++;
	}
}
