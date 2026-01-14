#pragma once
#include <string>
#include <nlohmann/json.hpp>

struct AnimTimeKindData
{
	int   kind				= -1;
	int   attach_index		= -1;
	float play_timer		= 0.0f;
	float total_time		= 0.0f;
};


/// @brief アニメーションの個々のデータ
/// @brief is_self_blend 自分自身でブレンドを行うかを判定(最初と最後がループしていないアニメーションで使用)
struct AnimKindData
{
	int			anim_handle			= -1;
	
	std::string path				= "";
	int			index				= 0;
	std::string tag					= "";
	float		play_speed			= 0.0f;
	bool		is_loop				= true;
	bool		is_self_blend		= false;
	float		ground_play_rate	= 1.0f;
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, AnimKindData& animator_data)
{
	j_data.at("path")				.get_to(animator_data.path);
	j_data.at("index")				.get_to(animator_data.index);
	j_data.at("tag")				.get_to(animator_data.tag);
	j_data.at("play_speed")			.get_to(animator_data.play_speed);
	j_data.at("is_loop")			.get_to(animator_data.is_loop);
	j_data.at("is_self_blend")		.get_to(animator_data.is_self_blend);
	j_data.at("ground_play_rate")	.get_to(animator_data.ground_play_rate);
}

inline void to_json(nlohmann::json& j_data, const AnimKindData& animator_data)
{
	j_data = nlohmann::json
	{
		{ "path",				animator_data.path },
		{ "index",				animator_data.index },
		{ "tag",				animator_data.tag },
		{ "play_speed",			animator_data.play_speed },
		{ "is_loop",			animator_data.is_loop },
		{ "is_self_blend",		animator_data.is_self_blend },
		{ "ground_play_rate",	animator_data.ground_play_rate },
	};
}
#pragma endregion
