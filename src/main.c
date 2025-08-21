#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/time.h>
#include "MLX42/MLX42.h"

#include "cub3d.h"
#include "hooks.h"
#include "enemy.h"
#include "map.h"
#include "minimap.h"
#include "render.h"
#include "util.h"

/*
	If you see a diagonal starting angle, fix map parsing
*/
static float	initial_facing(char player_start)
{
	if (player_start == 'N')
		return (0);
	else if (player_start == 'E')
		return (90);
	else if (player_start == 'S')
		return (180);
	else if (player_start == 'W')
		return (270);
	return (45);
}

static bool	initialize_modules(t_cub3d *cub3d, int argc, char **argv)
{
	bool	success;

	success = true;
	if (argc != 2)
		success = util_err_false(
				"Invalid arguments, usage: cub3D <path/to/map.cub>", NULL);
	if (success)
		success = map_load(&cub3d->map, argv[1]);
	if (success)
		success = enemy_pathing_alloc(cub3d);
	if (success)
	{
		cub3d->mlx = mlx_init(1920, 1080, "SIM-ulator", true);
		if (!cub3d->mlx)
			success = util_err_false("Failed to initialize MLX42", NULL);
	}
	if (success)
		if (!mlx_loop_hook(cub3d->mlx, loop_hook, cub3d))
			success = util_err_false(
					"Failed to set up loop hook with MLX42", NULL);
	if (success)
		mlx_key_hook(cub3d->mlx, key_hook, cub3d);
	if (success)
		mlx_cursor_hook(cub3d->mlx, cursor_hook, cub3d);
	return (success);
}

static void	cleanup_modules(t_cub3d *cub3d)
{
	render_cleanup(cub3d);
	minimap_cleanup(cub3d);
	if (cub3d->mlx)
		mlx_terminate(cub3d->mlx);
	enemy_pathing_dealloc(cub3d);
	map_unload(&cub3d->map);
}

int	main(int argc, char **argv)
{
	t_cub3d			cub3d;
	struct timeval	t;

	gettimeofday(&t, NULL);
	util_random(t.tv_sec);
	cub3d = (t_cub3d){};
	if (initialize_modules(&cub3d, argc, argv))
	{
		mlx_set_cursor_mode(cub3d.mlx, MLX_MOUSE_DISABLED);
		cub3d.hfov_deg = 90;
		cub3d.player.x = cub3d.map.player_x + 0.5;
		cub3d.player.y = cub3d.map.player_y + 0.5;
		cub3d.player_facing = initial_facing(cub3d.map.player_start);
		cub3d.enemy.pos.x = cub3d.map.enemy_start_x + 0.5f;
		cub3d.enemy.pos.y = cub3d.map.enemy_start_y + 0.5f;
		cub3d.enemy.anim_start_time = -INFINITY;
		cub3d.cursor_ready = false;
		mlx_loop(cub3d.mlx);
	}
	cleanup_modules(&cub3d);
}
