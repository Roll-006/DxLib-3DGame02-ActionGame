#pragma once
#include <string>

struct AnimTimeStateData
{
	int   kind;
	int   attach_index;
	float play_timer;
};

struct AnimKindData
{
	int			anim_handle;
	std::string tag;
	float		play_speed;
	bool		is_loop;
};
