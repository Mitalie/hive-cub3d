#include "material.h"

#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include "MLX42/MLX42.h"

#include "color.h"
#include "cub3d.h"
#include "map.h"

static mlx_texture_t	*material_to_texture(t_cub3d *cub3d,
	t_material material)
{
	if (material == MAT_WALL_NORTH)
		return (cub3d->map.textures[TEX_WALL_NORTH]);
	else if (material == MAT_WALL_SOUTH)
		return (cub3d->map.textures[TEX_WALL_SOUTH]);
	else if (material == MAT_WALL_WEST)
		return (cub3d->map.textures[TEX_WALL_WEST]);
	else if (material == MAT_WALL_EAST)
		return (cub3d->map.textures[TEX_WALL_EAST]);
	else if (material == MAT_DOOR_FACE)
		return (cub3d->map.textures[TEX_DOOR_FACE]);
	else if (material == MAT_DOOR_SIDE)
		return (cub3d->map.textures[TEX_DOOR_SIDE]);
	else if (material == MAT_TGT_INACTIVE)
		return (cub3d->map.textures[TEX_TGT_INACTIVE]);
	else if (material == MAT_TGT_ACTIVE)
		return (cub3d->map.textures[TEX_TGT_ACTIVE]);
	else if (material == MAT_TGT_BACK)
		return (cub3d->map.textures[TEX_TGT_BACK]);
	else if (material == MAT_TGT_COMPLETED)
		return (cub3d->target_completed_tex);
	else if (material == MAT_ENEMY)
		return (cub3d->map.textures[TEX_ENEMY]);
	return (NULL);
}

uint32_t	material_tex(t_cub3d *cub3d, t_material material,
	float xpos, float ypos)
{
	mlx_texture_t	*tex;
	uint32_t		x;
	uint32_t		y;
	uint8_t			*texel;

	tex = material_to_texture(cub3d, material);
	if (!tex)
		return (0xff);
	x = fmaxf(0.0f, fminf(tex->width - 1, tex->width * xpos));
	y = fmaxf(0.0f, fminf(tex->height - 1, tex->height * (1 - ypos)));
	texel = &tex->pixels[tex->bytes_per_pixel * (y * tex->width + x)];
	return (color_create(texel[0], texel[1], texel[2], texel[3]));
}
