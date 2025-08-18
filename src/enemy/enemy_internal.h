#ifndef ENEMY_INTERNAL_H
# define ENEMY_INTERNAL_H

# include <stdbool.h>
# include <stdint.h>

# include "cub3d.h"

typedef struct s_enemy_pathing_tile
{
	bool		visited;
	float		distance;
	uint32_t	parent_idx;
}	t_enemy_pathing_tile;

typedef struct s_enemy_pathing_pq_entry
{
	uint32_t	tile_idx;
	float		distance;
}	t_enemy_pathing_pq_entry;

bool		enemy_pathing_init(t_cub3d *cub3d);

void		enemy_pathing_pq_insert(t_enemy_pathing_state *state,
				uint32_t tile_idx, float distance);
uint32_t	enemy_pathing_pq_extract(t_enemy_pathing_state *state);

void		enemy_pathing(t_cub3d *cub3d);

float		enemy_anim_height_y(float t);
float		enemy_anim_bottom_y(float t);
float		enemy_anim_side_x(float t);
float		enemy_anim_move_x(float t);

#endif
