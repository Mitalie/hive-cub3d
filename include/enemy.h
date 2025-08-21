/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:19:42 by amakinen          #+#    #+#             */
/*   Updated: 2025/08/21 15:19:43 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENEMY_H
# define ENEMY_H

# include <stdbool.h>
# include <stdint.h>

# include "vec2.h"

typedef struct s_cub3d					t_cub3d;

typedef struct s_enemy_pathing_tile		t_enemy_pathing_tile;
typedef struct s_enemy_pathing_pq_entry	t_enemy_pathing_pq_entry;

typedef struct s_enemy
{
	t_vec2	pos;
	t_vec2	anim_start_pos;
	t_vec2	anim_end_pos;
	float	anim_start_time;
	float	anim_width_half;
	float	anim_height;
	float	anim_y_pos;
}	t_enemy;

/*
	Allocate the arrays once for the program run and keep reusing them to avoid
	unnecessary dynamic allocation.

	`nodes` array must be equal in size to map size (number of tiles).
	`pq` is also equal to map size - this is overkill, but we don't know how
	large the queue can grow ahead of time.
*/
typedef struct s_enemy_pathing_state
{
	t_enemy_pathing_tile		*tiles;
	t_enemy_pathing_pq_entry	*pq;
	uint32_t					pq_size;
}	t_enemy_pathing_state;

bool	enemy_pathing_alloc(t_cub3d *cub3d);
void	enemy_pathing_dealloc(t_cub3d *cub3d);

void	enemy_update(t_cub3d *cub3d);

#endif
