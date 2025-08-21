/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:20:02 by amakinen          #+#    #+#             */
/*   Updated: 2025/08/21 15:20:03 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTIL_H
# define UTIL_H

# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>

bool		util_err_false(const char *msg, const char *arg);
bool		util_errno_false(const char *msg, const char *arg);

int			util_memcmp(const void *a, const void *b, size_t n);
void		util_memmove(void *dst, const void *src, size_t n);
void		*util_realloc(void *buf, size_t old_size, size_t new_size);

bool		util_parse_uint8(char **str, uint8_t *value);

uint32_t	util_random(uint32_t seed);

#endif
