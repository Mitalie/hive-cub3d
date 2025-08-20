#include "cast_internal.h"

#include <math.h>

#include "vec2.h"

/*
	Calculates intersection of a line passing through `ray_start` in the
	direction `ray_dir`	with line y=`target_y`. Outputs distance along `ray_dir`
	in `.ray_len` and x-coordinate of the intersection in `.tgt_pos`.
*/
t_intersection	intersect_x(t_vec2 ray_start, t_vec2 ray_dir, float target_y)
{
	float			ray_len;
	t_intersection	result;

	if (ray_dir.y == 0.0f)
	{
		result.ray_len = INFINITY;
		result.tgt_pos = NAN;
		return (result);
	}
	ray_len = (target_y - ray_start.y) / ray_dir.y;
	result.ray_len = ray_len;
	result.tgt_pos = ray_len * ray_dir.x + ray_start.x;
	return (result);
}

/*
	Calculates intersection of a line passing through `ray_start` in the
	direction `ray_dir`	with line x=`target_x`. Outputs distance along `ray_dir`
	in `.ray_len` and y-coordinate of the intersection in `.tgt_pos`.
*/
t_intersection	intersect_y(t_vec2 ray_start, t_vec2 ray_dir, float target_x)
{
	float			ray_len;
	t_intersection	result;

	if (ray_dir.x == 0.0f)
	{
		result.ray_len = INFINITY;
		result.tgt_pos = NAN;
		return (result);
	}
	ray_len = (target_x - ray_start.x) / ray_dir.x;
	result.ray_len = ray_len;
	result.tgt_pos = ray_len * ray_dir.y + ray_start.y;
	return (result);
}

/*
	Calculates intersection of a line passing through `ray_start` in the
	direction `ray_dir`	with line passing through `target_start in the
	direction `target_dir`. Outputs distance along `ray_dir` in
	`.ray_len` and distance along `target_dir` in `.tgt_pos`.
*/
t_intersection	intersect(t_vec2 ray_start, t_vec2 ray_dir,
	t_vec2 target_start, t_vec2 target_dir)
{
	float			det;
	float			dx;
	float			dy;
	t_intersection	result;

	det = ray_dir.x * target_dir.y - ray_dir.y * target_dir.x;
	if (det == 0.0f)
	{
		result.ray_len = INFINITY;
		result.tgt_pos = NAN;
		return (result);
	}
	dx = target_start.x - ray_start.x;
	dy = target_start.y - ray_start.y;
	result.ray_len = (dx * target_dir.y - dy * target_dir.x) / det;
	result.tgt_pos = (dx * ray_dir.y - dy * ray_dir.x) / det;
	return (result);
}
