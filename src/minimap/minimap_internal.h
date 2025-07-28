#ifndef MINIMAP_INTERNAL_H
# define MINIMAP_INTERNAL_H

# include <stdint.h>

# include "map.h"
# include "vec2.h"

typedef struct s_minimap_targets
{
	t_vec2		pos[MAX_TARGETS];
	uint32_t	num;
}	t_minimap_targets;

#endif
