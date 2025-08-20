#include "../enemy_internal.h"
#include "enemy_anim_internal.h"

static float	enemy_anim_squeeze_y(float t)
{
	if (t > JUMP_TIME && t <= 2 * FLIGHT_PEAK_TIME - JUMP_TIME)
		return (JUMP_TO_FLIGHT_HEIGHT);
	return (enemy_anim_move_y(t));
}

float	enemy_anim_height_y(float t)
{
	float	vertical_squeezed;

	vertical_squeezed = HEIGHT_BELOW_COM + enemy_anim_squeeze_y(t);
	return (vertical_squeezed * (1 + HEIGHT_ABOVE_COM_RATIO));
}

float	enemy_anim_bottom_y(float t)
{
	return (enemy_anim_move_y(t) - enemy_anim_squeeze_y(t));
}

float	enemy_anim_side_x(float t)
{
	float	vertical_squeezed;
	float	horizontal_squeezed;

	vertical_squeezed = HEIGHT_BELOW_COM + enemy_anim_squeeze_y(t);
	horizontal_squeezed = WIDTH_HALF * HEIGHT_BELOW_COM / vertical_squeezed;
	return (horizontal_squeezed);
}

float	enemy_anim_move_x(float t)
{
	if (t < CROUCH_BOTTOM_TIME)
		return (0);
	if (t < JUMP_TIME)
	{
		t -= CROUCH_BOTTOM_TIME;
		return (A_JUMP_X * 0.5f * t * t);
	}
	if (t < 2 * FLIGHT_PEAK_TIME - JUMP_TIME)
	{
		t -= JUMP_TIME;
		return (V_FLIGHT_X * t + FLIGHT_START_X);
	}
	if (t < 2 * FLIGHT_PEAK_TIME - CROUCH_BOTTOM_TIME)
	{
		t -= 2 * FLIGHT_PEAK_TIME - CROUCH_BOTTOM_TIME;
		return (-A_JUMP_X * 0.5f * t * t + 1);
	}
	return (1);
}
