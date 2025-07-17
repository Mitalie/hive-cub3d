#include "control.h"

#include <math.h>

#include "cub3d.h"
#include "vec2.h"

#define WALL_COLL_DIST 0.125f

/*
	This will break if movement length exceeds one grid unit
*/

/*
	For one coordinate axis, determine how far along the movement vector we can
	go before we reach the next collision check line.
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

static bool	control_check_wall(
	t_cub3d *cub3d, int row, int pos_in_row, bool vertical_row)
{
	if (vertical_row)
		return (map_tile_is_wall(&cub3d->map, row, pos_in_row));
	else
		return (map_tile_is_wall(&cub3d->map, pos_in_row, row));
}

/*
	Determine whether there is actually a wall where the movement vector
	intersects the collision check line. Return a multiplier that limits the
	movement if necessary (1.0 for no collision).

	`vert = true` checks against vertical check lines, for limiting `x`.
	`vert = false` checks against horizontal check lines, for limiting `y`.
*/
static float	control_move_limit(
	t_cub3d *cub3d, t_vec2 move, float dist_to_line, bool vert)
{
	int		row_to_check;
	float	intersect;
	float	intersect_whole;
	float	intersect_frac;

	if (vert)
	{
		row_to_check = floorf(cub3d->player.x + copysignf(1, move.x));
		intersect = cub3d->player.y + move.y * dist_to_line;
	}
	else
	{
		row_to_check = floorf(cub3d->player.y + copysignf(1, move.y));
		intersect = cub3d->player.x + move.x * dist_to_line;
	}
	intersect_frac = modff(intersect, &intersect_whole);
	if (control_check_wall(cub3d, row_to_check, intersect_whole, vert))
		return (dist_to_line);
	if (intersect_frac < WALL_COLL_DIST
		&& control_check_wall(cub3d, row_to_check, intersect_whole - 1, vert))
		return (dist_to_line);
	if (intersect_frac > (1.0f - WALL_COLL_DIST)
		&& control_check_wall(cub3d, row_to_check, intersect_whole + 1, vert))
		return (dist_to_line);
	return (1.0f);
}

/*
	Cut the movement vector short on one or both coordinate axes if it would
	collide with a wall, then apply it to modify player position.

	We first cut the component that hits a wall earlier, to allow sliding along
	the wall with the other component. Then we check cut that component too in
	case we slide into a corner.

	A safety factor is applied to prevent a rounding error placing the player on
	wrong side of the collision check line.
*/
static void	control_move_with_collision(t_cub3d *cub3d, t_vec2 move)
{
	float	dist_to_line_x;
	float	dist_to_line_y;

	dist_to_line_x = control_move_next_check_line(cub3d->player.x, move.x);
	dist_to_line_y = control_move_next_check_line(cub3d->player.y, move.y);
	if (dist_to_line_x < 1.0f && dist_to_line_x < dist_to_line_y)
		move.x *= control_move_limit(cub3d, move, dist_to_line_x, true);
	if (dist_to_line_y < 1.0f)
		move.y *= control_move_limit(cub3d, move, dist_to_line_y, false);
	if (dist_to_line_x < 1.0f && dist_to_line_x >= dist_to_line_y)
		move.x *= control_move_limit(cub3d, move, dist_to_line_x, true);
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
