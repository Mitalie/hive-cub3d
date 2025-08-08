#include "enemy.h"
#include "enemy_internal.h"

#include "cub3d.h"

void	enemy_update(t_cub3d *cub3d)
{
	float	anim_time;
	float	move;

	while (cub3d->frame_timestamp - cub3d->enemy.anim_start_time > 2)
	{
		if (cub3d->enemy.anim_start_time < 0)
			cub3d->enemy.anim_start_time = 0;
		else
			cub3d->enemy.anim_start_time += 2;
		cub3d->enemy.anim_start_pos.x = cub3d->enemy.pos.x;
		cub3d->enemy.anim_start_pos.y = cub3d->enemy.pos.y;
		enemy_pathing(cub3d);
	}
	anim_time = cub3d->frame_timestamp - cub3d->enemy.anim_start_time;
	move = enemy_anim_move_x(anim_time);
	cub3d->enemy.pos.x = (1 - move) * cub3d->enemy.anim_start_pos.x
		+ move * cub3d->enemy.anim_end_pos.x;
	cub3d->enemy.pos.y = (1 - move) * cub3d->enemy.anim_start_pos.y
		+ move * cub3d->enemy.anim_end_pos.y;
}
