/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:19:33 by amakinen          #+#    #+#             */
/*   Updated: 2025/08/21 15:19:34 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLOR_H
# define COLOR_H

# include <stdint.h>

uint32_t	color_interp(uint32_t a, uint32_t b, float t);

uint32_t	color_create(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

#endif
