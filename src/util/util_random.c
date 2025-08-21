/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_random.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:23:04 by amakinen          #+#    #+#             */
/*   Updated: 2025/08/21 15:23:04 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"

#include <stdint.h>

/*
	Pass zero for seed to not reseed.
	Uses Linear Congruential Generator algorithm.
*/
uint32_t	util_random(uint32_t seed)
{
	static uint32_t	state = 1;

	if (seed)
		state = seed;
	state = (1103515245 * state + 12345) & 0x7FFFFFFF;
	return (state);
}
