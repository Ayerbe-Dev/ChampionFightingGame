#pragma once

class GameCoordinate
{
public:
	float x;
	float y;
	float x_offset;
	float y_offset;
	float x_spr_offset;
	float x_anim_offset;
	float y_spr_offset;
	float y_anim_offset;

	GameCoordinate();

	GameCoordinate(float window_width, float window_height);

	GameCoordinate(float window_width, float window_height, float start_x, float start_y);

	float getRenderCoodrinateX();

	float getRenderCoodrinateXAnim();

	float getRenderCoodrinateY();

	float getRenderCoodrinateYAnim();
};

