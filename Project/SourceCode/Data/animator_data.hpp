#pragma once
#include <string>

struct AnimTimeKindData
{
	int   kind				= -1;
	int   attach_index		= -1;
	float play_timer		= 0.0f;
	float total_time		= 0.0f;
};

struct AnimKindData
{
	int			anim_handle	= -1;
	int			index		= 0;
	std::string tag			= "";
	float		play_speed	= 0.0f;
	bool		is_loop		= true;
};
