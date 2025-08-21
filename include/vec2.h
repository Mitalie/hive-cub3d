/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec2.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:20:05 by amakinen          #+#    #+#             */
/*   Updated: 2025/08/21 15:20:05 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
