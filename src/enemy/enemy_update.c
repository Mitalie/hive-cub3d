#include "enemy.h"
#include "enemy_internal.h"

#include "cub3d.h"

void	enemy_update(t_cub3d *cub3d)
{
	float	anim_time;
	float	move;

	while (cub3d->frame_timestamp - cub3d->enemy.anim_start_time > 2)
	{
		cub3d->enemy.anim_start_pos.x = 68.5f;
		cub3d->enemy.anim_start_pos.y = 4.5f;
		cub3d->enemy.anim_end_pos.x = 69.5f;
		cub3d->enemy.anim_end_pos.y = 4.5f;
		cub3d->enemy.anim_start_time += 2;
	}
	anim_time = cub3d->frame_timestamp - cub3d->enemy.anim_start_time;
	move = enemy_anim_move_x(anim_time);
	cub3d->enemy.pos.x = (1 - move) * cub3d->enemy.anim_start_pos.x
		+ move * cub3d->enemy.anim_end_pos.x;
	cub3d->enemy.pos.y = (1 - move) * cub3d->enemy.anim_start_pos.y
		+ move * cub3d->enemy.anim_end_pos.y;
}
