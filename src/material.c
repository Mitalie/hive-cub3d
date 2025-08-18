#include <math.h>
#include <stdint.h>
#include "MLX42/MLX42.h"

#include "cast.h"
#include "cub3d.h"

static mlx_texture_t	*material_to_texture(t_cub3d *cub3d,
	t_material material)
{
	if (material == MAT_WALL_NORTH)
		return (cub3d->map.wall_north);
	else if (material == MAT_WALL_SOUTH)
		return (cub3d->map.wall_south);
	else if (material == MAT_WALL_WEST)
		return (cub3d->map.wall_west);
	else if (material == MAT_WALL_EAST)
		return (cub3d->map.wall_east);
	else if (material == MAT_DOOR_FACE)
		return (cub3d->map.door_face);
	else if (material == MAT_DOOR_SIDE)
		return (cub3d->map.door_side);
	else if (material == MAT_STATION_INACTIVE)
		return (cub3d->map.station_inactive);
	else if (material == MAT_STATION_ACTIVE)
		return (cub3d->map.station_active);
	else if (material == MAT_STATION_BACK)
		return (cub3d->map.station_back);
	else if (material == MAT_STATION_COMPLETED)
		return (cub3d->target_completed_tex);
	else if (material == MAT_ENEMY)
		return (cub3d->map.enemy_texture);
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
	return (texel[0] << 24 | texel[1] << 16 | texel[2] << 8 | texel[3]);
}
