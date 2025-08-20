#ifndef MATERIAL_H
# define MATERIAL_H

# include <stdint.h>

typedef struct s_cub3d	t_cub3d;

typedef enum e_material
{
	MAT_WALL_NORTH,
	MAT_WALL_SOUTH,
	MAT_WALL_WEST,
	MAT_WALL_EAST,
	MAT_DOOR_FACE,
	MAT_DOOR_SIDE,
	MAT_TGT_INACTIVE,
	MAT_TGT_ACTIVE,
	MAT_TGT_COMPLETED,
	MAT_TGT_BACK,
	MAT_ENEMY,
}	t_material;

uint32_t	material_tex(t_cub3d *cub3d, t_material material,
				float xpos, float ypos);

#endif
