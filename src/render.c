#include "render.h"

#include <math.h>
#include <stdint.h>
#include "MLX42/MLX42.h"

#include "cast.h"
#include "vec2.h"

static uint32_t	wall_color(t_cub3d *cub3d, t_material material,
	float xpos, float ypos)
{
	mlx_texture_t	*tex;
	uint32_t		x;
	uint32_t		y;
	uint8_t			*texel;

	if (material == MAT_WALL_NORTH)
		tex = cub3d->map.wall_north;
	else if (material == MAT_WALL_SOUTH)
		tex = cub3d->map.wall_south;
	else if (material == MAT_WALL_WEST)
		tex = cub3d->map.wall_west;
	else if (material == MAT_WALL_EAST)
		tex = cub3d->map.wall_east;
	else if (material == MAT_DOOR_FACE)
		tex = cub3d->map.door_face;
	else if (material == MAT_DOOR_SIDE)
		tex = cub3d->map.door_side;
	else
		return (0xff);
	x = fmaxf(0.0f, fminf(tex->width - 1, tex->width * xpos));
	y = fmaxf(0.0f, fminf(tex->height - 1, tex->height * (1 - ypos)));
	texel = &tex->pixels[tex->bytes_per_pixel * (y * tex->width + x)];
	return (texel[0] << 24 | texel[1] << 16 | texel[2] << 8 | 0xff);
}

uint32_t	sprite_color(t_cub3d *cub3d, t_material material,
	float xpos, float ypos)
{
	mlx_texture_t	*tex;
	uint32_t		x;
	uint32_t		y;
	uint8_t			*texel;

	if (material == MAT_STATION_INACTIVE)
		tex = cub3d->map.station_inactive;
	else if (material == MAT_STATION_ACTIVE)
		tex = cub3d->map.station_active;
	else if (material == MAT_STATION_BACK)
		tex = cub3d->map.station_back;
	else if (material == MAT_STATION_COMPLETED)
		tex = cub3d->target_completed_tex;
	else if (material == MAT_ENEMY)
		tex = cub3d->map.enemy_texture;
	else
		return (0xff);
	x = fmaxf(0.0f, fminf(tex->width - 1, tex->width * xpos));
	y = fmaxf(0.0f, fminf(tex->height - 1, tex->height * (1 - ypos)));
	texel = &tex->pixels[tex->bytes_per_pixel * (y * tex->width + x)];
	return (texel[0] << 24 | texel[1] << 16 | texel[2] << 8 | texel[3]);
}

/*
	ypos is first set based on the sprite distance so that 0.0 .. 1.0 matches
	floor and ceiling, then adjusted so that 0.0 .. 1.0 matches the top and
	the bottom of the particular sprite. For station sprites we just move the
	whole image down by 20%, for enemy sprites we account for animated size and
	position.
*/
static bool	render_sprite_pixel(t_cub3d *cub3d, int col,
	int row, t_cast_result *cr, float tan_vert)
{
	size_t		i;
	t_hit		*hit;
	uint32_t	color;
	float		ypos;

	i = 0;
	while (i < cr->num_transparent)
	{
		hit = &cr->transparent[i++];
		ypos = tan_vert * hit->distance + 0.5f;
		if (hit->material == MAT_ENEMY)
			ypos = (ypos - cub3d->enemy.anim_y_pos) / cub3d->enemy.anim_height;
		else
			ypos += 0.2f;
		if (ypos < 0.0f || ypos > 1.0f)
			return (false);
		color = sprite_color(cub3d, hit->material, hit->position_in_tile, ypos);
		if ((color & 0xff) == 0xff)
		{
			mlx_put_pixel(cub3d->render, col, row, color);
			return (true);
		}
	}
	return (false);
}

static void	render_column(t_cub3d *cub3d, float tan_vfov, int col,
	t_cast_result *cr)
{
	int			row;
	float		tan_vert;
	float		tan_wall_height;
	uint32_t	color;

	tan_wall_height = 0.5f / cr->opaque.distance;
	row = 0;
	while (row < cub3d->height)
	{
		tan_vert = -1 * tan_vfov * ((row + 0.5f) / cub3d->height - 0.5f);
		if (render_sprite_pixel(cub3d, col, row, cr, tan_vert))
		{
			row++;
			continue ;
		}
		if (tan_vert > -tan_wall_height && tan_vert < tan_wall_height)
			color = wall_color(cub3d, cr->opaque.material, cr->opaque.position_in_tile,
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
