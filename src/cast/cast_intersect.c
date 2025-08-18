#include "cast_internal.h"

#include <math.h>

#include "vec2.h"

/*
	Calculates intersection of a line passing through `ray_start` in the
	direction `ray_dir`	with line y=`target_y`. Outputs distance along `ray_dir`
	in `.distance_along_ray` and x-coordinate of the intersection in
	`.position_on_target`.
*/
t_intersection	intersect_x(t_vec2 ray_start, t_vec2 ray_dir, float target_y)
{
	float			distance_along_ray;
	t_intersection	result;

	if (ray_dir.y == 0.0f)
	{
		result.distance_along_ray = INFINITY;
		result.position_on_target = NAN;
		return (result);
	}
	distance_along_ray = (target_y - ray_start.y) / ray_dir.y;
	result.distance_along_ray = distance_along_ray;
	result.position_on_target = distance_along_ray * ray_dir.x + ray_start.x;
	return (result);
}

/*
	Calculates intersection of a line passing through `ray_start` in the
	direction `ray_dir`	with line x=`target_x`. Outputs distance along `ray_dir`
	in `.distance_along_ray` and y-coordinate of the intersection in
	`.position_on_target`.
*/
t_intersection	intersect_y(t_vec2 ray_start, t_vec2 ray_dir, float target_x)
{
	float			distance_along_ray;
	t_intersection	result;

	if (ray_dir.x == 0.0f)
	{
		result.distance_along_ray = INFINITY;
		result.position_on_target = NAN;
		return (result);
	}
	distance_along_ray = (target_x - ray_start.x) / ray_dir.x;
	result.distance_along_ray = distance_along_ray;
	result.position_on_target = distance_along_ray * ray_dir.y + ray_start.y;
	return (result);
}

/*
	Calculates intersection of a line passing through `ray_start` in the
	direction `ray_dir`	with line passing through `target_start in the
	direction `target_dir`. Outputs distance along `ray_dir` in
	`.distance_along_ray` and distance along `target_dir` in
	`.position_on_target`.
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
		result.distance_along_ray = INFINITY;
		result.position_on_target = NAN;
		return (result);
	}
	dx = target_start.x - ray_start.x;
	dy = target_start.y - ray_start.y;
	result.distance_along_ray = (dx * target_dir.y - dy * target_dir.x) / det;
	result.position_on_target = (dx * ray_dir.y - dy * ray_dir.x) / det;
	return (result);
}
