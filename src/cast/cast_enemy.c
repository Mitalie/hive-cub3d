#include "cast_internal.h"

#include "util.h"

static t_hit	*cast_insert_transparent(t_cast_result *cr, float distance)
{
	size_t	index;
	t_hit	*slot;

	if (cr->num_transparent == MAX_TRANSPARENT)
		return (NULL);
	index = 0;
	while (index < cr->num_transparent
		&& distance > cr->transparent[index].distance)
		index++;
	slot = &cr->transparent[index];
	util_memmove(slot + 1, slot, sizeof(*slot) * cr->num_transparent - index);
	cr->num_transparent++;
	return (slot);
}

/*
	Determine if current ray hits an enemy, and insert the hit in the
	transparent hits list if so.

	Scale of enemy_dir is affected by player_to_enemy distance, and results in
	incorrectly scaled intersection.position_on_target. Correct it by
	multiplying with the distance.
*/
void	cast_enemy(t_cast_state *state)
{
	t_enemy			*enemy;
	t_vec2			player_to_enemy;
	t_vec2			enemy_dir;
	t_intersection	intersection;
	t_hit			*slot;

	enemy = &state->cub3d->enemy;
	player_to_enemy.x = enemy->pos.x - state->cub3d->player.x;
	player_to_enemy.y = enemy->pos.y - state->cub3d->player.y;
	enemy_dir.x = player_to_enemy.y;
	enemy_dir.y = -player_to_enemy.x;
	intersection = intersect(state->pos, state->dir, enemy->pos, enemy_dir);
	intersection.position_on_target *= intersection.distance_along_ray;
	if (intersection.distance_along_ray < 0
		|| intersection.distance_along_ray > state->cr->opaque.distance
		|| intersection.position_on_target > enemy->anim_width_half
		|| intersection.position_on_target < -enemy->anim_width_half)
		return ;
	slot = cast_insert_transparent(state->cr, intersection.distance_along_ray);
	if (!slot)
		return ;
	slot->distance = intersection.distance_along_ray;
	slot->material = MAT_ENEMY;
	slot->position_in_tile = intersection.position_on_target
		/ enemy->anim_width_half * 0.5f + 0.5f;
}
