#define G 10
#define A_CROUCH 5
#define CROUCH_DEPTH 0.05f
#define JUMP_TO_FLIGHT_HEIGHT 0.05f
#define A_JUMP_Y 20
#define FLIGHT_PEAK 0.25f

#define A_JUMP_X 20
#define V_FLIGHT_X 2
#define FLIGHT_START_X 0.1f

#define CROUCH_HALF_TIME 0.1f
#define CROUCH_BOTTOM_TIME 0.2f
#define JUMP_TIME 0.3f
#define FLIGHT_PEAK_TIME 0.5f

#define HEIGHT_BELOW_COM 0.48f
#define HEIGHT_ABOVE_COM_RATIO 0.5f
#define WIDTH_HALF 0.36f

static float	enemy_anim_crouch_top(float t)
{
	return (-A_CROUCH * 0.5f * t * t);
}

static float	enemy_anim_crouch_bottom(float t)
{
	t -= CROUCH_BOTTOM_TIME;
	return (-A_CROUCH * 0.5f * t * t - CROUCH_DEPTH);
}

static float	enemy_anim_jump(float t)
{
	t -= CROUCH_BOTTOM_TIME;
	return ((A_JUMP_Y) * 0.5f * t * t - CROUCH_DEPTH);
}

static float	enemy_anim_flight(float t)
{
	t -= FLIGHT_PEAK_TIME;
	return (-G * 0.5f * t * t + FLIGHT_PEAK);
}

static float	enemy_anim_move_y(float t)
{
	if (t <= 0)
		return (0);
	if (t <= CROUCH_HALF_TIME)
		return (enemy_anim_crouch_top(t));
	if (t <= CROUCH_BOTTOM_TIME)
		return (enemy_anim_crouch_bottom(t));
	if (t <= JUMP_TIME)
		return (enemy_anim_jump(t));
	if (t <= FLIGHT_PEAK_TIME)
		return (enemy_anim_flight(t));
	return (enemy_anim_move_y(2 * FLIGHT_PEAK_TIME - t));
}

static float	enemy_anim_squeeze_y(float t)
{
	if (t > JUMP_TIME && t <= 2 * FLIGHT_PEAK_TIME - JUMP_TIME)
		return (JUMP_TO_FLIGHT_HEIGHT);
	return (enemy_anim_move_y(t));
}

float	enemy_anim_top_y(float t)
{
	return (enemy_anim_move_y(t)
		+ HEIGHT_BELOW_COM * (1 + HEIGHT_ABOVE_COM_RATIO)
		+ enemy_anim_squeeze_y(t) * HEIGHT_ABOVE_COM_RATIO);
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
