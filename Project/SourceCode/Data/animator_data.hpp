#pragma once
#include <string>

struct AnimTimeKindData
{
	int   kind;
	int   attach_index;
	float play_timer;
	float total_time;

	AnimTimeKindData() : 
		kind(-1), attach_index(-1), play_timer(0.0f), total_time(0.0f){}

	AnimTimeKindData(const int kind, const int attach_index, const float play_timer, const float total_time) :
		kind(kind), attach_index(attach_index), play_timer(play_timer), total_time(total_time){}
};

struct AnimKindData
{
	int			anim_handle;
	int			index;
	std::string tag;
	float		play_speed;
	bool		is_loop;

	AnimKindData() : 
		anim_handle(-1), index(0), tag(""), play_speed(0.0f), is_loop(true){}

	AnimKindData(const int anim_handle, const int index, const std::string& tag, const float play_speed, const bool is_loop) :
		anim_handle(anim_handle), index(index), tag(tag), play_speed(play_speed), is_loop(is_loop){}
};
