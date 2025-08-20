#ifndef ENEMY_ANIM_INTERNAL_H
# define ENEMY_ANIM_INTERNAL_H

# define G 10
# define A_CROUCH 5
# define CROUCH_DEPTH 0.05f
# define JUMP_TO_FLIGHT_HEIGHT 0.05f
# define A_JUMP_Y 20
# define FLIGHT_PEAK 0.25f

# define A_JUMP_X 20
# define V_FLIGHT_X 2
# define FLIGHT_START_X 0.1f

# define CROUCH_HALF_TIME 0.1f
# define CROUCH_BOTTOM_TIME 0.2f
# define JUMP_TIME 0.3f
# define FLIGHT_PEAK_TIME 0.5f

# define HEIGHT_BELOW_COM 0.48f
# define HEIGHT_ABOVE_COM_RATIO 0.5f
# define WIDTH_HALF 0.36f

float	enemy_anim_move_y(float t);

#endif
