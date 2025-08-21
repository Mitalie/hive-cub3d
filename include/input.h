/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:19:48 by amakinen          #+#    #+#             */
/*   Updated: 2025/08/21 15:19:49 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# include "MLX42/MLX42.h"

# include "cub3d.h"

void	input_timed(t_cub3d *cub3d);
void	input_key(t_cub3d *cub3d, mlx_key_data_t key_data);
void	input_cursor(t_cub3d *cub3d, float dx, float dy);

#endif
