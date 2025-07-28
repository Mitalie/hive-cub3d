#include "minimap.h"
#include "minimap_internal.h"

#include <math.h>
#include <stdint.h>
#include "MLX42/MLX42.h"

#include "cub3d.h"
#include "vec2.h"

#define CIRCLE_RADIUS 0.475f
#define RANGE_BLOCKS 12
#define BLIP_RADIUS 0.02f

/*
	Outside circle:
		x^2 + y^2 > r^2
		x^2 > r^2 - y^2
	Inside cone (remember y grows down):
		y < tan(hfov / 2) * x AND y > -tan(hfov / 2) * x
		x > y / tan(hfov / 2) AND x < -y / tan(hfov / 2)
*/

static void	minimap_render_bg_row(t_cub3d *cub3d, int row,
	float circle_limit, float cone_limit)
{
	uint32_t	col;
	float		x;

	col = -1;
	while (++col < cub3d->minimap.size)
	{
		x = (col - (cub3d->minimap.size - 1) * 0.5f);
		if (x * x > circle_limit)
			mlx_put_pixel(cub3d->minimap.bg, col, row, 0);
		else if (x > cone_limit && x < -cone_limit)
			mlx_put_pixel(cub3d->minimap.bg, col, row, 0x2f2f9066);
		else
			mlx_put_pixel(cub3d->minimap.bg, col, row, 0x2f2f9044);
	}
}

#define DEG_TO_RAD 0.017453292519943295769f

void	minimap_render_bg(t_cub3d *cub3d)
{
	uint32_t	row;
	float		r;
	float		y;
	float		circle_limit;
	float		cone_limit;

	r = CIRCLE_RADIUS * cub3d->minimap.size;
	row = -1;
	while (++row < cub3d->minimap.size)
	{
		y = (row - (cub3d->minimap.size - 1) * 0.5f);
		circle_limit = r * r - y * y;
		cone_limit = y / tanf(cub3d->hfov_deg * DEG_TO_RAD * 0.5f);
		minimap_render_bg_row(cub3d, row, circle_limit, cone_limit);
	}
}

/*
	Calculate position relative to player, rotate according to player facing
	direction, and transform to minimap pixel coordinates.
*/
static void	minimap_transform_targets(
	t_cub3d *cub3d, t_minimap_targets *targets)
{
	uint32_t	i;
	t_vec2		pos;
	float		pixels_per_block;
	float		max_dist_blocks;

	pixels_per_block = CIRCLE_RADIUS * cub3d->minimap.size / RANGE_BLOCKS;
	max_dist_blocks = RANGE_BLOCKS * (1 - BLIP_RADIUS / CIRCLE_RADIUS);
	targets->num = 0;
	i = -1;
	while (++i < cub3d->map.num_targets)
	{
		if (cub3d->map.targets[i].completed)
			continue ;
		pos.x = cub3d->map.targets[i].x + 0.5f - cub3d->player.x;
		pos.y = cub3d->map.targets[i].y + 0.5f - cub3d->player.y;
		if (pos.x * pos.x + pos.y * pos.y > max_dist_blocks * max_dist_blocks)
			continue ;
		pos = vec2_rotate(pos, -cub3d->player_facing);
		pos.x = pos.x * pixels_per_block + (cub3d->minimap.size - 1) * 0.5f;
		pos.y = pos.y * pixels_per_block + (cub3d->minimap.size - 1) * 0.5f;
		targets->pos[targets->num++] = pos;
	}
}

/*
	Blips are brightest in the center and fade near edges with I = 1 - d^2 where
	d is the distance of current pixel from the target, relative to blip size.
	Intensity of overlapping blips is simply added together, capped at 1.
*/
static float	minimap_fg_pixel_alpha(t_minimap_targets *targets,
	uint32_t x, uint32_t y, float blip_radius_pixels_sq)
{
	float		alpha;
	uint32_t	i;
	float		dist_x;
	float		dist_y;
	float		dist_sq;

	alpha = 0;
	i = -1;
	while (++i < targets->num)
	{
		dist_x = x - targets->pos[i].x;
		dist_y = y - targets->pos[i].y;
		dist_sq = dist_x * dist_x + dist_y * dist_y;
		if (dist_sq < blip_radius_pixels_sq)
			alpha += 1 - dist_sq / blip_radius_pixels_sq;
	}
	if (alpha > 1)
		alpha = 1;
	return (alpha);
}

void	minimap_render_fg(t_cub3d *cub3d)
{
	t_minimap_targets	targets;
	uint32_t			y;
	uint32_t			x;
	float				alpha;
	float				blip_radius_pixels_sq;

	blip_radius_pixels_sq = BLIP_RADIUS * cub3d->minimap.size;
	blip_radius_pixels_sq *= blip_radius_pixels_sq;
	minimap_transform_targets(cub3d, &targets);
	y = -1;
	while (++y < cub3d->minimap.size)
	{
		x = -1;
		while (++x < cub3d->minimap.size)
		{
			alpha = minimap_fg_pixel_alpha(&targets, x, y,
					blip_radius_pixels_sq);
			mlx_put_pixel(cub3d->minimap.fg, x, y,
				0xff << 24 | (uint8_t)(alpha * 0xff));
		}
	}
}
