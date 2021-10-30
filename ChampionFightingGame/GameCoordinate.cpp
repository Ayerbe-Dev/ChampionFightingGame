#include "GameCoordinate.h"
#include "utils.h"



GameCoordinate::GameCoordinate()
{
	x = 0.0;
	y = 0.0;
	x_offset = 0.0;
	y_offset = 0.0;
}

GameCoordinate::GameCoordinate(float x, float y)
{
	this->x = x;
	this->y = y;
}

GameCoordinate::GameCoordinate(float window_width, float window_height, float start_x, float start_y)
{
	x = start_x;
	y = start_y;
	x_offset = window_width / 2;
	y_offset = window_height / 2;
}

float GameCoordinate::getRenderCoodrinateX()
{
	return x + x_offset - x_spr_offset;
}

float GameCoordinate::getRenderCoodrinateXAnim()
{
	return x + x_offset - x_anim_offset;
}

float GameCoordinate::getRenderCoodrinateY()
{
	return (y * -1.0) + WINDOW_HEIGHT - y_spr_offset;
}

float GameCoordinate::getRenderCoodrinateYAnim()
{
	return (y * -1.0) + WINDOW_HEIGHT - y_anim_offset;
}