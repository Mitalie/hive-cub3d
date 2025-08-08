#define _GNU_SOURCE
#include "enemy_internal.h"
#include "enemy.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>

// list of tiles to store search metadata (2d array the size of map)
// - visited flag to avoid reprocessing tiles, initialize to false
// - distance to reach this tile
// - best route to tile (the neighbor this tile was first reached from)

// priority queue (space for all tiles)
// - insert new candidate tile
// - select best candidate tile

// ensure player tile is not also enemy tile
// - game over
// - we probably check this before calling the pathfinding code
// initialize queue with just player tile
// while
// - select best candidate
// - for each neighbor
// - - if neighbor is enemy, return current tile (last step player->enemy == first step enemy->player)
// - - if not visited and not impassable
// - - - set visited flag
// - - - set distance
// - - - set route
// - - - insert to queue with priority: distance + estimated remaining distance

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

#include <stdio.h>

static void	tile_xy(t_cub3d *cub3d, uint32_t tile_idx, char *arr)
{
	sprintf(arr, "%u,%u", tile_idx % cub3d->map.width, tile_idx / cub3d->map.width);
}

static void	print_queue(t_cub3d *cub3d)
{
	uint32_t 					i;
	uint32_t					line_pos;
	uint32_t					line_size;
	t_enemy_pathing_pq_entry	*entry;
	char						tilestr[22];

	return ;
	line_pos = 0;
	line_size = 1;
	i = 0;
	while (i < cub3d->enemy_pathing.pq_size)
	{
		entry = &cub3d->enemy_pathing.pq[i++];
		tile_xy(cub3d, entry->tile_idx, tilestr);
		printf("%.2f[%s]  ", entry->distance, tilestr);
		line_pos++;
		if (line_pos == line_size)
		{
			printf("\n");
			line_pos = 0;
			line_size += line_size;
		}
	}
	printf("\n");
}

static void	enemy_pathing_goal(t_cub3d *cub3d,
	uint32_t current_tile, uint32_t tile_idx)
{
	uint32_t	parent_tile;
	uint32_t	prev_tile;
	char		tilestr[22];

	tile_xy(cub3d, tile_idx, tilestr);
	printf("found player tile of %s\n", tilestr);
	prev_tile = tile_idx;
	parent_tile = current_tile;
	while (parent_tile != tile_idx)
	{
		prev_tile = tile_idx;
		tile_idx = parent_tile;
		parent_tile = cub3d->enemy_pathing.tiles[tile_idx].parent_idx;
		tile_xy(cub3d, tile_idx, tilestr);
		printf("  reached from %s with dist %f\n", tilestr, cub3d->enemy_pathing.tiles[tile_idx].distance + 1);
	}
	tile_xy(cub3d, prev_tile, tilestr);
	printf("animating enemy to %s\n", tilestr);
	cub3d->enemy.anim_end_pos.x = prev_tile % cub3d->map.width + 0.5f;
	cub3d->enemy.anim_end_pos.y = prev_tile / cub3d->map.width + 0.5f;
}

static bool	enemy_pathing_neighbor(t_cub3d *cub3d,
	uint32_t current_tile, uint32_t tile_idx)
{
	float		distance;
	t_map_tile	tile;
	uint32_t	x;
	uint32_t	y;
	char		tilestr[22];

	if (cub3d->enemy_pathing.tiles[tile_idx].visited)
		return (false);
	x = tile_idx % cub3d->map.width;
	y = tile_idx / cub3d->map.width;
	tile = map_tile(&cub3d->map, x, y);
	if (tile != TILE_EMPTY && tile != TILE_DOOR_EW && tile != TILE_DOOR_NS)
		return (false);
	cub3d->enemy_pathing.tiles[tile_idx].visited = true;
	cub3d->enemy_pathing.tiles[tile_idx].parent_idx = current_tile;
	distance = cub3d->enemy_pathing.tiles[current_tile].distance + 1;
	cub3d->enemy_pathing.tiles[tile_idx].distance = distance;
	if (x == floorf(cub3d->player.x) && y == floorf(cub3d->player.y))
	{
		enemy_pathing_goal(cub3d, current_tile, tile_idx);
		return (true);
	}
	tile_xy(cub3d, tile_idx, tilestr);
	printf("    queueing tile %s with dist + heuristic %f\n", tilestr,
		distance + enemy_pathing_distance_heuristic(cub3d, tile_idx));
	enemy_pathing_pq_insert(&cub3d->enemy_pathing, tile_idx,
		distance + enemy_pathing_distance_heuristic(cub3d, tile_idx));
	print_queue(cub3d);
	return (false);
}

static void	enemy_pathing_start(t_cub3d *cub3d, uint32_t map_size)
{
	uint32_t	i;
	uint32_t	current_tile;
	char		tilestr1[22];
	char		tilestr2[22];

	i = 0;
	while (i < map_size)
		cub3d->enemy_pathing.tiles[i++].visited = false;
	cub3d->enemy_pathing.pq_size = 0;
	current_tile = floorf(cub3d->enemy.pos.x) + cub3d->map.width * floorf(cub3d->enemy.pos.y);
	cub3d->enemy_pathing.tiles[current_tile].visited = true;
	cub3d->enemy_pathing.tiles[current_tile].distance = 0;
	cub3d->enemy_pathing.tiles[current_tile].parent_idx = current_tile;
	tile_xy(cub3d, current_tile, tilestr1);
	tile_xy(cub3d, floorf(cub3d->player.x) + cub3d->map.width * floorf(cub3d->player.y), tilestr2);
	printf("pathfind from enemy tile %s to player tile %s\n", tilestr1, tilestr2);
	enemy_pathing_pq_insert(&cub3d->enemy_pathing, current_tile,
		enemy_pathing_distance_heuristic(cub3d, current_tile));
}

void	enemy_pathing(t_cub3d *cub3d)
{
	uint32_t	current_tile;
	uint32_t	map_size;
	bool		finished;
	char		tilestr[22];

	// ensure player tile is not also enemy tile
	if (floorf(cub3d->player.x) == floorf(cub3d->enemy.pos.x)
		&& floorf(cub3d->player.y) == floorf(cub3d->enemy.pos.y))
		return ;
	map_size = cub3d->map.width * cub3d->map.height;
	enemy_pathing_start(cub3d, map_size);
	finished = false;
	while (!finished)
	{
		current_tile = enemy_pathing_pq_extract(&cub3d->enemy_pathing);
		tile_xy(cub3d, current_tile, tilestr);
		printf("  considering tile %s with dist + heuristic %f\n", tilestr,
			cub3d->enemy_pathing.tiles[current_tile].distance + enemy_pathing_distance_heuristic(cub3d, current_tile));
		print_queue(cub3d);
		if (current_tile >= 1)
			finished = enemy_pathing_neighbor(cub3d, current_tile, current_tile - 1);
		if (!finished && current_tile < map_size - 1)
			finished = enemy_pathing_neighbor(cub3d, current_tile, current_tile + 1);
		if (!finished && current_tile >= cub3d->map.width)
			finished = enemy_pathing_neighbor(cub3d, current_tile, current_tile - cub3d->map.width);
		if (!finished && current_tile < map_size - cub3d->map.width)
			finished = enemy_pathing_neighbor(cub3d, current_tile, current_tile + cub3d->map.width);
	}
}
