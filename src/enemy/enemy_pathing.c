/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_pathing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:20:57 by amakinen          #+#    #+#             */
/*   Updated: 2025/08/21 16:16:42 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "enemy_internal.h"
#include "enemy.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#include "cub3d.h"
#include "map.h"

/*
	A* search algorithm for 4-way grid movement, with true straight-line
	distance as the heuristic. This is a consistent heuristic, so A* is
	guaranteed to find an optimal path without processing any node more than
	once. Therefore we can omit the code for updating the best path to a node.
*/

static float	enemy_pathing_distance_heuristic(t_cub3d *cub3d,
	uint32_t tile_idx)
{
	float	x;
	float	y;
	float	dx;
	float	dy;

	x = tile_idx % cub3d->map.width + 0.5f;
	y = tile_idx / cub3d->map.width + 0.5f;
	dx = x - cub3d->player.x;
	dy = y - cub3d->player.y;
	return (sqrtf(dx * dx + dy * dy));
}

static void	enemy_pathing_goal(t_cub3d *cub3d,
	uint32_t curr_tile, uint32_t tile_idx)
{
	uint32_t	parent_tile;
	uint32_t	prev_tile;

	prev_tile = tile_idx;
	parent_tile = curr_tile;
	while (parent_tile != tile_idx)
	{
		prev_tile = tile_idx;
		tile_idx = parent_tile;
		parent_tile = cub3d->enemy_pathing.tiles[tile_idx].parent_idx;
	}
	cub3d->enemy.anim_end_pos.x = prev_tile % cub3d->map.width + 0.5f;
	cub3d->enemy.anim_end_pos.y = prev_tile / cub3d->map.width + 0.5f;
}

static bool	enemy_pathing_neighbor(t_cub3d *cub3d,
	uint32_t curr_tile, uint32_t tile_idx)
{
	float		distance;
	t_map_tile	tile;
	uint32_t	x;
	uint32_t	y;

	if (cub3d->enemy_pathing.tiles[tile_idx].visited)
		return (false);
	x = tile_idx % cub3d->map.width;
	y = tile_idx / cub3d->map.width;
	tile = map_tile(&cub3d->map, x, y);
	if (tile != TILE_EMPTY && tile != TILE_DOOR_EW && tile != TILE_DOOR_NS)
		return (false);
	cub3d->enemy_pathing.tiles[tile_idx].visited = true;
	cub3d->enemy_pathing.tiles[tile_idx].parent_idx = curr_tile;
	distance = cub3d->enemy_pathing.tiles[curr_tile].distance + 1;
	cub3d->enemy_pathing.tiles[tile_idx].distance = distance;
	if (x == floorf(cub3d->player.x) && y == floorf(cub3d->player.y))
	{
		enemy_pathing_goal(cub3d, curr_tile, tile_idx);
		return (true);
	}
	enemy_pathing_pq_insert(&cub3d->enemy_pathing, tile_idx,
		distance + enemy_pathing_distance_heuristic(cub3d, tile_idx));
	return (false);
}

static void	enemy_pathing_start(t_cub3d *cub3d, uint32_t map_size)
{
	uint32_t	i;
	uint32_t	curr_tile;

	i = 0;
	while (i < map_size)
		cub3d->enemy_pathing.tiles[i++].visited = false;
	cub3d->enemy_pathing.pq_size = 0;
	curr_tile = floorf(cub3d->enemy.pos.x)
		+ cub3d->map.width * floorf(cub3d->enemy.pos.y);
	cub3d->enemy_pathing.tiles[curr_tile].visited = true;
	cub3d->enemy_pathing.tiles[curr_tile].distance = 0;
	cub3d->enemy_pathing.tiles[curr_tile].parent_idx = curr_tile;
	enemy_pathing_pq_insert(&cub3d->enemy_pathing, curr_tile,
		enemy_pathing_distance_heuristic(cub3d, curr_tile));
}

void	enemy_pathing(t_cub3d *cub3d)
{
	uint32_t	curr_tile;
	uint32_t	map_size;
	bool		finished;

	if (floorf(cub3d->player.x) == floorf(cub3d->enemy.pos.x)
		&& floorf(cub3d->player.y) == floorf(cub3d->enemy.pos.y))
		return ;
	map_size = cub3d->map.width * cub3d->map.height;
	enemy_pathing_start(cub3d, map_size);
	finished = false;
	while (!finished && cub3d->enemy_pathing.pq_size > 0)
	{
		curr_tile = enemy_pathing_pq_extract(&cub3d->enemy_pathing);
		if (curr_tile >= 1)
			finished = enemy_pathing_neighbor(cub3d, curr_tile, curr_tile - 1);
		if (!finished && curr_tile < map_size - 1)
			finished = enemy_pathing_neighbor(cub3d, curr_tile, curr_tile + 1);
		if (!finished && curr_tile >= cub3d->map.width)
			finished = enemy_pathing_neighbor(cub3d, curr_tile,
					curr_tile - cub3d->map.width);
		if (!finished && curr_tile < map_size - cub3d->map.width)
			finished = enemy_pathing_neighbor(cub3d, curr_tile,
					curr_tile + cub3d->map.width);
	}
}
