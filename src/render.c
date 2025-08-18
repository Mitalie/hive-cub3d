#include "render.h"

#include <math.h>
#include <stdint.h>
#include "MLX42/MLX42.h"

#include "cast.h"
#include "material.h"
#include "vec2.h"

/*
	ypos is first set based on the sprite distance so that 0.0 .. 1.0 matches
	floor and ceiling, then adjusted so that 0.0 .. 1.0 matches the top and
	the bottom of the particular sprite. For station sprites we just move the
	whole image down by 20%, for enemy sprites we account for animated size and
	position.
*/
static bool	render_hit_pixel(t_cub3d *cub3d, t_vec2i pixel,
	t_hit *hit, float tan_vert)
{
	uint32_t	color;
	float		ypos;

	ypos = tan_vert * hit->distance + 0.5f;
	if (hit->material == MAT_ENEMY)
		ypos = (ypos - cub3d->enemy.anim_y_pos) / cub3d->enemy.anim_height;
	else if (hit->material == MAT_STATION_INACTIVE
		|| hit->material == MAT_STATION_ACTIVE
		|| hit->material == MAT_STATION_COMPLETED
		|| hit->material == MAT_STATION_BACK)
		ypos += 0.2f;
	if (ypos < 0.0f || ypos > 1.0f)
		return (false);
	color = material_tex(cub3d, hit->material, hit->position_in_tile, ypos);
	if ((color & 0xff) == 0xff)
	{
		mlx_put_pixel(cub3d->render, pixel.x, pixel.y, color);
		return (true);
	}
	return (false);
}

static bool	render_sprite_pixel(t_cub3d *cub3d, t_vec2i pixel,
	t_cast_result *cr, float tan_vert)
{
	size_t		i;
	t_hit		*hit;

	i = 0;
	while (i < cr->num_transparent)
	{
		hit = &cr->transparent[i++];
		if (render_hit_pixel(cub3d, pixel, hit, tan_vert))
			return (true);
	}
	return (false);
}

static void	render_bg_pixel(t_cub3d *cub3d, t_vec2i pixel,
	t_cast_result *cr, float tan_vert)
{
	uint32_t	color;

	if (render_hit_pixel(cub3d, pixel, &cr->opaque, tan_vert))
		return ;
	if (tan_vert > 0)
		color = cub3d->map.color_ceil;
	else
		color = cub3d->map.color_floor;
	mlx_put_pixel(cub3d->render, pixel.x, pixel.y, color);
}

static void	render_column(t_cub3d *cub3d, float tan_vfov, int col,
	t_cast_result *cr)
{
	t_vec2i		pixel;
	float		tan_vert;

	pixel.x = col;
	pixel.y = -1;
	while (++pixel.y < cub3d->height)
	{
		tan_vert = -1 * tan_vfov * ((pixel.y + 0.5f) / cub3d->height - 0.5f);
		if (render_sprite_pixel(cub3d, pixel, cr, tan_vert))
			continue ;
		render_bg_pixel(cub3d, pixel, cr, tan_vert);
	}
}

#define DEG_TO_RAD 0.017453292519943295769f

void	render_view(t_cub3d *cub3d)
{
	float			tan_hfov;
	float			tan_vfov;
	t_vec2			direction;
	int				col;
	t_cast_result	cast_result;

	tan_hfov = 2 * tanf(cub3d->hfov_deg * 0.5f * DEG_TO_RAD);
	tan_vfov = tan_hfov * cub3d->height / cub3d->width;
	col = 0;
	while (col < cub3d->width)
	{
		direction.y = -1;
		direction.x = tan_hfov * ((col + 0.5f) / cub3d->width - 0.5f);
		direction = vec2_rotate(direction, cub3d->player_facing);
		cast_result.num_transparent = 0;
		cast(cub3d, cub3d->player, direction, &cast_result);
		render_column(cub3d, tan_vfov, col, &cast_result);
		col++;
	}
}
