/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:19:29 by amakinen          #+#    #+#             */
/*   Updated: 2025/08/21 15:19:30 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAST_H
# define CAST_H

# include <stddef.h>

# include "material.h"
# include "vec2.h"

typedef struct s_cub3d	t_cub3d;

typedef enum e_side
{
	HIT_NORTH,
	HIT_SOUTH,
	HIT_WEST,
	HIT_EAST,
}	t_side;

typedef struct s_hit
{
	float		distance;
	float		pos_in_tile;
	t_side		side;
	t_material	material;
}	t_hit;

# define MAX_TRANSPARENT 8

typedef struct s_cast_result
{
	t_hit	opaque;
	size_t	num_transparent;
	t_hit	transparent[MAX_TRANSPARENT];
}	t_cast_result;

void	cast(t_cub3d *cub3d, t_vec2 pos, t_vec2 dir, t_cast_result *out);

#endif
