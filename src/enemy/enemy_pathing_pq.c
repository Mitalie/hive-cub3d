#include "enemy_internal.h"
#include "enemy.h"

#include <stdint.h>

/*
	Priority queue for use in pathfinding. Based on a binary min-heap where
	each node always stores a lower value that its children. Therefore the
	lowest value (low distance = high priority) is always stored in the root
	node, and insertion/extraction move nodes if necessary to maintain the
	min-heap property.

	Because heap updates only move individual nodes instead of entire subtrees,
	we can efficiently store nodes in an array without needing pointers. Parent
	and child indices can always be calculated from current index:

	             0
	      1              2
	   3     4       5       6
	  7 8   9 10   11 12   13 14
	15 ...
*/

static uint32_t	pq_parent(uint32_t i)
{
	return ((i - 1) / 2);
}

static uint32_t	pq_left_child(uint32_t i)
{
	return (2 * i + 1);
}

static uint32_t	pq_right_child(uint32_t i)
{
	return (2 * i + 2);
}

/*
	Insert into last slot and bubble up in binary heap... except we bubble up
	the empty slot instead until we find the correct position, and only then
	write the new entry there.
*/
void	enemy_pathing_pq_insert(t_enemy_pathing_state *state,
	uint32_t tile_idx, float distance)
{
	uint32_t	current_idx;
	uint32_t	parent_idx;

	current_idx = state->pq_size++;
	while (current_idx > 0)
	{
		parent_idx = pq_parent(current_idx);
		if (distance >= state->pq[pq_parent(current_idx)].distance)
			break ;
		state->pq[current_idx] = state->pq[parent_idx];
		current_idx = parent_idx;
	}
	state->pq[current_idx].tile_idx = tile_idx;
	state->pq[current_idx].distance = distance;
}

/*
	Extract the value in first slot, then move last to first and bubble down in
	binary heap... except we bubble down the empty slot instead until we find
	the correct position, and only then write the last entry there.

	Returns only the tile index, as the caller just wants to know which tile to
	process next.

	Caller must ensure the queue is not empty before calling.
*/
uint32_t	enemy_pathing_pq_extract(t_enemy_pathing_state *state)
{
	uint32_t	best_tile_idx;
	uint32_t	current_idx;
	uint32_t	min_idx;
	uint32_t	child;

	best_tile_idx = state->pq[0].tile_idx;
	state->pq_size--;
	current_idx = 0;
	min_idx = 0;
	while (min_idx != state->pq_size)
	{
		min_idx = state->pq_size;
		child = pq_left_child(current_idx);
		if (child < state->pq_size
			&& state->pq[child].distance < state->pq[min_idx].distance)
			min_idx = child;
		child = pq_right_child(current_idx);
		if (child < state->pq_size
			&& state->pq[child].distance < state->pq[min_idx].distance)
			min_idx = child;
		state->pq[current_idx] = state->pq[min_idx];
		current_idx = min_idx;
	}
	return (best_tile_idx);
}
