/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_parse_int.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:22:58 by amakinen          #+#    #+#             */
/*   Updated: 2025/08/21 15:22:59 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"

#include <stdbool.h>
#include <stdint.h>

static bool	util_isdigit(char c)
{
	return (c >= '0' && c <= '9');
}

bool	util_parse_uint8(char **str, uint8_t *value)
{
	uint8_t	digit;

	if (!util_isdigit(**str))
		return (false);
	*value = 0;
	while (util_isdigit(**str))
	{
		digit = *(*str)++ - '0';
		if (*value > (UINT8_MAX - digit) / 10)
			return (false);
		*value = *value * 10 + digit;
	}
	return (true);
}
