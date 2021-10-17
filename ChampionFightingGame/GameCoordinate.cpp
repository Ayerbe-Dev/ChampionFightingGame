#include "GameCoordinate.h"
#include "utils.h"

GameCoordinate::GameCoordinate()
{
	x = 0.0;
	y = 0.0;
	x_offset = 0.0;
	y_offset = 0.0;
}

GameCoordinate::GameCoordinate(f32 x, f32 y)
{
	this->x = x;
	this->y = y;
}

GameCoordinate::GameCoordinate(f32 window_width, f32 window_height, f32 start_x, f32 start_y)
{
	x = start_x;
	y = start_y;
	x_offset = window_width / 2;
	y_offset = window_height / 2;
}

f32 GameCoordinate::getRenderCoodrinateX()
{
	return x + x_offset - x_spr_offset;
}

f32 GameCoordinate::getRenderCoodrinateY()
{
	return (y * -1.0) + WINDOW_HEIGHT - y_spr_offset;
}