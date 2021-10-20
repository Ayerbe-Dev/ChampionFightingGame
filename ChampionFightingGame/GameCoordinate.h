#pragma once
#include "utils.h"
class GameCoordinate
{
public:
	f32 x;
	f32 y;
	f32 x_offset;
	f32 y_offset;
	f32 x_spr_offset;
	f32 x_anim_offset;
	f32 y_spr_offset;
	f32 y_anim_offset;

	GameCoordinate();

	GameCoordinate(f32 window_width, f32 window_height);

	GameCoordinate(f32 window_width, f32 window_height, f32 start_x, f32 start_y);

	f32 getRenderCoodrinateX();

	f32 getRenderCoodrinateXAnim();

	f32 getRenderCoodrinateY();

	f32 getRenderCoodrinateYAnim();
};

