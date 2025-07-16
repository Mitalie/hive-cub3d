#include "control.h"

#include <math.h>

#include "cast.h"
#include "cub3d.h"
#include "vec2.h"

#define WALL_COLL_DIST 0.125f

#include <stdio.h>

/*
	This will break if movement length exceeds one grid unit
*/

static float	control_move_next_check_line(float pos, float dir)
{
	float	next_line_coord;
	if (dir == 0.0f || dir == -0.0f)
		return (INFINITY);
	if (dir > 0.0f)
	{
		next_line_coord = ceilf(pos + WALL_COLL_DIST) - WALL_COLL_DIST;
		return ((next_line_coord - pos) / dir);
	}
	next_line_coord = floorf(pos - WALL_COLL_DIST) + WALL_COLL_DIST;
	return ((pos - next_line_coord) / -dir);
}

static bool	control_check_wall(t_cub3d *cub3d, int x, int y)
{
	char	tile;

	if (x < 0
		|| (uint32_t)x >= cub3d->map.width
		|| y < 0
		|| (uint32_t)y >= cub3d->map.height)
		return (true);
	tile = cub3d->map.grid[y * cub3d->map.width + x];
	return (tile == '1');
}

static void	control_move_with_collision(t_cub3d *cub3d, t_vec2 move)
{
	float	dist_to_x;
	float	dist_to_y;
	float	intersection;
	float	intersection_whole;
	int		next_row;

	dist_to_x = control_move_next_check_line(cub3d->player.x, move.x);
	dist_to_y = control_move_next_check_line(cub3d->player.y, move.y);
	if (dist_to_x < 1.0f)
	{
		next_row = floor(cub3d->player.x + copysignf(1, move.x));
		intersection = cub3d->player.y + move.y * dist_to_x;
		intersection = modff(intersection, &intersection_whole);
		if ((intersection < WALL_COLL_DIST && control_check_wall(cub3d, next_row, intersection_whole - 1))
			|| control_check_wall(cub3d, next_row, intersection_whole)
			|| ((intersection > (1.0f - WALL_COLL_DIST) && control_check_wall(cub3d, next_row, intersection_whole + 1))))
			move.x *= dist_to_x;
	}
	if (dist_to_y < 1.0f)
	{
		next_row = floor(cub3d->player.y + copysignf(1, move.y));
		intersection = cub3d->player.x + move.x * dist_to_y;
		intersection = modff(intersection, &intersection_whole);
		if ((intersection < WALL_COLL_DIST && control_check_wall(cub3d, intersection_whole - 1, next_row))
			|| control_check_wall(cub3d, intersection_whole, next_row)
			|| ((intersection > (1.0f - WALL_COLL_DIST) && control_check_wall(cub3d, intersection_whole + 1, next_row))))
			move.y *= dist_to_y;
	}
	cub3d->player.x += move.x * 0.99f;
	cub3d->player.y += move.y * 0.99f;
}

/*
	First set up the move vector as if facing north, then rotate it based on
	actual facing direction and apply the rotated movement to player position.
*/
void	control_move(t_cub3d *cub3d, int arg, float time)
{
	float	speed;
	float	distance;
	t_vec2	move;

	speed = 1.0;
	if (mlx_is_key_down(cub3d->mlx, MLX_KEY_LEFT_SHIFT))
		speed = 3.0;
	distance = speed * time;
	move.x = 0;
	move.y = 0;
	if (arg == CONTROL_FORWARD)
		move.y = -distance;
	if (arg == CONTROL_BACKWARD)
		move.y = distance;
	if (arg == CONTROL_LEFT)
		move.x = -distance;
	if (arg == CONTROL_RIGHT)
		move.x = distance;
	move = vec2_rotate(move, cub3d->player_facing);
	control_move_with_collision(cub3d, move);
}

void	control_turn(t_cub3d *cub3d, int arg, float time)
{
	float	turn_speed;
	float	angle;

	turn_speed = 90.0;
	angle = turn_speed * time;
	if (arg == CONTROL_TURN_LEFT)
		cub3d->player_facing = fmodf(cub3d->player_facing - angle, 360);
	if (arg == CONTROL_TURN_RIGHT)
		cub3d->player_facing = fmodf(cub3d->player_facing + angle, 360);
}
