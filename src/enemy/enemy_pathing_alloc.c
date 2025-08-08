#include "enemy.h"
#include "enemy_internal.h"

#include <stdint.h>
#include <stdlib.h>

#include "cub3d.h"

bool	enemy_pathing_alloc(t_cub3d *cub3d)
{
	uint32_t	map_size;

	map_size = cub3d->map.width * cub3d->map.height;
	cub3d->enemy_pathing.tiles = malloc(map_size * sizeof(*cub3d->enemy_pathing.tiles));
	cub3d->enemy_pathing.pq = malloc(map_size * sizeof(*cub3d->enemy_pathing.pq));
	cub3d->enemy_pathing.pq_size = 0;
	return (cub3d->enemy_pathing.tiles != NULL
		&& cub3d->enemy_pathing.pq != NULL);
}

void	enemy_pathing_dealloc(t_cub3d *cub3d)
{
	free(cub3d->enemy_pathing.tiles);
	free(cub3d->enemy_pathing.pq);
}
