#pragma once
#include <string>

struct AnimTimeKindData
{
	int   kind;
	int   attach_index;
	bool  is_play;
	float play_timer;

	AnimTimeKindData() : 
		kind(-1), attach_index(-1), is_play(true), play_timer(0.0f){}

	AnimTimeKindData(const int kind, const int attach_index, const bool is_play, const float play_timer) :
		kind(kind), attach_index(attach_index), is_play(is_play), play_timer(play_timer){}
};

struct AnimKindData
{
	int			anim_handle;
	int			index;
	std::string tag;
	float		play_speed;
	bool		is_loop;

	AnimKindData() : 
		anim_handle(-1), index(0), tag(""), play_speed(0.0f), is_loop(true) { }

	AnimKindData(const int anim_handle, const int index, const std::string& tag, const float play_speed, const bool is_loop) :
		anim_handle(anim_handle), index(index), tag(tag), play_speed(play_speed), is_loop(is_loop){}
};
