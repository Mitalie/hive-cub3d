/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:20:00 by amakinen          #+#    #+#             */
/*   Updated: 2025/08/21 15:20:00 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# include <stdbool.h>

# include "cub3d.h"

bool	render_setup(t_cub3d *cub3d);
void	render_cleanup(t_cub3d *cub3d);

void	render_view(t_cub3d *cub3d);

#endif
