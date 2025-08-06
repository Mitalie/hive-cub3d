#ifndef ENEMY_H
# define ENEMY_H

# include "vec2.h"

typedef struct s_cub3d	t_cub3d;

typedef struct s_enemy
{
	t_vec2	pos;
	t_vec2	anim_start_pos;
	t_vec2	anim_end_pos;
	float	anim_start_time;
}	t_enemy;

void	enemy_render(t_cub3d *cub3d);
void	enemy_update(t_cub3d *cub3d);

#endif
