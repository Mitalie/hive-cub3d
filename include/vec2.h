#ifndef VEC2_H
# define VEC2_H

typedef struct s_vec2
{
	float	x;
	float	y;
}	t_vec2;

typedef struct s_vec2i
{
	int	x;
	int	y;
}	t_vec2i;

t_vec2	vec2_rotate(t_vec2 v, float degrees);

#endif
