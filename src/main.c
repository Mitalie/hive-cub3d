/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:00:04 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/25 16:01:03 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"

int	main(void)
{
	mlx_t	*mlx;

	mlx = mlx_init(800, 600, "fdf", true);
	if (!mlx)
		return (1);
	mlx_loop(mlx);
	mlx_terminate(mlx);
}
