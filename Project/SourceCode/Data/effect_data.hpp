#pragma once
#include <string>

struct EffectData
{
	std::string obj_name		= "";
	std::string file_path		= "";
	float		play_speed		= 0.0f;
	float		play_wait_time	= 0.0f;
	bool		is_loop			= false;
};
