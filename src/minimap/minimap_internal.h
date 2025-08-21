/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_internal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:22:10 by amakinen          #+#    #+#             */
/*   Updated: 2025/08/21 15:22:12 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIMAP_INTERNAL_H
# define MINIMAP_INTERNAL_H

# include <stdint.h>

# include "map.h"
# include "vec2.h"

typedef struct s_minimap_targets
{
	t_vec2		pos[MAX_TARGETS];
	uint32_t	num;
}	t_minimap_targets;

#endif
