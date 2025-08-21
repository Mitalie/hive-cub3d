/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:19:46 by amakinen          #+#    #+#             */
/*   Updated: 2025/08/21 15:19:46 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HOOKS_H
# define HOOKS_H

# include "MLX42/MLX42.h"

void	loop_hook(void *param);
void	key_hook(mlx_key_data_t data, void *param);
void	cursor_hook(double x, double y, void *param);

#endif
