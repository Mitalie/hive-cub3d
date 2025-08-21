#include "enemy.h"
#include "enemy_internal.h"

#include <math.h>

#include "cub3d.h"
#include "map.h"

/*
	Animation was designed with 0.2s pre-jump crouch, total 0.6s of jump,
	flight, and landing, and another 0.2s straightening out of the landing
	crouch. To make the duck a bit faster without making the animation look
	unnaturally sped up, we skip the crouch/straighten and just go straight
	into next jump with 0.6s interval.
*/
#define ANIM_START_OFFSET 0.2f
#define ANIM_DURATION 0.6f

/*
	We expect enemy_pathing to update anim_end_pos to the next tile along the
	path from enemy to player, but initialize to current position in case no
	path exists.
*/
static void	enemy_update_anim(t_cub3d *cub3d)
{
	if (cub3d->enemy.anim_start_time < 0)
		cub3d->enemy.anim_start_time = 0;
	else
		cub3d->enemy.anim_start_time = cub3d->frame_timestamp
			- fmodf(cub3d->frame_timestamp, ANIM_DURATION);
	cub3d->enemy.anim_start_pos.x = cub3d->enemy.pos.x;
	cub3d->enemy.anim_start_pos.y = cub3d->enemy.pos.y;
	cub3d->enemy.anim_end_pos.x = cub3d->enemy.pos.x;
	cub3d->enemy.anim_end_pos.y = cub3d->enemy.pos.y;
	enemy_pathing(cub3d);
}

void	enemy_update(t_cub3d *cub3d)
{
	float	anim_time;
	float	move;

	if (!(cub3d->map.features & FEAT_ENEMY))
		return ;
	anim_time = cub3d->frame_timestamp - cub3d->enemy.anim_start_time;
	if (anim_time > ANIM_DURATION)
	{
		enemy_update_anim(cub3d);
		anim_time = cub3d->frame_timestamp - cub3d->enemy.anim_start_time;
	}
	anim_time += ANIM_START_OFFSET;
	move = enemy_anim_move_x(anim_time);
	cub3d->enemy.pos.x = (1 - move) * cub3d->enemy.anim_start_pos.x
		+ move * cub3d->enemy.anim_end_pos.x;
	cub3d->enemy.pos.y = (1 - move) * cub3d->enemy.anim_start_pos.y
		+ move * cub3d->enemy.anim_end_pos.y;
	cub3d->enemy.anim_width_half = enemy_anim_side_x(anim_time);
	cub3d->enemy.anim_y_pos = enemy_anim_bottom_y(anim_time);
	cub3d->enemy.anim_height = enemy_anim_height_y(anim_time);
}
