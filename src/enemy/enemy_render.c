#include "enemy.h"
#include "enemy_internal.h"

#include <math.h>
#include <stdint.h>
#include "MLX42/MLX42.h"

#include "cub3d.h"

#include "cast.h"
uint32_t	sprite_color(t_cub3d *cub3d, t_material material,
	float xpos, float ypos);

void	enemy_render_col(t_cub3d *cub3d, float tan_vfov, int col, float xpos, float anim_time, float distance)
{
	float		tan_top;
	float		tan_bottom;
	int			row_top;
	int			row_bottom;
	int			row;
	uint32_t	color;

	tan_top = -(enemy_anim_top_y(anim_time) - 0.5f) / distance;
	tan_bottom = -(enemy_anim_bottom_y(anim_time) - 0.5f) / distance;
	row_top = floor((tan_top / tan_vfov + 0.5f) * cub3d->height);
	row_bottom = floor((tan_bottom / tan_vfov + 0.5f) * cub3d->height);
	row = 0;
	if (row_top > 0)
		row = row_top;
	while (row < row_bottom && row < cub3d->height)
	{
		color = sprite_color(cub3d, MAT_ENEMY, xpos,
				(float)(row - row_bottom) / (row_top - row_bottom));
		if ((color & 0xff) == 0xff)
			mlx_put_pixel(cub3d->render, col, row, color);
		row++;
	}
}

#define DEG_TO_RAD 0.017453292519943295769f

/*
	tan_col = tan_hfov * ((col + 0.5f) / cub3d->width - 0.5f);
	tan_col / tan_hfov = (col + 0.5f) / cub3d->width - 0.5f
	tan_col / tan_hfov + 0.5f = (col + 0.5f) / cub3d->width
	(tan_col / tan_hfov + 0.5f) * cub3d->width = col + 0.5f
	col = (tan_col / tan_hfov + 0.5f) * cub3d->width - 0.5f
	col = floor((tan_col / tan_hfov + 0.5f) * cub3d->width)
*/

void	enemy_render(t_cub3d *cub3d)
{
	t_vec2	player_to_enemy;
	float	distance;
	float	left;
	float	right;
	float	anim_time;
	float	tan_hfov;
	float	tan_vfov;
	float	tan_left;
	float	tan_right;
	int		col_left;
	int		col_right;
	int		col;

	anim_time = cub3d->frame_timestamp - cub3d->enemy.anim_start_time;
	player_to_enemy.x = cub3d->enemy.pos.x - cub3d->player.x;
	player_to_enemy.y = cub3d->enemy.pos.y - cub3d->player.y;
	player_to_enemy = vec2_rotate(player_to_enemy, -cub3d->player_facing);
	distance = -player_to_enemy.y;
	if (distance < 0)
		return ;
	left = player_to_enemy.x - enemy_anim_side_x(anim_time);
	right = player_to_enemy.x + enemy_anim_side_x(anim_time);
	tan_hfov = 2 * tanf(cub3d->hfov_deg * 0.5f * DEG_TO_RAD);
	tan_vfov = tan_hfov * cub3d->height / cub3d->width;
	tan_left = left / distance;
	tan_right = right / distance;
	col_left = floor((tan_left / tan_hfov + 0.5f) * cub3d->width);
	col_right = floor((tan_right / tan_hfov + 0.5f) * cub3d->width);
	col = 0;
	if (col_left > 0)
		col = col_left;
	while (col < col_right && col < cub3d->width)
	{
		enemy_render_col(cub3d, tan_vfov, col, (float)(col - col_left) / (col_right - col_left), anim_time, distance);
		col++;
	}
}
