/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:23:07 by amakinen          #+#    #+#             */
/*   Updated: 2025/08/21 15:23:08 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "color.h"

#include <math.h>
#include <stdint.h>

uint32_t	color_interp(uint32_t a, uint32_t b, float t)
{
	uint32_t	i;
	uint32_t	out;
	float		at;
	float		bt;

	t = fmaxf(0, fminf(1, t));
	i = 0;
	out = 0;
	while (i < 32)
	{
		at = (a >> i & 0xff) * (1 - t);
		bt = (b >> i & 0xff) * t;
		out |= (uint32_t)(at + bt) << i;
		i += 8;
	}
	return (out);
}

uint32_t	color_create(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	return ((uint32_t)r << 24
		| (uint32_t)g << 16
		| (uint32_t)b << 8
		| (uint32_t)a
	);
}
