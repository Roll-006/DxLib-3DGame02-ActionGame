#pragma once
#include <string>

struct AnimTimeStateData
{
	int   kind;
	int   attach_index;
	float play_timer;

	AnimTimeStateData() : 
		kind(-1), attach_index(-1), play_timer(0.0f){}

	AnimTimeStateData(int kind, int attach_index, float play_timer) : 
		kind(kind), attach_index(attach_index), play_timer(play_timer){}
};

struct AnimKindData
{
	int			anim_handle;
	std::string tag;
	float		play_speed;
	bool		is_loop;

	AnimKindData() : 
		anim_handle(-1), tag(""), play_speed(0.0f), is_loop(true) {}

	AnimKindData(int anim_handle, std::string tag, float play_speed, bool is_loop) : 
		anim_handle(anim_handle), tag(tag), play_speed(play_speed), is_loop(is_loop){}
};
